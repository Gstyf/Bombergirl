#include "Gamestate.h"
#include "ResourceManager.h"

constexpr const char* LevelFile = "./Resources/Levels.txt";

//-----Entity Spawning
MovingEntity Gamestate::SpawnMovingEntity(EntityKind kind, Vector2 position)
{ //This method spawns an entity using kind and vector2 as argument, returns a tEntity which is used in the AddEntity Functions
	MovingEntity tMovingEntity;
	switch (kind)
	{
	case PLAYER:
		tMovingEntity.kind = kind;
		tMovingEntity.position = { position.x + 16.f, position.y + 16.f }; // Offset slightly.
		tMovingEntity.sprite = characterSprites[0];
		tMovingEntity.size = 16.f;
		break;	
	case ENEMY:
		tMovingEntity.kind = kind;
		tMovingEntity.position = { position.x + 16.f, position.y + 16.f }; // Offset slightly.
		tMovingEntity.sprite = enemySprites[0];
		tMovingEntity.size = 16.f;
		tMovingEntity.points = 100;
		enemiesRemaining++; //Count upwards how many enemies are in the level.
		break;
	case BOMB:
		tMovingEntity.kind = kind;
		tMovingEntity.position = { position.x, position.y };
		tMovingEntity.sprite = bombSprites[0];
		tMovingEntity.size = 16.f;
		break;
	case BLAST:
		tMovingEntity.kind = kind;
		tMovingEntity.position = { position.x, position.y };
		tMovingEntity.size = 8.f;
		break;
	case CIRCLE:
		tMovingEntity.kind = kind;
		tMovingEntity.position = { position.x + 32.f, position.y + 32.f };
		tMovingEntity.size = 32.f;
		break;
	default:
		break;
	}
	return tMovingEntity;
}

void Gamestate::AddMovingEntity(MovingEntity e)
{ // Adds an entity to the list of entites and a pointer to that entity in a vector.
	if (e.kind != BLAST)
	{
		movingEnityList.push_back(e);
		movingEntityPointer.push_back(&movingEnityList.back());
	}
	else if (e.kind == BLAST)
	{
		//Blast Up
		e.velocity = Vector2{ 0, -1.f } * VELOCITYMULTIPLIER;
		e.position += Vector2{ 0, -1.f } * BLAST_SPAWNDISTANCE;
		e.sprite = blastsSprites[0];
		movingEnityList.push_back(e);
		movingEntityPointer.push_back(&movingEnityList.back());
		//Blast Right
		e.velocity = Vector2{ 1.f, 0 } * VELOCITYMULTIPLIER;
		e.position += Vector2{ 1.f, 0 } * BLAST_SPAWNDISTANCE;
		e.sprite = blastsSprites[1];
		movingEnityList.push_back(e);
		movingEntityPointer.push_back(&movingEnityList.back());
		//Blast Down
		e.velocity = Vector2{ 0, 1.f } *VELOCITYMULTIPLIER;
		e.position += Vector2{ 0, 1.f } * BLAST_SPAWNDISTANCE;
		e.sprite = blastsSprites[2];
		movingEnityList.push_back(e);
		movingEntityPointer.push_back(&movingEnityList.back());
		//Blast Left
		e.velocity = Vector2{ -1.f, 0 } *VELOCITYMULTIPLIER;
		e.position += Vector2{ -1.f, 0 } * BLAST_SPAWNDISTANCE;
		e.sprite = blastsSprites[3];
		movingEnityList.push_back(e);
		movingEntityPointer.push_back(&movingEnityList.back());
	}
}

StaticEntity Gamestate::SpawnStaticEntity(EntityKind k, Vector2 v)
{ //This method spawns an entity using kind and vector2 as argument, returns a tEntity which is used in the AddEntity Functions
	StaticEntity tStaticEntity;
	switch (k)
	{
	case BOX:
		tStaticEntity.kind = k;
		tStaticEntity.position = v;
		tStaticEntity.sprite = boxSprites[0];
		tStaticEntity.size = 32.f;
		tStaticEntity.destructible = true;
		tStaticEntity.points = 35;
		break;
	case WALL:
		tStaticEntity.kind = k;
		tStaticEntity.position = v;
		tStaticEntity.sprite = boxSprites[1];
		tStaticEntity.size = 32.f;
		tStaticEntity.destructible = false;
		break;
	case AABB:
		tStaticEntity.kind = k;
		tStaticEntity.position = v;
		tStaticEntity.size = 64.f;
		break;
	default:
		break;
	}
	return tStaticEntity;
}

