#include "Render.h"



//-----RENDERING NUMERATORS-----//
//-----MainMenu
float scrollingBGfore = 0.f;
float scrollingBGmid = 0.f;
float scrollingBGback = 0.f;

int textFlashing = 0;


//-----COLLECTED DRAW FUNCTIONS-----//
void DrawEntityPlayer(Gamestate* state)
{
	DrawTextureV(state->player.sprite, Vector2(state->player.position.x - 16, state->player.position.y - 16), WHITE);
	if (state->player.entityTimer < 30)
	{
		if (state->player.facingRight) state->player.sprite = characterSprites[1];
		else state->player.sprite = characterSprites[5];
	}
	else if (state->player.entityTimer >= 30 && state->player.entityTimer <= 60)
	{
		if (state->player.facingRight) state->player.sprite = characterSprites[0];
		else state->player.sprite = characterSprites[6];
		{
			if (state->player.entityTimer == 60) state->player.entityTimer = 0;
		}
	}
	else
	{
		if (state->playerPointer->facingRight)
		{
			if (state->playerPointer->entityTimer == 61)
			{
				state->playerPointer->sprite = characterSprites[2];
			}
			if (state->playerPointer->entityTimer == 65)
			{
				state->playerPointer->sprite = characterSprites[3];
			}
			if (state->playerPointer->entityTimer == 69)
			{
				state->playerPointer->sprite = characterSprites[4];
			}
			if (state->playerPointer->entityTimer == 73)
			{
				state->playerPointer->entityTimer = 0;
			}
		}
		else
		{
			if (state->playerPointer->entityTimer == 61)
			{
				state->playerPointer->sprite = characterSprites[7];
			}
			if (state->playerPointer->entityTimer == 65)
			{
				state->playerPointer->sprite = characterSprites[8];
			}
			if (state->playerPointer->entityTimer == 69)
			{
				state->playerPointer->sprite = characterSprites[9];
			}
			if (state->playerPointer->entityTimer == 73)
			{
				state->playerPointer->entityTimer = 0;
			}
		}
	}
}

void DrawEntitiesNonPlayers(Gamestate* state)
{
	if (state->movingEntityPointer.size() != 0)
	{
		for (MovingEntity* e : state->movingEntityPointer)
		{
			switch (e->kind)
			{
			case ENEMY:
				DrawTextureV(e->sprite, e->position - Vector2{ 16,16 }, WHITE);
				if (e->entityTimer == 0)
				{
					if (!e->facingRight)
					{
						e->sprite = enemySprites[0];
					}
					else
					{
						e->sprite = enemySprites[2];
					}
				}
				if (e->entityTimer == 30)
				{
					if (!e->facingRight)
					{
						e->sprite = enemySprites[1];
					}
					else
					{
						e->sprite = enemySprites[3];
					}
				}
				break;
			case BOMB: // Bombs will explode after 5 seconds, and then have isAlive switche to false, so they get deleted.
				DrawTextureV(e->sprite, e->position - Vector2{ 16,16 }, WHITE);
				if (Vector2Length(e->velocity) < 1.f)
				{//The sprire that is shown is a modulus function of the entity timer. This makes it flicker.
					if (e->entityTimer < 180)
					{
						if (e->entityTimer % 15 == 0)
						{
							e->sprite = bombSprites[1];
						}
						if (e->entityTimer % 30 == 0)
						{
							e->sprite = bombSprites[0];
						}
					}
					else if (e->entityTimer >= 180 && e->entityTimer < 300)
					{//Here it will flicker faster between angrier sprites to warn of imminent explosion
						if (e->entityTimer % 5 == 0)
						{
							e->sprite = bombSprites[2];
						}
						if (e->entityTimer % 10 == 0)
						{
							e->sprite = bombSprites[3];
						}
					}
					else
					{
						e->sprite = explosionSprites[e->entityTimer - 300];
					}
				}
				else
				{
					//If the bomb's velocity magnitude is large enough, use a modulus to calculate which 'rolling' sprite to use.
					e->sprite = bombSprites[4 + (e->entityTimer % 6)];
				}
				break;
			case BLAST:
				DrawTextureV(e->sprite, e->position - Vector2{ 16,16 }, WHITE);
				break;
			default:
				break;
			}
		}
	}
}

