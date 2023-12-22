#include "Update.h"
#include <iostream>

//-----UPDATE FUNTCIONS-----//
//Player Update During Gameplay
void PlayerUpdate(Gamestate* state)
{
	//Collision check with Arena Border
	UpdateEntityVelocity(state->playerPointer, PlayerMovementInput());
	if (state->playerPointer->position.x < 64)
	{
		if (CheckCircleVsLineSegment(state->playerPointer, LINESEGMENT_TOPLEFT, Vector2{ LINESEGMENT_TOPLEFT.x,LINESEGMENT_BOTTOMRIGHT.y }))
		{
			CircleLineSegmentCollision(state->playerPointer, LINESEGMENT_TOPLEFT, Vector2{ LINESEGMENT_TOPLEFT.x,LINESEGMENT_BOTTOMRIGHT.y });
		}
	}
	if (state->playerPointer->position.y < 64)
	{
		if (CheckCircleVsLineSegment(state->playerPointer, LINESEGMENT_TOPLEFT, Vector2{ LINESEGMENT_BOTTOMRIGHT.x,LINESEGMENT_TOPLEFT.y }))
		{
			CircleLineSegmentCollision(state->playerPointer, LINESEGMENT_TOPLEFT, Vector2{ LINESEGMENT_BOTTOMRIGHT.x,LINESEGMENT_TOPLEFT.y });
		}
	}
	if (state->playerPointer->position.x > SCREENWIDTH - 64)
	{
		if (CheckCircleVsLineSegment(state->playerPointer, LINESEGMENT_BOTTOMRIGHT, Vector2{ LINESEGMENT_BOTTOMRIGHT.x,LINESEGMENT_TOPLEFT.y }))
		{
			CircleLineSegmentCollision(state->playerPointer, LINESEGMENT_BOTTOMRIGHT, Vector2{ LINESEGMENT_BOTTOMRIGHT.x,LINESEGMENT_TOPLEFT.y });
		}
	}
	if (state->playerPointer->position.y > SCREENHEIGHT - 64)
	{
		if (CheckCircleVsLineSegment(state->playerPointer, LINESEGMENT_BOTTOMRIGHT, Vector2{ LINESEGMENT_TOPLEFT.x,LINESEGMENT_BOTTOMRIGHT.y }))
		{
			CircleLineSegmentCollision(state->playerPointer, LINESEGMENT_BOTTOMRIGHT, Vector2{ LINESEGMENT_TOPLEFT.x,LINESEGMENT_BOTTOMRIGHT.y });
		}
	}

	//Collision check with AABBs in Arena (Boxes)
	for (StaticEntity* staticEntity : state->staticEntityPointer)
	{
		if (CheckCircleVsAABB(state->playerPointer, staticEntity))
		{
			CircleAABBCollision(state->playerPointer, staticEntity);
		}
	}

	//Collision check with movingEnities; Enemies, Bombs, and Blasts
	for (MovingEntity* movingEnity : state->movingEntityPointer)
	{
		if (CheckCircleVsCircle(state->playerPointer, movingEnity))
		{
			if (movingEnity->kind == ENEMY && state->invinsibilityFrames <= 0)
			{
				state->lives--;
				state->invinsibilityFrames = 60;
			}
			if (movingEnity->kind == BOMB)
			{
				CircleCircleCollision(state->playerPointer, movingEnity);
			}
			if (movingEnity->kind == BLAST && state->invinsibilityFrames <= 0)
			{
				state->lives--;
				state->invinsibilityFrames = 60;
			}
		}
	}
	state->playerPointer->position += state->playerPointer->velocity;
	if (state->playerPointer->velocity.x >= 0) { state->player.facingRight = true; }
	else { state->player.facingRight = false; }

	//-----Player Input
	if (!Vector2Length(state->playerPointer->velocity) == 0)
	{
		state->bombDrop = UpdateBombDropSite(state->playerPointer);
	}
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
	{
		//state->AddMovingEntity(state->SpawnMovingEntity(BOMB, state->bombDrop));
		if (state->placeBombTimer == 0)
		{
			state->FutureEntity(BOMB, state->bombDrop);
			state->playerPointer->entityTimer = 61; //will animate placing the bomb
			state->placeBombTimer = BOMB_COOLDOWN; //Can place a bomb again in 2 seconds.
		}
	}
}

