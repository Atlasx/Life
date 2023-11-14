# Build Instructions

This project makes use of Premake to build the project solutions:
- Install [premake](https://premake.github.io/) and add it to your cmd/bash PATH
- Run `premake5 vs2022` substituting with your visual studio or other IDE. Run `premake5 --help` for more info.
- Build Debug or Release configuration within your IDE and run

## Notes

Some time estimates here:

50x50 grid of cells: 2500 cells

each cell needs to read 9 neighboring cells + set result 
2500 * 10 = 25000 cell reads

50,000 iterations of the game
25000 * 50000 = 1250000000 = 1.25 billion

CPU hertz on test machine: 3.7 Ghz
single threaded timing: ~2500ms

3.7cycle/s * 2.5s / 1.25operations = 7.4 avg cycles per operation

With dirty rect speed up:
~1200ms

3.7 * 1.2 / 1.25 = 3.5 cycles per operation

This improvement is highly data dependent and will not help if there are oscillators
in opposing corners of the grid. My next improvement would be to generate multiple
dirtyRects which reactively group dirtied cells into more performant rects.

Functionality-wise, this could also use a chunked grid system to allow for as-needed
expansion of the game world. Additional rule-sets and types of cells would also be
good areas of extension.

Wasn't able to get yaml library linking properly for this build so I'm just compiling in
some examples here to test with.