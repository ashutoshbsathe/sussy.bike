"""
splitted_keyframes = []
with open('keyframes_79_110.txt', 'r') as f:
    lines = f.read().splitlines()

for line in lines:
    splitted_keyframes.append([float(t) for t in line[:-1].split(' ')][0])
    
for s in splitted_keyframes:
    print(s+1371+8)
f.close()
"""
change_camera = 0
fname = './end'
with open(fname + '.txt', 'r') as f:
    lines = [line.strip() for line in f.readlines()]

for i in range(len(lines)):
    params = lines[i].split(' ')
    params[1] = str(1)
    params[2] = str(1)
    params[3] = str(1)
    params[4] = str(1)
    params[5] = str(change_camera)
    lines[i] = ' '.join(params) + ' '

with open(fname + f'_cam={change_camera}.txt', 'w') as f:
    f.write('\n'.join(lines))

sequence = [
    './startKeyframe_cam=1.txt',
    './startToFirst.txt',
    './obstacle1.txt',
    './obstacle2.txt',
    './obstacle3-1.txt',
    './obstacle3.txt',
    './obstacle4.txt',
    './end.txt'
]