void Gamestate::AddStaticEntity(StaticEntity e)
{ // Adds an entity to the list of entites and a pointer to that entity in a vector.
	staticEnityList.push_back(e);
	staticEntityPointer.push_back(&staticEnityList.back());
}

//-----Future Entity Spawning
void Gamestate::FutureEntity(EntityKind kind, Vector2 position)
{
	MovingEntity tmEntity; // Temporary MovingEntity
	StaticEntity tsEntity; // Temporary StaticEntity
	switch (kind)
	{
	case PLAYER:
		tmEntity.kind = kind;
		tmEntity.position = position;
		futureMovingEnity.push_back(tmEntity);
		futureMovingEnityPointer.push_back(&futureMovingEnity.back());
		break;
	case BOMB:
		tmEntity.kind = kind;
		tmEntity.position = position;
		futureMovingEnity.push_back(tmEntity);
		futureMovingEnityPointer.push_back(&futureMovingEnity.back());
		break;
	case BOX:
		tsEntity.kind = kind;
		tsEntity.position = position;
		futureStaticEnity.push_back(tsEntity);
		futureStaticEnityPointer.push_back(&futureStaticEnity.back());
		break;
	case WALL:
		tsEntity.kind = kind;
		tsEntity.position = position;
		futureStaticEnity.push_back(tsEntity);
		futureStaticEnityPointer.push_back(&futureStaticEnity.back());
		break;
	case BLAST:
		tmEntity.kind = kind;
		tmEntity.position = position;
		futureMovingEnity.push_back(tmEntity);
		futureMovingEnityPointer.push_back(&futureMovingEnity.back());
		break;
	default:
		break;
	}
}

void Gamestate::AddFutureEntities()
{
	if (futureMovingEnity.size() > 0)
	{
		for (MovingEntity* e : futureMovingEnityPointer)
		{
			//SpawnMovingEntity(e.kind, e.position);
			AddMovingEntity(SpawnMovingEntity(e->kind, e->position));
		}
		futureMovingEnity.clear();
		futureMovingEnityPointer.clear();
	}
	if (futureStaticEnity.size() > 0)
	{
		for (StaticEntity* e : futureStaticEnityPointer)
		{
			//SpawnStaticEntity(e.kind, e.position);
			AddStaticEntity(SpawnStaticEntity(e->kind, e->position));
		}
		futureStaticEnity.clear();
		futureStaticEnityPointer.clear();
	}
}

//-----Entity Clearing

void Gamestate::RemoveDeadEntities()
{
	// I've used this method thrice before. It looks really ugly, but it is the way we were shown all the way back in programming 1. This time, however, I am using it twice in the same function for two separate pairs of arrays.
	//Clear out dead MovingEntities
	auto deadMovingEntity = std::remove_if(movingEntityPointer.begin(), movingEntityPointer.end(), [](const Entity* e) -> bool { return !e->isAlive; });
	movingEntityPointer.erase(deadMovingEntity, movingEntityPointer.end());
	movingEnityList.remove_if([](const Entity& e) -> bool { return !e.isAlive; });
	//Clear out dead StaticEntities
	auto deadStaticEntity = std::remove_if(staticEntityPointer.begin(), staticEntityPointer.end(), [](const Entity* e) -> bool { return !e->isAlive; });
	staticEntityPointer.erase(deadStaticEntity, staticEntityPointer.end());
	staticEnityList.remove_if([](const Entity& e) -> bool { return !e.isAlive; });
}

void Gamestate::Reset()
{
	for (MovingEntity* e : movingEntityPointer)
	{
		e->isAlive = false;
	}
	for (StaticEntity* e : staticEntityPointer)
	{
		e->isAlive = false;
	}
	RemoveDeadEntities();
}

