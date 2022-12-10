from binascii import a2b_hex as a2b
import hashlib

key = [
    0x66, 0x13, 0x98, 0xaf, 0x49, 0x40, 0xa1, 0xca, 0xd4, 0x43, 0xd7, 0xe7, 0x90, 0xb6, 0x12, 0xce,
    0xb0, 0x62, 0xba, 0xe5, 0x73, 0xc5, 0x0b, 0x37, 0xda, 0xe8, 0x0e, 0xc9, 0x2f, 0xe6, 0xf2, 0x68,
    0xc8, 0xd3, 0xb9, 0x6e, 0xaa, 0xfe, 0xf0, 0x8a, 0xa3, 0xe1, 0x5c, 0x78, 0xe0, 0x92, 0x5d, 0x1a,
    0xfd, 0x08, 0x35, 0x97, 0x0c, 0xd8, 0x7e, 0xe2, 0x1f, 0xae, 0xa8, 0x36, 0xdf, 0x63, 0x06, 0xe9,
    0xb5, 0xac, 0x2b, 0x6b, 0x93, 0x3b, 0x41, 0x5e, 0x07, 0xb1, 0xf1, 0xee, 0x9d, 0x29, 0x61, 0xb7,
    0x16, 0xf5, 0xcc, 0x04, 0x4b, 0x33, 0x15, 0xdc, 0x79, 0xd2, 0x59, 0xdd, 0xed, 0xf3, 0x6c, 0x2a,
    0x99, 0x22, 0xf6, 0x09, 0x19, 0x65, 0xa2, 0x6f, 0x03, 0x14, 0xb8, 0x18, 0xcf, 0x74, 0x57, 0x24,
    0x5b, 0x26, 0x94, 0xfb, 0x8b, 0x60, 0x4a, 0x84, 0xdb, 0x1d, 0xec, 0x1e, 0xff, 0x3e, 0xcb, 0xc6,
    0x91, 0x9c, 0x21, 0x47, 0x7b, 0x87, 0xb2, 0x67, 0xd1, 0x55, 0x83, 0x56, 0xbd, 0x25, 0xef, 0x9e,
    0xde, 0x39, 0xb4, 0x76, 0xab, 0xa7, 0xeb, 0x52, 0xd5, 0x34, 0x31, 0xbf, 0x95, 0xf4, 0x0f, 0x3f,
    0x4e, 0x9a, 0xad, 0xbe, 0x4f, 0xb3, 0x8e, 0x71, 0x53, 0xea, 0xc2, 0xf7, 0x77, 0x70, 0x86, 0x9b,
    0x7f, 0x9f, 0xa5, 0x3d, 0x2e, 0xe3, 0x10, 0x42, 0x50, 0xd9, 0x81, 0xa6, 0x64, 0x3a, 0x27, 0xbb,
    0xd6, 0x7c, 0x02, 0x48, 0x80, 0x2c, 0x05, 0x89, 0xa9, 0xc0, 0xfa, 0xc4, 0x1c, 0x30, 0x85, 0x58,
    0xfc, 0x8d, 0xe4, 0x6a, 0x2d, 0x00, 0x96, 0x82, 0x6d, 0x8c, 0xd0, 0xa0, 0x1b, 0x54, 0x4c, 0x4d,
    0xcd, 0xc7, 0x11, 0x72, 0xf9, 0x7a, 0x3c, 0x45, 0x7d, 0x8f, 0x17, 0x88, 0x51, 0x28, 0x5a, 0xbc,
    0xa4, 0x0d, 0x75, 0xc1, 0x5f, 0x01, 0x44, 0xf8, 0x69, 0x38, 0x0a, 0x32, 0x46, 0xc3, 0x20, 0x23]

def xor(p, k):
    return ((p ^ k) + 17) & 0xff

def encrypt(data, move):
    data = list(data)
    datalen = len(data)
    index = 0
    for x in range(datalen):
        if index == 256:
            index = 0
        data[x] = hex(xor(data[x], key[index])).replace("0x", "")
        if len(data[x]) == 1:
            data[x] = "0" + data[x]
        index += 1
    return Hash(a2b("".join(data)))

def Hash(data):
    a = hashlib.sha256()
    a.update(data)
    return a.hexdigest()

if __name__ == '__main__':
    with open("data.txt", "rb") as f:
        a = f.read()
    b = encrypt(a, 2)
    print(b)