import sys

def opts(byte):
    # top bit: 0 = left, 1 = right
    # bottom bit: 0 = or, 1 = and
    if byte is None: return []
    return [
        byte | (byte << 1) & 0xff,
        byte & (byte << 1),
        byte | (byte >> 1) & 0xff,
        byte & (byte >> 1),
    ]

def pb12(data):
    data = iter(data)

    literals = bytearray()
    bits = 0
    control = 0
    prev = [None, None]
    gotta_end = False

    chunk = bytearray()
    while True:
        try:
            byte = next(data)
        except StopIteration:
            if bits == 0: break
            byte = 0
        chunk.append(byte)

        if byte in prev:
            bits += 2
            control <<= 1
            control |= 1
            control <<= 1
            if prev[1] == byte:
                control |= 1 # 10 = out[-2], 11 = out[-1]
        else:
            bits += 2
            control <<= 2
            options = opts(prev[1])
            if byte in options:
                # 01 = modify
                control |= 1

                bits += 2
                control <<= 2
                control |= options.index(byte)
            else:
                # 00 = literal
                literals.append(byte)
        prev = [prev[1], byte]
        if bits >= 8:
            outctl = control >> (bits - 8)
            assert outctl != 1 # that's the end byte
            yield bytes([outctl]) + literals
            bits -= 8
            control &= (1 << bits) - 1
            literals = bytearray()
            chunk = bytearray()
    yield b'\x01'

_, infile, outfile = sys.argv
with open(infile, 'rb') as f:
    data = f.read()
with open(outfile, 'wb') as f:
    f.writelines(pb12(data))
