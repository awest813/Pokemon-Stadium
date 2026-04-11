import hashlib

with open('baseroms/us/baserom.n64', 'rb') as f:
    data = bytearray(f.read())

# Byte swap (v64 -> z64)
# Pairs are swapped: 0123 -> 1032
for i in range(0, len(data), 2):
    data[i], data[i+1] = data[i+1], data[i]

sha1 = hashlib.sha1(data).hexdigest()
print(f"Swapped SHA1: {sha1}")
