# BoatLakeCgv
Uni of York CS 2003 CGV Assessment Solution

## About
This project is the result of my work for open assessment of Computer Graphics course (CGV) at the University of York in the year 2002 (my second year). The goal was to build a boat on the lake that should have been 'playable' (i.e. movable).

As the boat moves it leaves two trails of 'bubbles' - one for each engine. Simple array-based particle system was used. 

As far as I recall the requirements, behind the lake, there should be green hills and behind them - brown mountains (mine are slightly reddish - I like it so :)). I have used Perlin noise to create the landscape. 

There were extra points for the boat shape, so I decided to use Bezier patches for that. I have created patch editor so one can modify boat shape 'in-game'. Boat views and editor can be accessed using alternative-click activated menu.

## how to build
* get(/build) *premake5* and *Glut* (or perhaps *freeglut*)
* update Glut directories in _boat_lake_premake5.lua_
* run regenerate.sh (or regenerate.cmd, depending on the platform).
* open VS/Xcode/gmake projects in build/<configuration>/ and build the project
* run executable at "build/products/BoatLake" to see the boat on the lake!


Old compilation instructions also found in src/README