//MovingEntity Update During Gameplay (non-player)
void MovingEntityUpdate(Gamestate* state)
{
	for (MovingEntity* e : state->movingEntityPointer)
	{
		e->entityTimer++;
		//Collision checks with Arena Border
		if (e->position.x < 64)
		{
			if (CheckCircleVsLineSegment(e, LINESEGMENT_TOPLEFT, Vector2{ LINESEGMENT_TOPLEFT.x,LINESEGMENT_BOTTOMRIGHT.y }))
			{
				CircleLineSegmentCollision(e, LINESEGMENT_TOPLEFT, Vector2{ LINESEGMENT_TOPLEFT.x,LINESEGMENT_BOTTOMRIGHT.y });
			}
		}
		if (e->position.y < 64)
		{
			if (CheckCircleVsLineSegment(e, LINESEGMENT_TOPLEFT, Vector2{ LINESEGMENT_BOTTOMRIGHT.x,LINESEGMENT_TOPLEFT.y }))
			{
				CircleLineSegmentCollision(e, LINESEGMENT_TOPLEFT, Vector2{ LINESEGMENT_BOTTOMRIGHT.x,LINESEGMENT_TOPLEFT.y });
			}
		}
		if (e->position.x > SCREENWIDTH - 64)
		{
			if (CheckCircleVsLineSegment(e, LINESEGMENT_BOTTOMRIGHT, Vector2{ LINESEGMENT_BOTTOMRIGHT.x,LINESEGMENT_TOPLEFT.y }))
			{
				CircleLineSegmentCollision(e, LINESEGMENT_BOTTOMRIGHT, Vector2{ LINESEGMENT_BOTTOMRIGHT.x,LINESEGMENT_TOPLEFT.y });
			}
		}
		if (e->position.y > SCREENHEIGHT - 64)
		{
			if (CheckCircleVsLineSegment(e, LINESEGMENT_BOTTOMRIGHT, Vector2{ LINESEGMENT_TOPLEFT.x,LINESEGMENT_BOTTOMRIGHT.y }))
			{
				CircleLineSegmentCollision(e, LINESEGMENT_BOTTOMRIGHT, Vector2{ LINESEGMENT_TOPLEFT.x,LINESEGMENT_BOTTOMRIGHT.y });
			}
		}
		//Collision with AABBs (Boxes)
		for (StaticEntity* staticEntity : state->staticEntityPointer)
		{
			if (CheckCircleVsAABB(e, staticEntity))
			{
				CircleAABBCollision(e, staticEntity);
			}
		}
		//-----Enemy Specific Update
		if (e->kind == ENEMY)
		{
			if (Vector2Length(e->velocity) < 0.1f)
			{//If the enemy is standing still, make it lurch in the direciton it is facing.
				if (!e->facingRight)
				{
					e->velocity = ENEMY_LURCH * -1;
				}
				else
				{
					e->velocity = ENEMY_LURCH;
				}
			}
			e->velocity = Vector2Lerp(e->velocity, Vector2{ 0,0 }, LERP_T);
			e->position += e->velocity;
			if (e->velocity.x >= 0)
			{
				e->facingRight = true;
			}
			else
			{
				e->facingRight = false;
			}
			if (e->entityTimer > 60)
			{
				e->entityTimer = 0;
			}
		}
		//-----Bomb Specific Update
		if (e->kind == BOMB)
		{
			e->velocity = Vector2Lerp(e->velocity, Vector2{ 0,0 }, LERP_T);
			for (MovingEntity* otherEnity : state->movingEntityPointer)
			{
				if (otherEnity->kind == ENEMY)
				{
					if (CheckCircleVsCircle(e, otherEnity))
					{//Bomb explodes immediately on contact with enemies.
						e->isAlive = false;
					}
				}
			}
		}
		//-----Blast Specific Update
		if (e->kind == BLAST)
		{//Check if Blasts are touching any Boxes or Walls
			for (StaticEntity* se : state->staticEntityPointer)
			{//Don't do the calc if they aren't within range of each other.
				if (CheckClosestPoint(e->position, se->position) < 96)
				{//if they are within proximity, then we check for CircleVsAABB collision, and if true, resolve collision.
					if (CheckCircleVsAABB(e, se))
					{//If they are colliding, resolve collision.
						CircleAABBCollision(e, se);
					}
				}
			}
			for (MovingEntity* otherEnity : state->movingEntityPointer)
			{
				if (otherEnity->kind == BOMB && otherEnity->entityTimer < BOMB_FUSE)
				{ //If a Blast touches a Bomb, set the entityTimer of the Bomb to 270, so it explodes nearly immediately.
					if (CheckCircleVsCircle(e, otherEnity))
					{
						otherEnity->entityTimer = BOMB_FUSE;
					}
				}
				if (otherEnity->kind == ENEMY)
				{
					if (CheckCircleVsCircle(e, otherEnity))
					{
						otherEnity->isAlive = false;
					}
				}
			}
		}
		e->position += e->velocity;
	}
}

