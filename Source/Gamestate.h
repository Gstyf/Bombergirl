#pragma once
#include "myMath.h"
#include "raymath.h"
#include "Constants.h"
#include "Entity.h"
#include <list>
#include <vector>

//-----Gamestate Header File-----//
//Runtime behaviour; which mode the game is in, and what measures are tracked in a level as well as loading levels and entities.

enum States
{
	TITLESCREEN = 0,
	GAMEPLAYSCREEN,
	SCORESCREEN,
	TESTSCREEN,
};

enum TestingMode
{
	CIRCLE_CIRCLE = 0,
	CIRCLE_AABB,
	CIRCLE_LINESEGMENT,
	AABB_AABB,
};

struct Gamestate
{
	//Gamestate should contain what is needed for logic and render,
	//So game mode, levels, entities, numbers, etc.
	States state = TITLESCREEN; // Used to tell the Update and Render function what to do
	TestingMode testMode = CIRCLE_CIRCLE; // Used to determing what shapes to display in test menu

	//-----StateLogic-----//
	int menuSelection = 0; //used to highlight menu buttons, and make selection.
	bool menuDisplayed = false; //used for any game state, essentially if the game is paused, or if a menu should be displayed

	int testModeByInt = 0; // Will iterate and change testMode with this number
	bool isCollidingInTestScreen = false; //if the collision ever returns true, then this is switched to true, and vice versa.

	int currentScore = 0; // If high enough, write into hiscore.
	int lives = 0; // When 0, Zoom to highscore screen.
	int timer = 0; // When 0, Zoom to highscore, and when win - add to score.
	int enemiesRemaining = 0; // When all enemies are dead, gameover (or win).
	int invinsibilityFrames = 60; // How many frames before player can take damage again. 
	int placeBombTimer = 0; //counts down to 0 on update (2 seconds), and bombs can only be placed when at 0.
	bool gameOver = false; // When true, display game over screen, and stop all other logic.
	bool newHiscore = false; // When true, insert the new hiscore in the correct place. 

	//----Player-----//
	MovingEntity player = {};
	MovingEntity* playerPointer = {};
	Vector2 bombDrop = {}; // The spot where bombs are dropped, placed adjacent from player in a cardinal direction.

	//-----RESOURCE STORAGE-----//
	//Arrays for moving entities
	std::list<MovingEntity> movingEnityList = {};
	std::vector<MovingEntity*> movingEntityPointer = {};

	//Arrays for static entities
	std::list<StaticEntity> staticEnityList = {};
	std::vector<StaticEntity*> staticEntityPointer = {};

	//-----ENTITY SPAWNING-----//
	std::vector<MovingEntity> futureMovingEnity = {};
	std::vector<MovingEntity*> futureMovingEnityPointer = {};
	std::vector<StaticEntity> futureStaticEnity = {};
	std::vector<StaticEntity*> futureStaticEnityPointer = {};
	
	MovingEntity SpawnMovingEntity(EntityKind k, Vector2 v); //Spawns an entity using EntityKind and Vector2 as argument, returns an Entity which is used in the AddEntity Functions
	void AddMovingEntity(MovingEntity e); // Adds an entity to the list of entites and a pointer to that entity in a vector.

	StaticEntity SpawnStaticEntity(EntityKind k, Vector2 v); //Spawns an entity using EntityKind and Vector2 as argument, returns an Entity which is used in the AddEntity Functions
	void AddStaticEntity(StaticEntity e); // Adds an entity to the list of entites and a pointer to that entity in a vector.
	
	void FutureEntity(EntityKind kind, Vector2 position); // Adds temporary entity information that will be retrieved at beginning of next update to spawn entities from the vectors above.
	void AddFutureEntities(); // At beginning of updates, searches the two lists of future entities and loads them into the entity lists, then clears the future Entity vectors.
	void RemoveDeadEntities(); // goes through the entity lists and removes the ones that return false on the boolean isAlive.

	//-----STATE / LEVEL LOADING-----//
	void Reset(); // Clears all entity lists from entities by marking them all isAlive = false
	void LoadLevelFromTextFile(); // Retrieves the Level.txt file, converts content to string, and loads level from that.
	void LoadTestLevel(TestingMode testmode);
};