//-----STATE RENDER FUNCTIONS-----//

void RenderTitleScreen(Gamestate* state)
{
	//-----Parallax backround scrolling.-----//
	scrollingBGback -= 0.1f;
	scrollingBGmid -= 0.01f;
	scrollingBGfore -= 1.f;

	if (scrollingBGback <= -mainMenuTextures[3].width * 2) { scrollingBGback = 0.f; }

	DrawTextureEx(mainMenuTextures[3], Vector2{ scrollingBGback, 0 }, 0.f, 2.f, WHITE);
	DrawTextureEx(mainMenuTextures[3], Vector2{ (mainMenuTextures[3].width * 2) + scrollingBGback, 0 }, 0.f, 2.f, WHITE);

	if (scrollingBGmid <= -mainMenuTextures[2].width * 2) { scrollingBGmid = 0.f; }

	DrawTextureEx(mainMenuTextures[2], Vector2{ scrollingBGmid, 0 }, 0.f, 2.f, WHITE);
	DrawTextureEx(mainMenuTextures[2], Vector2{ (mainMenuTextures[2].width * 2) + scrollingBGmid, 0 }, 0.f, 2.f, WHITE);

	if (scrollingBGfore <= -mainMenuTextures[1].width * 2) { scrollingBGfore = 0.f; }

	DrawTextureEx(mainMenuTextures[1], Vector2{ scrollingBGfore, 0 }, 0.f, 2.f, WHITE);
	DrawTextureEx(mainMenuTextures[1], Vector2{ (mainMenuTextures[1].width * 2) + scrollingBGfore, 0 }, 0.f, 2.f, WHITE);

	DrawTextureEx(mainMenuTextures[0], Vector2{ 0, 0 }, 0.f, 2.f, WHITE);

	//-----Menu screen-----//

	textFlashing++;
	if (!state->menuDisplayed)
	{
		if (textFlashing < 30)
		{
			if (state->state == TITLESCREEN)
			{
				DrawText("Press 'Backspace' to Start.", 32, SCREENHEIGHT - 42, 32, RED);
			}
			else
			{
				DrawText("Press 'Backspace' to Exit.", 32, SCREENHEIGHT - 42, 32, RED);
			}
		}
	}
	else
	{
		if (textFlashing < 30)
		{
			DrawText("Press 'Enter' to Select.", 32, SCREENHEIGHT - 42, 32, RED);
		}
	}
	if (textFlashing == 60)
	{
		textFlashing = 0;
	}

}

