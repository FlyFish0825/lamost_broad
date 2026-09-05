import re, json, math
from pathlib import Path
import numpy as np
from scipy.optimize import linear_sum_assignment

ROOT = Path(r'F:\file\BaiduSyncdisk\Project\lamost_broad\Canvas')
HTML = ROOT / 'canvas.html'
s = HTML.read_text(encoding='utf-8')
fronts = json.loads(re.search(r'FRONT_COORDS\s*=\s*(\[.*?\]);', s, re.S).group(1))
rows = [9,13,17,21,25,25,29,29,31,31,33,30,26,26,26]
ys = [310,290,270,250,230,210,190,170,150,130,110,90,70,50,30]
pts = []

def half_width(y):
    q = abs(y)
    if q <= 145:
        return 335.0
    t = (325.0 - q) / 180.0
    return 165.0 + max(0.0, min(1.0, t)) * 170.0

def add_row(y, count):
    xmax = half_width(y) - 14.0
    if abs(y) <= 50:
        n = count // 2
        gap = math.sqrt(max(0.0, 60.0**2-y*y)) + 14.0
        xs = [-xmax+i*(xmax-gap)/(n-1) for i in range(n)] + [gap+i*(xmax-gap)/(n-1) for i in range(n)]
    else:
        xs = [-xmax+i*(2*xmax)/(count-1) for i in range(count)]
    for x in xs:
        pts.append({'x':x,'y':float(y)})
for y,n in zip(ys,rows): add_row(y,n)
for y,n in zip(reversed(ys),reversed(rows)): add_row(-y,n)
for i,q in enumerate(pts,1): q['id']=f'L{i:03d}'

top8=sorted([q for q in pts if q['y']>0],key=lambda q:math.hypot(q['x'],q['y']),reverse=True)[:8]
spare={q['id'] for q in top8}
for q in top8:
    mirror=min(pts,key=lambda p:abs(p['x']-q['x'])+abs(p['y']+q['y']))
    spare.add(mirror['id'])
active=[q for q in pts if q['id'] not in spare]
assert len(active)==726 and len(fronts)==726

F=np.array([[q[1],q[2]] for q in fronts],dtype=float)
T=np.array([[q['x'],q['y']] for q in active],dtype=float)
dx=F[:,None,0]-T[None,:,0]
dy=F[:,None,1]-T[None,:,1]
cost=dx*dx+dy*dy
upper_bad=(F[:,None,1]>8.0)&(T[None,:,1]<0.0)
lower_bad=(F[:,None,1]<-8.0)&(T[None,:,1]>0.0)
cost+=(upper_bad|lower_bad)*1.0e7
side_bad=(np.abs(F[:,None,0])>30.0)&(np.abs(T[None,:,0])>30.0)&(np.sign(F[:,None,0])!=np.sign(T[None,:,0]))
cost+=side_bad*2.0e5
ri,ci=linear_sum_assignment(cost)
assign=np.empty(len(fronts),dtype=int)
assign[ri]=ci
def cross_with(i,ti,j,tj):
    a=F[i]; b=T[ti]; c=F[j]; d=T[tj]
    rx,ry=b[0]-a[0],b[1]-a[1]
    sx,sy=d[0]-c[0],d[1]-c[1]
    den=rx*sy-ry*sx
    if abs(den)<1e-9: return False
    qx,qy=c[0]-a[0],c[1]-a[1]
    t=(qx*sy-qy*sx)/den
    u=(qx*ry-qy*rx)/den
    return 1e-6<t<1-1e-6 and 1e-6<u<1-1e-6

def cross_pairs():
    out=[]
    n=len(assign)
    for i in range(n):
        ai=assign[i]
        for j in range(i+1,n):
            if cross_with(i,ai,j,assign[j]): out.append((i,j))
    return out

def valid_target(i,tidx):
    fy,ty=F[i,1],T[tidx,1]
    if fy>8 and ty<0: return False
    if fy<-8 and ty>0: return False
    fx,tx=F[i,0],T[tidx,0]
    if abs(fx)>30 and abs(tx)>30 and np.sign(fx)!=np.sign(tx): return False
    return True

def local_delta(i,j):
    ti,tj=assign[i],assign[j]
    old=new=0
    for k in range(len(assign)):
        if k==i or k==j: continue
        tk=assign[k]
        old+=cross_with(i,ti,k,tk)+cross_with(j,tj,k,tk)
        new+=cross_with(i,tj,k,tk)+cross_with(j,ti,k,tk)
    old+=cross_with(i,ti,j,tj)
    new+=cross_with(i,tj,j,ti)
    old_len=np.linalg.norm(F[i]-T[ti])+np.linalg.norm(F[j]-T[tj])
    new_len=np.linalg.norm(F[i]-T[tj])+np.linalg.norm(F[j]-T[ti])
    return new-old,float(new_len-old_len)

initial_cross=len(cross_pairs())
for _ in range(400):
    pairs=cross_pairs()
    best=None
    best_score=0.0
    for i,j in pairs:
        ti,tj=assign[i],assign[j]
        if not valid_target(i,tj) or not valid_target(j,ti): continue
        dc,dl=local_delta(i,j)
        score=dc*60.0+dl
        if dc<0 and score<best_score:
            best_score=score; best=(i,j)
    if best is None: break
    i,j=best
    assign[i],assign[j]=assign[j],assign[i]

final_cross=len(cross_pairs())
mapping={fronts[i][0]:active[int(assign[i])]['id'] for i in range(len(fronts))}
assert len(mapping)==726 and len(set(mapping.values()))==726
assert not (set(mapping.values())&spare)
xy_len=[math.hypot(fronts[i][1]-active[int(assign[i])]['x'],fronts[i][2]-active[int(assign[i])]['y']) for i in range(len(fronts))]
sign_bad=sum(1 for i in range(len(fronts)) if not valid_target(i,int(assign[i])))

simple=json.dumps(mapping,ensure_ascii=False,separators=(',',':'))
pat=r'const OPTIMIZED_FRONT_MAPPING=.*?;\s*\nfunction applyOptimizedBackMapping'
rep='const OPTIMIZED_FRONT_MAPPING='+simple+';\nfunction applyOptimizedBackMapping'
new_s,nrep=re.subn(pat,rep,s,count=1,flags=re.S)
if nrep!=1: raise RuntimeError(f'front mapping block replace failed: {nrep}')
HTML.write_text(new_s,encoding='utf-8')
(ROOT/'optimized_front_to_middle.json').write_text(json.dumps(mapping,indent=2,ensure_ascii=False),encoding='utf-8')
print('front mapping:',len(mapping),'cross:',initial_cross,'->',final_cross)
print('unique active:',len(set(mapping.values())),'spare used:',len(set(mapping.values())&spare),'constraint bad:',sign_bad)
print('planar length avg/max mm:',round(float(np.mean(xy_len)),3),round(float(np.max(xy_len)),3))
