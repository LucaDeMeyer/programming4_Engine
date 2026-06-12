import sys
import struct
import os

def bake_level(csv_path, bin_path):
    if not os.path.exists(csv_path):
        print(f"Error: Could not find {csv_path}")
        sys.exit(1)

    grid = []
    cols = 0
    rows = 0

   
    with open(csv_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line: continue
            
            cells = line.split(',')
            grid.extend([int(c) for c in cells])
            cols = len(cells)
            rows += 1

  
    with open(bin_path, 'wb') as f:
       
        f.write(struct.pack('i', cols))
        f.write(struct.pack('i', rows))
        

        for tile in grid:
            f.write(struct.pack('i', tile))

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: BakeLevel.py <input.csv> <output.bin>")
        sys.exit(1)
        
    bake_level(sys.argv[1], sys.argv[2])