void RenderGameplayScreen(Gamestate* state)
{
	//-----Arena outline
	DrawRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, BLACK);
	DrawRectangle(32, 32, SCREENWIDTH - 64, SCREENHEIGHT - 64, RAYWHITE);

	//-----Main Gameplay Level Renders.
	//-----Player Render
	DrawEntityPlayer(state);

	//-----StaticEntity Render
	if (state->staticEntityPointer.size() != 0)
	{
		for (StaticEntity* e : state->staticEntityPointer)
		{
			DrawTextureV(e->sprite, e->position, WHITE);
		}
	}

	//-----MovingEntity Render 
	DrawEntitiesNonPlayers(state);

	//-----UI Render (lives and score)
	/*Lives*/
	if (state->lives == 3) { DrawTextureV(heartSprite, UI_HEART + (UI_HEART_SPACER * 2), WHITE); }
	if (state->lives >= 2) { DrawTextureV(heartSprite, UI_HEART + UI_HEART_SPACER, WHITE); }
	if (state->lives >= 1) { DrawTextureV(heartSprite, UI_HEART, WHITE); }
	/*Timer*/
	std::string timeRemaing = "Time: ";
	timeRemaing += std::to_string(state->timer / 60);
	DrawText(timeRemaing.c_str(), (int)UI_TIMER.x, (int)UI_TIMER.y, 28, RAYWHITE);
	/*score*/
	std::string currentScoreString = "Score: ";
	currentScoreString += std::to_string(state->currentScore).c_str();
	DrawText((currentScoreString.c_str()), (int)UI_SCORE.x, (int)UI_SCORE.y, 28, RAYWHITE);
	std::string hiscore = "Hiscore: ";
	hiscore += std::to_string(highScoreArrayNumbers[0]);
	DrawText((hiscore.c_str()), (int)UI_HISCORE.x, (int)UI_HISCORE.y, 28, RAYWHITE);

	//--BOMB DROP SPOT
	DrawCircleV(state->bombDrop, 3, RED);

	//-----GameOver Window (when lives = 0, or timer = 0)
	if (state->gameOver)
	{
		if (state->enemiesRemaining > 0)
		{
			DrawText("GAME OVER", SCREENWIDTH_TENTH * 2, SCREENHEIGHT_TENTH * 4, 64, RED);
		}
		else
		{
			DrawText("VICTORY!!", SCREENWIDTH_TENTH * 2, SCREENHEIGHT_TENTH * 4, 64, RED);
		}
		DrawText("Press 'Backspace' to continue", SCREENWIDTH_TENTH, SCREENHEIGHT_TENTH * 6, 32, RED);
	}
}

void RenderScoresScreen(Gamestate* state)
{
	RenderTitleScreen(state);
	DrawRectangleRec(MENUWINDOW, DARKGRAY);
	DrawRectangleLinesEx(MENUWINDOW_FRAME, 2.f, RED);
	DrawText("HISCORE", SCREENWIDTH_TENTH * 4, SCREENHEIGHT_TENTH * 2 + 28, 32, YELLOW);

	int spaceString = 0;
	for (std::string s : highScoreArrayNames)
	{
		DrawText(s.c_str(), HISCORE_NAMES_XPOSITION, HISCORE_YAXIS_OFFSET + (64 * spaceString), 32, RAYWHITE);
		spaceString++;
	}
	int spaceInt = 0;
	for (int s : highScoreArrayNumbers)
	{
		std::string hScore = std::to_string(s);
		DrawText(hScore.c_str(), HISCORE_SCORES_XPOSITION, HISCORE_YAXIS_OFFSET + (64 * spaceInt), 32, RAYWHITE);
		spaceInt++;
	}

	if (newHiscore)
	{
		DrawRectangleRec(HISCORE_NAMEINPUT_WINDOW, LIGHTGRAY);
		DrawRectangleLinesEx(HISCORE_NAMEINPUT_LINES, 2.f, RED);
		DrawText("Input 3 Characters", (int)HISCORE_NAMEINPUT_WINDOW.x + 12, (int)HISCORE_NAMEINPUT_WINDOW.y + 32, 24, YELLOW);
		DrawText(inputName, (int)(HISCORE_NAMEINPUT_WINDOW.x + SCREENWIDTH_TENTH * 1.5f), (int)(HISCORE_NAMEINPUT_WINDOW.y + 64), 24, YELLOW);
	}
}

//This will be red for as long as there is no collision detected aren't colliding. 
static Color collisonCheckColour = RED;
//This will read what the current collision mode is
static std::string collisionTestModeString = "Current Mode";

