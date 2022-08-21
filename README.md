417 Group Project


Project is configured to compile on Linux using g++.

The project requires 4 database files totalling just over 1GB to run. I tried to upload the files to git LFS, but after many hours of troubleshooting I could not get it to work. I contacted the helpdesk for advice but they did not respond. The only option I can think of is to link a google drive folder with the databases. 

This is the link: https://drive.google.com/drive/folders/1phuwpdroP99XY_4mIuJraz4aWBAo2ALX?usp=sharing

The project is formatted to have the 4 files (with the names they have on google drive) in a subdirecty of the source code called "Databases". If desired the filepaths to the database files can be modified with the PDB class (PDB.cpp).

To run:

run command 'make'
run command './solver "<scramble>"'

where scramble is a string of moves.
For example, "D L B2 R2 B' R2 U2 L2 B2 U2 B D2 L2 R' U B2 L R' B2 F'"

The output will be a (shortest possible length) list of moves to return the scrambled cube to the solved state.

Typical scrambles may take hours to solve, so in the interest of time I recommend testing scrambles that are known to have short solutions, or just short scrambles. If you input a short scramble, the output may be the scramble in reverse, but I assure you the solver has no knowledge of the moves applied to the starting cube.

solutions up to 14 moves are solved virtually instantly, and 16 move solutions can usually be solved in a matter of minutes. 

There is no built in timing, so run with Linux "time" command to get timing.
