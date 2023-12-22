#pragma once
#include "raylib.h"

//-----Entity Header File-----//

enum EntityKind
{
	NONE = 0, //Nothing should be EntityKind NONE
	PLAYER,
	ENEMY,
	BOMB,
	BLAST,
	BOX,
	WALL,
	//-----FOR TESTING SCREEN
	CIRCLE,
	AABB,
};

struct Entity
{ //Add Entity Properties Here
	EntityKind kind = NONE;
	Vector2 position = { 0,0 };
	Texture2D sprite;
	float size = 0.f; //Used for collision, mostly.
	int entityTimer = 0; // Used to calculate which sprite to use on a timer (60 = 1 sec), also used to determine when entities should trigger a timer event, or expire.
	int points = 0; //Used to determine how many points you get when they are destroyed (enemies, boxes)
	bool isAlive = true; // Erase when false
};

struct MovingEntity : Entity
{ // Player, Bombs, and maybe enemies and objects?
	Vector2 velocity = { 0,0 }; //Direction the entity is moving in
	bool facingRight = true; //If true, use sprite facing right, otherwise use sprite facing left.
};

struct StaticEntity : Entity
{ // Walls, destructible and non-destructible
	bool destructible = false; // If they are affected by bombs smashing into them.
};
