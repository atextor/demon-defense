Demon Defense
=============

Demon Defense is a tower defense game (currently in a very early state,
not really playable).

The idea was to use the characters from the original DOOM game and make
them walk along a track. You can place marines as towers around them
to fend them off.

Current state: Monsters walk along the track, no interactivity.

Building and running
--------------------

You need to have SDL installed. On Ubuntu, apt-get install libsdl1.2-dev.

Build the game with

 make

You need an original DOOM WAD file for the graphics. If you don't have
one, the Shareware WAD will work as well:
http://doomwiki.org/wiki/DOOM1.WAD
Place the file doom1.wad in the current directory, then run

 ./defense

Technical information
---------------------

I relied on the document "The Unofficial Doom Specs" by Matthew S Fell,
which can be found here: http://www.gamers.org/dhs/helpdocs/dmsp1666.html
(copy dmsp1666.txt in local directory).

The game extracts the lumps (graphics etc.) from the original WAD file.

