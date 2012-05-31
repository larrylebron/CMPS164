CMPS 164 Game Engines -- Code Refactoring and Change Log
Larry LeBron -- llebron@soe.ucsc.edu
Kwong Fai Jonathan Lew -- klew3@ucsc.edu

For this submission, we significantly refactored our engine, and implemented some changes and additions.

Refactoring:

-PhysicsManager and RenderManager classes are now purely utilities to be used by simulated and drawable objects
-Base GameObject class contains int ID and object normal
-Drawable base class/interface forces implementation of a draw function, and stores a color for all drawable objects. Only
	this class holds a reference to the renderManager.
-Simulated base class/interface forces implementation of a doSimulation function, and stores a velocity, timer and other
	essential physics related members. This is also the only class to hold a reference to the physicsManager.
-Plane base class contains distance, normal and vertex data. Tile and wall inherit from this, which allows polymorphism in the tile's list of edge bounds.
-Smart pointer memory management system is functioning, and releases memory allocated to level and gameObjects when references no longer point to them.



Additions:

-Physics Manager can calculate sphere-plane and point-plane collision data, as well as intersection point and time
-Simulated ball now accepts applications of force, rolls onto new tiles, bounces off walls, and enters the cup upon collision
-Ball checks one frame ahead for collisions and updates velocity in the following frame as necessary
-Tiles store friction data, which slows the ball as it rolls
-Gravity causes the ball to roll down hills
-The main game loop now allows for user input to impel the ball
-Fixed camera bugs from previous submission
-Updated loader complete with new logging
-Implemented b and n for browsing through levels
-Implemented HUD