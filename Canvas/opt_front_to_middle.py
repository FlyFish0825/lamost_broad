"""Export the optimized mappings embedded in canvas.html and synchronize LED tables.

Run: python opt_front_to_middle.py
Uses the HTML's saved default mappings, not unsaved browser edits.
No third-party packages are required. Geometry and wiring are not re-optimized.
Legacy back-map A keys are sequence keys; middleId is the authoritative endpoint.
"""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent

def require(ok, message):
    if not ok:
        raise ValueError(message)

def read_text(path):
    return path.read_bytes().decode('utf-8-sig')

def constant(html, name):
    match = re.search(r'const\s+' + name + r'\s*=\s*(\{[^\r\n]*\});', html)
    require(match is not None, 'Missing constant: ' + name)
    return json.loads(match.group(1))

def main():
    html_path = ROOT / 'canvas.html'
    group_path = ROOT / 'main_node_pcb_groups.json'
    led_path = ROOT.parent / 'lamost_broad_MCU/Core/pcb_broad/just_led.c'
    html = read_text(html_path)
    front = constant(html, 'OPTIMIZED_FRONT_MAPPING')
    old_back = constant(html, 'OPTIMIZED_BACK_MAPPING')
    require(set(front) == {f'A{i}' for i in range(1, 727)}, 'Expected A1 through A726')
    require(all(isinstance(v, str) and re.fullmatch(r'L\d{3}', v) for v in front.values()), 'Invalid middle ID')
    require(len(set(front.values())) == 726, 'Duplicate middle endpoints')
    active = sorted(front.values(), key=lambda v: int(v[1:]))
    require(all(1 <= int(v[1:]) <= 742 for v in active), 'Middle ID out of range')
    inverse = {middle: fid for fid, middle in front.items()}
    require(set(old_back) == {f'A{i}' for i in range(1, 727)}, 'Expected 726 legacy sequence keys')
    back = {}
    middle_back = {}
    tables = [[0] * 16 for _ in range(52)]
    port_details = {}
    for i in range(1, 727):
        key = f'A{i}'
        record = old_back[key]
        mid = record.get('middleId', active[i-1])
        require(mid in inverse and mid not in middle_back, 'Missing/duplicate middle endpoint')
        pcb, port = record['pcb'], record['port']
        require(type(pcb) is int and type(port) is int and 0 <= pcb < 52 and 0 <= port < 16, 'Invalid PCB/port')
        require((pcb, port) not in port_details, 'Duplicate PCB port')
        fid = inverse[mid]
        back[key] = {'pcb': pcb, 'port': port, 'backId': f'{pcb}:{port}', 'middleId': mid, 'frontId': fid}
        middle_back[mid] = back[key]
        tables[pcb][port] = int(fid[1:])
        port_details[pcb, port] = {'middle_id': mid, 'front_id': fid}
    values = [n for row in tables for n in row if n]
    require(sorted(values) == list(range(1, 727)), 'LED IDs are not a complete bijection')
    require(sum(n == 0 for row in tables for n in row) == 106, 'Expected 106 reserved ports')

    groups = json.loads(read_text(group_path))
    require(len(groups['groups']) == 26, 'Expected 26 main nodes')
    require(sorted(g['main_node'] for g in groups['groups']) == list(range(1, 27)), 'Invalid main-node IDs')
    seen = []
    for group in groups['groups']:
        require(len(group['pcbs']) == 2 and sorted(p['local_can_id'] for p in group['pcbs']) == [0, 1], 'Invalid local CAN group')
        for entry in group['pcbs']:
            pcb = entry['pcb']
            require(1 <= pcb <= 52, 'Invalid global PCB number')
            seen.append(pcb)
            entry['ports'] = [{'port': j+1, **port_details.get((pcb-1, j), {'middle_id': None, 'front_id': None})} for j in range(16)]
    require(sorted(seen) == list(range(1, 53)), 'PCB groups must cover all 52 boards exactly once')
    groups['port_numbering'] = 'ports.port 为 USB/LED 的 1~16 编号；middle_id/front_id 为 null 表示保留接口。'
    groups['mapping_source'] = 'canvas.html: 焦面 front_id → 理线 middle_id → PCB/port；后端 JSON 的历史 A 键仅为顺序索引，以 middleId/frontId 字段为准。'

    led = read_text(led_path)
    pattern = r'(#(?:if|elif) PCB_ID == (\d+)\s+const uint16_t pcb_map\[JUST_BOARD_LED_NUMBER\] = \{)(.*?)(\n\};)'
    newline = '\r\n' if '\r\n' in led else '\n'
    found = []
    def replace_table(match):
        pcb = int(match.group(2))
        require(1 <= pcb <= 52, 'Unexpected PCB conditional')
        found.append(pcb)
        row = tables[pcb-1]
        body = newline + '    ' + ', '.join(f'{n}U' for n in row[:8]) + ',' + newline + '    ' + ', '.join(f'{n}U' for n in row[8:])
        return match.group(1) + body + newline + '};'
    led_new = re.sub(pattern, replace_table, led, flags=re.S)
    require(sorted(found) == list(range(1, 53)), 'Expected 52 LED map blocks')
    html_new, n = re.subn(r'const OPTIMIZED_BACK_MAPPING=\{[^\r\n]*\};', 'const OPTIMIZED_BACK_MAPPING=' + json.dumps(back, ensure_ascii=False, separators=(',', ':')) + ';', html)
    require(n == 1, 'Back-map replacement failed')
    old = 'const rid=activeRouteIds[f.seq-1];'
    new = 'const rid=b.middleId||activeRouteIds[f.seq-1];'
    if old in html_new:
        html_new = html_new.replace(old, new, 1)
    require(new in html_new, 'Unsupported back mapping loader')
    outputs = {
        html_path: html_new,
        ROOT / 'optimized_front_to_middle.json': json.dumps(front, ensure_ascii=False, indent=2) + '\n',
        ROOT / 'optimized_middle_to_back.json': json.dumps(back, ensure_ascii=False, indent=2) + '\n',
        group_path: json.dumps(groups, ensure_ascii=False, indent=2) + '\n',
        led_path: led_new,
    }
    # All validation above completes before any file is written.
    for path, text in outputs.items():
        path.write_bytes(text.encode('utf-8'))
    print('Exported 726 complete A -> L -> PCB/port mappings; 52 LED tables, 106 reserved ports, 26 CAN groups.')

if __name__ == '__main__':
    main()