void ResetValues(Gamestate* state)
{//Resets the values of a level when you first start, or when you retry.
	state->gameOver = false;
	state->currentScore = 0;
	state->enemiesRemaining = 0;
	state->timer = PLAY_TIMER;
	state->lives = PLAY_LIVES;
}

//-----MAIN UPDATE LOOPS-----//

void TitleScreenUpdate(Gamestate* state)
{
	if (IsKeyPressed(KEY_BACKSPACE) && !state->menuDisplayed)
	{
		state->menuDisplayed = true;
		return;
	}
}

//GAMEPLAY LOOP
void GameplayUpdate(Gamestate* state)
{
	if (!state->gameOver)
	{
		//-----Player Collision Checks & Movement
		PlayerUpdate(state);

		//-----MovingEntity Collision Checks & Movement
		MovingEntityUpdate(state);

		//-----Upkeep & Score
		state->timer--;
		state->player.entityTimer++;
		if (state->placeBombTimer > 0)
		{
			state->placeBombTimer--;
		}
		if (state->invinsibilityFrames > 0)
		{
			state->invinsibilityFrames--;
		}
		for (MovingEntity* e : state->movingEntityPointer)
		{
			//Add the points for any not-alive entities to the current score
			if (!e->isAlive)
			{
				state->currentScore += e->points;
			}
			//Unalive any Bombs or Blasts that have reached the end of their timer lifespan.
			if ((e->kind == BOMB && e->entityTimer == BOMB_TIMER) || (e->kind == BLAST && e->entityTimer == BLAST_TIMER))
			{
				e->isAlive = false;
			}
		}
		for (StaticEntity* e : state->staticEntityPointer)
		{
			//Add the points for any not-alive entities to the current score
			if (!e->isAlive)
			{
				state->currentScore += e->points;
			}
		}

		//-----End of Update Clean-up & Gameover check
		for (MovingEntity* e : state->movingEntityPointer)
		{
			if (e->kind == BOMB && e->isAlive == false)
			{
				state->FutureEntity(BLAST, e->position);
			}
			if (e->kind == ENEMY && !e->isAlive)
			{
				state->enemiesRemaining--;
			}
		}
		state->RemoveDeadEntities();
		if (state->timer <= 0 || state->lives <= 0 || state->enemiesRemaining <= 0)
		{
			if (state->enemiesRemaining <= 0)
			{// When all enemies are dead, you get an extra point for each 60 frames left on the timer
				state->currentScore += (int)(state->timer / 60);
			}
			state->gameOver = true;
		}
	}
	if (IsKeyPressed(KEY_BACKSPACE) && !state->menuDisplayed)
	{
		state->menuDisplayed = true;
		return;
	}
}