void RenderTestScreen(Gamestate* state)
{
	if (state->isCollidingInTestScreen)
	{
		collisonCheckColour = GREEN;
	}
	else
	{
		collisonCheckColour = RED;
	}
	ClearBackground(BLACK);
	DrawText("This is the Test menu", SCREENWIDTH_TENTH, SCREENHEIGHT_TENTH, 32, YELLOW);
	switch (state->testMode)
	{
	case CIRCLE_CIRCLE:
		collisionTestModeString = "Circle vs Circle";
		if (state->movingEntityPointer.size() != 0)
		{
			for (MovingEntity* movingEntity : state->movingEntityPointer)
			{
				DrawCircleV(movingEntity->position, movingEntity->size, collisonCheckColour);
			}
		}
		break;
	case CIRCLE_AABB:
		collisionTestModeString = "Circle vs AABB";
		if (state->movingEntityPointer.size() != 0 && state->staticEntityPointer.size() != 0)
		{
			for (MovingEntity* movingEntity : state->movingEntityPointer)
			{
				DrawCircleV(movingEntity->position, movingEntity->size, collisonCheckColour);
			}
			for (StaticEntity* staticEntity : state->staticEntityPointer)
			{
				DrawRectangleV(staticEntity->position, { staticEntity->size,staticEntity->size }, collisonCheckColour);
			}
		}
		break;
	case CIRCLE_LINESEGMENT:
		collisionTestModeString = "Circle vs Line segment";
		if (state->movingEntityPointer.size() != 0)
		{
			for (MovingEntity* movingEntity : state->movingEntityPointer)
			{
				DrawCircleV(movingEntity->position, movingEntity->size, collisonCheckColour);
			}
			DrawLineEx(state->movingEntityPointer[1]->position, state->movingEntityPointer[2]->position, 3.f, collisonCheckColour);
		}
		break;
	case AABB_AABB:
		collisionTestModeString = "AABB vs AABB";
		if (state->staticEntityPointer.size() != 0)
		{
			for (StaticEntity* staticEntity : state->staticEntityPointer)
			{
				DrawRectangleV(staticEntity->position, { staticEntity->size,staticEntity->size }, collisonCheckColour);
			}
		}
		break;
	default:
		break;
	}
	DrawText("Current Collision Test:", SCREENWIDTH_TENTH, SCREENHEIGHT_TENTH * 6, 32, YELLOW);
	DrawText(collisionTestModeString.c_str(), SCREENWIDTH_TENTH, SCREENHEIGHT_TENTH * 7, 32, collisonCheckColour);
	DrawText("Click the shape you want to move, and drag.", SCREENWIDTH_TENTH, SCREENHEIGHT_TENTH * 8, 16, YELLOW);
	DrawText("Use Left-Right arrow keys to change mode", SCREENWIDTH_TENTH, SCREENHEIGHT_TENTH * 8 + 20, 16, YELLOW);
	DrawText("When objects collide, they should turn green!", SCREENWIDTH_TENTH, SCREENHEIGHT_TENTH * 8 + 40, 16, YELLOW);
}

//Button colour, these are use in the method below and have their colours overriden by the player menu selection.
static Color firstSelection = DARKGRAY;
static Color secondSelection = DARKGRAY;
static Color thirdSelection = DARKGRAY;

