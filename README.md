## Fast xyz trajectory reader

`cxyz` is a fast xyz trajectory reader. Written in c++, and interfaced with python using Cython, `cxyz` theoretically out performs it's python counterpart developed by the `MDAnalysis` team. 

`cxyz` is in its early development state now, where its `XYZReader` class can read xyz data of whichever frame to a `numpy.ndarray` object. The logic of `XYZReader` is highly inspired by its python counterpart developed `MDAnalysis` team. First, `XYZReader` reads xyz trajectory file line by line, meanwhile recording the offset of each frame's start. (**offset for the input file stream** , unsinged long long type) Later, `XYZReader` is capable to get access to each frame start from these offsets easily.

## install

``` shell
git clone git@github.com:ruihao69/cxyz.git
pip install -e .
```

## Example

demo:
```
import numpy as np
from cxyz import XYZReader

# create an instance
xyz = XYZReader("tio2-water.xyz")

# read frame positions (offsets relative to the begining of fstream)
# after running, three members in xyz is initialized, namely
# n_atoms      n_frames       and f_pos
# # of atom    # of frames        offsets
xyz.run_get_fpos()

print("The trajectory has %d atoms" %xyz.n_atoms)
print("The trajectory has %d frames" %xyz.n_frames)
print("The last five offsets for this trajectory:\n", xyz.f_pos[-5:])

# you can invoke whichever xyz data by runing
frame_idx = 123
xyz.get_frame_coords(frame_idx)
print("The coordinates for the first five atoms for frame #123:\n", xyz.coords[:5])
```

outputs:
```
The trajectory has 990 atoms
The trajectory has 4001 frames
The last five offsets for this trajectory:
 [154396336 154434974 154473612 154512250 154550888]
The coordinates for the first five atoms for frame #123:
 [[ 0.872259 11.494716  1.818022]
 [ 0.870223  1.108388  2.991948]
 [ 4.215645 11.411075  3.037844]
 [ 5.394652  1.144893  2.820568]
 [ 2.90701   0.912237  2.825836]]
```