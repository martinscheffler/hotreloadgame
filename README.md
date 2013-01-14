A minimal game engine/framework capable of hot-loading game code written in C.
Code is mostly from this blog post:
http://timothylottes.blogspot.de/2013/01/run-time-binary-reload-detailed.html

I also added some hello world code for OpenGL 3.0. The X window is kept open between reloads.

Tested on SUSE 12.2.

Usage: 
* First compile the hotloader with ./buildhotloader
* Then start the compile process with ./recompile
* Finally start the game with ./hotloader.
  A simple 3d scene with a bright background color should appear.
  Try changing some code in game.c, for example the clear color. As soon as you
  save, the background color in the running game should change.