//-----Level Loading
void Gamestate::LoadLevelFromTextFile()
{
	//-----This runs before the textfile is read-----//
	bool readChar = false;
	MovingEntity tMovingEntity;
	StaticEntity tStaticentity;

	//-----Starts reading level.txt from here-----//
	std::string level_text_file = LoadFileText(LevelFile);
	int x = 0; int y = 0;
	for (char& l : level_text_file)
	{
		if (!readChar)
		{
			if (l == '=')
			{
				readChar = true; // Start reading level.txt characters
				y = -1;
			}
		}
		else
		{
			switch (l)
			{
				//-----Player (not added to an array)
			case 'P':
				tMovingEntity.kind = PLAYER;
				tMovingEntity.position = { (32.f * x) + 16, (32.f * y) + 16 }; // Offset slightly.
				tMovingEntity.sprite = characterSprites[0];
				tMovingEntity.size = 16;
				player = tMovingEntity;
				playerPointer = &player;
				x++;
				break;
				//-----MovingEntity-----//
			case 'E':
				AddMovingEntity(SpawnMovingEntity(ENEMY, Vector2{ 32.f * x, 32.f * y }));
				x++;
				break;
				//-----StaticEntity-----/
			case 'O': //Box (Destructible)
				AddStaticEntity(SpawnStaticEntity(BOX, Vector2{ (32.f * x), (32.f * y) }));
				x++;
				break;
			case 'W': //Wall (Indestructible)
				Vector2{ (32.f * x), (32.f * y) };
				AddStaticEntity(SpawnStaticEntity(WALL, Vector2{ (32.f * x), (32.f * y) }));
				x++;
				break;
			case '\n': // New Line
				y++; x = 0;
				break;
			case '*': // Stop reading the level.txt characters. Used so I can write descriptions in ´file
				readChar = false;
				break;
			default: //Empty Space
				x++;
				break;
			}
		}
	}
}

void Gamestate::LoadTestLevel(TestingMode tMode)
{
	state = TESTSCREEN;
	switch (tMode)
	{
	case CIRCLE_CIRCLE:
		testMode = tMode;
		AddMovingEntity(SpawnMovingEntity(CIRCLE, Vector2{ SCREENWIDTH_TENTH * 3, SCREENHEIGHT_TENTH * 3 }));
		AddMovingEntity(SpawnMovingEntity(CIRCLE, Vector2{ SCREENWIDTH_TENTH * 6, SCREENHEIGHT_TENTH * 3 }));
		break;
	case CIRCLE_AABB:
		testMode = tMode;
		AddMovingEntity(SpawnMovingEntity(CIRCLE, Vector2{ SCREENWIDTH_TENTH * 3, SCREENHEIGHT_TENTH * 3 }));
		AddStaticEntity(SpawnStaticEntity(AABB, Vector2{ SCREENWIDTH_TENTH * 6, SCREENHEIGHT_TENTH * 3 }));
		break;
	case CIRCLE_LINESEGMENT:
		testMode = tMode;
		AddMovingEntity(SpawnMovingEntity(CIRCLE, Vector2{ SCREENWIDTH_TENTH * 3, SCREENHEIGHT_TENTH * 3 }));
		AddMovingEntity(SpawnMovingEntity(CIRCLE, Vector2{ SCREENWIDTH_TENTH * 5, SCREENHEIGHT_TENTH * 2 }));
		AddMovingEntity(SpawnMovingEntity(CIRCLE, Vector2{ SCREENWIDTH_TENTH * 7, SCREENHEIGHT_TENTH * 4 }));
		break;
	case AABB_AABB:
		testMode = tMode;
		AddStaticEntity(SpawnStaticEntity(AABB, Vector2{ SCREENWIDTH_TENTH * 3, SCREENHEIGHT_TENTH * 3 }));
		AddStaticEntity(SpawnStaticEntity(AABB, Vector2{ SCREENWIDTH_TENTH * 6, SCREENHEIGHT_TENTH * 3 }));
		break;
	default:
		break;
	}

}
