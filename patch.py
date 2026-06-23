from pathlib import Path
import struct

path = Path("catch.bin")

delay_iterations = 200_000
delay_literal_offset = 0x54

data = bytearray(path.read_bytes())

# The code compares against iterations - 1
value = delay_iterations - 1
data[delay_literal_offset:delay_literal_offset + 4] = struct.pack("<I", value)

# Trim trailing erased flash bytes
while data and data[-1] == 0xFF:
    data.pop()

path.write_bytes(data)

print(f"patched delay constant to {value} = 0x{value:08x}")
print(f"new size: {len(data)} bytes")