void ScorescreenUpdate(Gamestate* state)
{
	//This if-statement checks to see if a new highscore has been set, and if so to remain on name-input until the player has written three characters, then move on to overwrite new highscore position with those three letters.
	if (newHiscore)
	{
		int key = GetKeyPressed();
		while (key > 0)
		{
			if ((key > 31) && (key < 126) && (inputCharacterCount < MAX_CHARACTERS))
			{
				inputName[inputCharacterCount] = (char)key;
				inputCharacterCount++;
			}
			key = GetKeyPressed();
		}

		if (IsKeyPressed(KEY_BACKSPACE))
		{
			inputCharacterCount--;
			if (inputCharacterCount < 0) inputCharacterCount = 0;
			inputName[inputCharacterCount] = '\0';
		}

		if (IsKeyPressed(KEY_ENTER))
		{
			if (inputName[2] == '\0' || inputName[2] == '\n')
			{
				std::cout << "Needs to be 3 characters!" << std::endl;
				return;
			}
			std::string input = "";
			for (char c : inputName)
			{
				input += c;
			}
			highScoreArrayNames[newHiscoreSlot] = input.c_str();
			newHiscore = false;
			newHiscoreSlot = 0;
			WriteScore();
		}
	}
	if (IsKeyPressed(KEY_BACKSPACE) && !state->menuDisplayed && !newHiscore)
	{
		state->menuDisplayed = true;
		return;
	}
}

void TestScreenUpdate(Gamestate* state)
{
	Vector2 mousePosition = GetMousePosition();
	state->isCollidingInTestScreen = false;
	switch (state->testMode)
	{
	case CIRCLE_CIRCLE:
		if (CheckCircleVsCircle(state->movingEntityPointer[0], state->movingEntityPointer[1]))
		{
			state->isCollidingInTestScreen = true;
		}
		break;
	case CIRCLE_AABB:
		if (CheckCircleVsAABB(state->movingEntityPointer[0], state->staticEntityPointer[0]))
		{
			state->isCollidingInTestScreen = true;
		}
		break;
	case CIRCLE_LINESEGMENT:
		if (CheckCircleVsLineSegment(state->movingEntityPointer[0], state->movingEntityPointer[1]->position, state->movingEntityPointer[2]->position))
		{
			state->isCollidingInTestScreen = true;
		}
		break;
	case AABB_AABB:
		if (CheckAABBvsAABB(state->staticEntityPointer[0], state->staticEntityPointer[1]))
		{
			state->isCollidingInTestScreen = true;
		}
		break;
	default:
		break;
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		switch (state->testMode)
		{
		case CIRCLE_CIRCLE:
			for (MovingEntity* e : state->movingEntityPointer)
				if (CheckClosestPoint(e->position, mousePosition) < e->size)
				{
					e->position = mousePosition;
					return;
				}
			break;
		case CIRCLE_AABB:
			for (MovingEntity* e : state->movingEntityPointer)
				if (CheckClosestPoint(e->position, mousePosition) < e->size)
				{
					e->position = mousePosition;
					return;
				}
			for (StaticEntity* e : state->staticEntityPointer)
				if (CheckPointInsideRectangle(mousePosition, e, { 0,0 }))
				{
					e->position = mousePosition - Vector2{ e->size / 2, e->size / 2 };
					return;
				}
			break;
		case CIRCLE_LINESEGMENT:
			for (MovingEntity* e : state->movingEntityPointer)
				if (CheckClosestPoint(e->position, mousePosition) < e->size)
				{
					e->position = mousePosition;
					return;
				}
			break;
		case AABB_AABB:
			for (StaticEntity* e : state->staticEntityPointer)
				if (CheckPointInsideRectangle(mousePosition, e, { 0,0 }))
				{
					e->position = mousePosition - Vector2{ e->size / 2, e->size / 2 };
					return;
				}
			break;
		default:
			break;
		}

	}
	bool switchMode = false;
	if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
	{
		if (state->testModeByInt > 0) { state->testModeByInt--; }
		else { state->testModeByInt = 3; }
		switchMode = true;
	}
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
	{
		if (state->testModeByInt < 3) { state->testModeByInt++; }
		else { state->testModeByInt = 0; }
		switchMode = true;
	}
	if (switchMode)
	{
		switch (state->testModeByInt)
		{
		case 0:
			state->Reset();
			state->LoadTestLevel(CIRCLE_CIRCLE);
			break;
		case 1:
			state->Reset();
			state->LoadTestLevel(CIRCLE_AABB);
			break;
		case 2:
			state->Reset();
			state->LoadTestLevel(CIRCLE_LINESEGMENT);
			break;
		case 3:
			state->Reset();
			state->LoadTestLevel(AABB_AABB);
			break;
		default:
			break;
		}
	}
	if (IsKeyPressed(KEY_BACKSPACE) && !state->menuDisplayed)
	{
		state->menuDisplayed = true;
		return;
	}
}



