splitted_keyframes = []
with open('keyframes_79_110.txt', 'r') as f:
    lines = f.read().splitlines()

for line in lines:
    splitted_keyframes.append([float(t) for t in line[:-1].split(' ')][0])
    
for s in splitted_keyframes:
    print(s+1371+8)
f.close()