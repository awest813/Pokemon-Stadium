with open('baseroms/us/baserom.n64', 'rb') as f:
    d = bytearray(f.read())

for i in range(0, len(d), 2):
    tmp = d[i]
    d[i] = d[i+1]
    d[i+1] = tmp

with open('baseroms/us/baserom.z64', 'wb') as f:
    f.write(d)