void RenderMenu(Gamestate* state)
{
	//Menu Window Background
	DrawRectangleRec(MENUWINDOW, LIGHTGRAY);
	DrawRectangleLinesEx(MENUWINDOW_FRAME, 2.f, RED);
	//Buttons
	DrawRectangleV(BUTTON_POSITION * Vector2{ 1,3 }, BUTTON_SIZE, firstSelection);
	DrawRectangleV(BUTTON_POSITION * Vector2{ 1,4.5f }, BUTTON_SIZE, secondSelection);
	DrawRectangleV(BUTTON_POSITION * Vector2{ 1,6 }, BUTTON_SIZE, thirdSelection);
	//Draws text over button (to show what selecting it does in different modes)
	switch (state->state)
	{
	case TITLESCREEN:
		DrawText("Start Game", (int)(BUTTON_POSITION.x + 64.f), (int)(BUTTON_POSITION.y * 3.4f), 24, RED);
		DrawText("Hiscores", (int)(BUTTON_POSITION.x + 72.f), (int)(BUTTON_POSITION.y * 4.8f), 24, RED);
		DrawText("Test Screen", (int)(BUTTON_POSITION.x + 58.f), (int)(BUTTON_POSITION.y * 6.3f), 24, RED);
		break;
	case GAMEPLAYSCREEN:
		if (!state->gameOver)
		{
			DrawText("Continue", (int)(BUTTON_POSITION.x + 72.f), (int)(BUTTON_POSITION.y * 3.4f), 24, RED);
			DrawText("Resign", (int)(BUTTON_POSITION.x + 82.f), (int)(BUTTON_POSITION.y * 4.8f), 24, RED);
			DrawText("Main Menu", (int)(BUTTON_POSITION.x + 68.f), (int)(BUTTON_POSITION.y * 6.3f), 24, RED);
		}
		else
		{
			DrawText("Hiscore", (int)(BUTTON_POSITION.x + 72.f), (int)(BUTTON_POSITION.y * 3.4f), 24, RED);
			DrawText("Retry", (int)(BUTTON_POSITION.x + 82.f), (int)(BUTTON_POSITION.y * 4.8f), 24, RED);
			DrawText("Main Menu", (int)(BUTTON_POSITION.x + 68.f), (int)(BUTTON_POSITION.y * 6.3f), 24, RED);
		}
		break;
	case SCORESCREEN:
		DrawText("Close Menu", (int)(BUTTON_POSITION.x + 68.f), (int)(BUTTON_POSITION.y * 3.4f), 24, RED);
		DrawText("Reset Scores", (int)(BUTTON_POSITION.x + 52.f), (int)(BUTTON_POSITION.y * 4.8f), 24, RED);
		DrawText("Main Menu", (int)(BUTTON_POSITION.x + 68.f), (int)(BUTTON_POSITION.y * 6.3f), 24, RED);
		break;
	case TESTSCREEN:
		DrawText("Continue", (int)(BUTTON_POSITION.x + 72.f), (int)(BUTTON_POSITION.y * 3.4f), 24, RED);
		DrawText("Reset", (int)(BUTTON_POSITION.x + 82.f), (int)(BUTTON_POSITION.y * 4.8f), 24, RED);
		DrawText("Main Menu", (int)(BUTTON_POSITION.x + 58.f), (int)(BUTTON_POSITION.y * 6.3f), 24, RED);
		break;
	default: //Should never get here.
		break;
	}
	//Changes colour of the selected button
	firstSelection = DARKGRAY;
	secondSelection = DARKGRAY;
	thirdSelection = DARKGRAY;
	switch (state->menuSelection)
	{
	case 0:
		break;
	case 1:
		firstSelection = YELLOW;
		break;
	case 2:
		secondSelection = YELLOW;
		break;
	case 3:
		thirdSelection = YELLOW;
		break;
	default: //should never get here.
		break;
	}
	//Extra text to dress the menu.
	DrawText("Main Menu", SCREENWIDTH_TENTH * 4 - 18, (int)(SCREENHEIGHT_TENTH * 2.4f), 32, RED);
	DrawText("Bombergirl, a Programming for C++ 4 Assignment Project",
		(int)(SCREENWIDTH_TENTH * 2.3f), (int)(SCREENHEIGHT_TENTH * 7.3f), 12, RED);
	DrawText("Author: Gustaf Styf",
		(int)(SCREENWIDTH_TENTH * 2.3f), (int)(SCREENHEIGHT_TENTH * 7.5f), 12, RED);
}

void Render(Gamestate* state)
{
	switch (state->state)
	{
	case TITLESCREEN:
		RenderTitleScreen(state);
		break;
	case GAMEPLAYSCREEN:
		RenderGameplayScreen(state);
		break;
	case SCORESCREEN:
		RenderScoresScreen(state);
		break;
	case TESTSCREEN:
		RenderTestScreen(state);
		break;
	default: //Should never get here.
		break;
	}
	if (state->menuDisplayed)
	{
		RenderMenu(state);
	}
}