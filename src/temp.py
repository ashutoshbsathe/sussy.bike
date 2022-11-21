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
"""
"""
with open('keyframes_animesh.txt', 'r') as f:
    lines = [line.strip() for line in f.readlines()]

add_delay = [0 for _ in lines]
add_delay[70] = 25
add_delay[71] = 25
add_delay[72] = 10
add_delay[73] = 10
add_delay[74] = 10
orig_keys = [int(line.split(' ')[0]) for line in lines]
diff = [1]
for i in range(1, len(orig_keys)):
    diff.append(orig_keys[i] - orig_keys[i-1] + add_delay[i])
new_keys = [1]
for i in range(1, len(diff)):
    new_keys.append(new_keys[-1] + diff[i])
print(orig_keys)
print(diff)
print(new_keys)

for i in range(len(lines)):
    params = lines[i].split(' ')
    params[0] = str(new_keys[i])
    params[1] = str(1)
    params[2] = str(1)
    params[3] = str(1)
    params[4] = str(1)
    lines[i] = ' '.join(params) + ' '

with open('keyframes.txt', 'w') as f:
    f.write('\n'.join(lines))
"""

"""
sequences = [
    './init_camera.txt',
    './keyframes_good.txt',
    './end_camera.txt'
]

keyframes = []
offset = 0
for fname in sequences:
    with open(fname, 'r') as f:
        lines = [line.strip() for line in f.readlines()]
    for i in range(len(lines)):
        params = lines[i].split(' ')
        params[0] = str(int(params[0]) + offset)
        lines[i] = ' '.join(params) + ' '
    keyframes.extend(lines)
    last_keyframe = keyframes[-1]
    offset = int(last_keyframe.split(' ')[0]) + 1
with open('keyframes.txt', 'w') as f:
    f.write('\n'.join(keyframes))
"""

new_length = 4000
with open('./keyframes_with_everything.txt', 'r') as f:
    lines = [line.strip() for line in f.readlines()]

orig_keys = [int(line.split(' ')[0]) for line in lines]
diff = [1]
for i in range(1, len(orig_keys)):
    diff.append(int((orig_keys[i] - orig_keys[i-1]) * new_length / orig_keys[-1]))
new_keys = [1]
for i in range(1, len(diff)):
    new_keys.append(new_keys[-1] + diff[i])
for i in range(len(lines)):
    params = lines[i].split(' ')
    params[0] = str(new_keys[i])
    lines[i] = ' '.join(params) + ' '

with open('./keyframes.txt', 'w') as f:
    f.write('\n'.join(lines))
