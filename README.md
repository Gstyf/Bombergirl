# Bombergirl
A small individual project made for a programming assignment with a focus on collision systems.

This was an assignment project for one of my programming courses. The goal was to design a game that utilised four types of collision systems in a 2D game space. The Circle vs Linesegment, Circle vs Circle, Circle vs Axis-aligned Bounding Box (AABB), and AABB vs AABB.

The game features a single level where the goal is to kill all enemies using placeable bombs that have a timed detonation, but that can be kicked into destructible obstacles and enemies to detonate them on impact. There is also a test room to check if the collisions work where you can cycle between objects of Line segments, Circles and AABBs to touch them together.

The game also features a basic implementation of a parallax screen in the main menu.

In the process of making this game I reinforce my understanding of refactoring and reusing code, particularly in my collison scripts.

If I were to revisit this code in the future, here is what I would like to improve:
1) I have a lot of indentation. I would like to refactor a lot of that code, especially where I have lengthy nested switch-cases and loops.
2) I have a bug where a player can get axis-locked with the collisionbox of objects in the game which can only be unstuck by walking along adjacent objects until the player is free. Annoying and potentially game breaking.
3) The code for sprite animation is funky. It works, but I want to redo it.
4) Redo the art. I am not a graphics designer, but this art is too placeholder-y, even for me.