void MenuWindowUpdate(Gamestate* state)
{
	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
	{
		if (state->menuSelection == 0) { state->menuSelection++; }
		if (state->menuSelection > 1) { state->menuSelection--; }
	}
	if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
	{
		if (state->menuSelection == 0 || state->menuSelection < 3)
		{
			state->menuSelection++;
		}
	}
	if (IsKeyPressed(KEY_ENTER))
	{
		switch (state->menuSelection)
		{
		case 1:
			if (state->state == TITLESCREEN)
			{
				state->state = GAMEPLAYSCREEN;
				ResetValues(state);
				state->LoadLevelFromTextFile();
				state->bombDrop = UpdateBombDropSite(state->playerPointer);
			}
			else if (state->state == GAMEPLAYSCREEN && state->gameOver) //If it's a game-over
			{
				//move to score screen, check if new highscore.
				state->state = SCORESCREEN;
				newHiscoreSlot = UpdateHiscores(state->currentScore, highScoreArrayNames, highScoreArrayNumbers);
			}
			break;
		case 2:
			if (state->state == SCORESCREEN)
			{
				FactoryResetScore();
			}
			else if (state->state == TESTSCREEN)
			{
				state->Reset();
				state->LoadTestLevel(state->testMode);
			}
			if (state->state == TITLESCREEN)
			{
				state->state = SCORESCREEN;
			}
			if (state->state == GAMEPLAYSCREEN)
			{
				if (!state->gameOver)
				{
					//If it isn't GameOver, then this resigns you do the score screen
					state->state = SCORESCREEN;
				}
				else
				{
					//if its GameOver, reload the level
					state->Reset();
					ResetValues(state);
					state->LoadLevelFromTextFile();
					state->bombDrop = UpdateBombDropSite(state->playerPointer);
				}
			}
			break;
		case 3:
			if (state->state == TITLESCREEN)
			{
				//Opens test screen, so load in default stage.
				state->LoadTestLevel(state->testMode);
			}
			else
			{
				state->Reset();
				ResetCharacterInput();
				state->state = TITLESCREEN;
			}
			break;
		default:
			break;
		}
		state->menuSelection = 0;
		state->menuDisplayed = false;
	}
}

void Update(Gamestate* state)
{
	if (!state->menuDisplayed)
	{
		switch (state->state)
		{
		case TITLESCREEN:
			TitleScreenUpdate(state);
			break;
		case GAMEPLAYSCREEN:
			state->AddFutureEntities();
			GameplayUpdate(state);
			break;
		case SCORESCREEN:
			ScorescreenUpdate(state);
			break;
		case TESTSCREEN:
			state->AddFutureEntities();
			ScorescreenUpdate(state);
			TestScreenUpdate(state);
			break;
		default: //Should never get here.
			break;
		}
	}
	else
	{
		MenuWindowUpdate(state);
	}
}