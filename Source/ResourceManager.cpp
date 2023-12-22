#include "ResourceManager.h"
#include <iostream>
#include <fstream>


constexpr const char* ScoreFile = "./Resources/Score.bin";

//-----HISCORE FUNCTIONS
void LoadScore()
{
	std::ifstream sFile(ScoreFile, std::ios::binary);
	//std::string buffer;
	for (int i = 0; i < 5; i++)
	{
		highScoreArrayNames[i].resize(3);
		sFile.read(highScoreArrayNames[i].data(), 3);
		sFile.read((char*)&highScoreArrayNumbers[i], 4);
	}
}

//Writes the highscores to the .bin file.
void WriteScore()
{
	std::ofstream saveFile("./Resources/Score.bin", std::ios::out | std::ios::binary);
	for (int i = 0; i < 5; i++)
	{
		saveFile << highScoreArrayNames[i];
		saveFile.write(reinterpret_cast<char*>(&highScoreArrayNumbers[i]), sizeof(int));
	}
}

//After game moves to score screen, sorts the array of scores and names to add the now hiscore.
int UpdateHiscores(int score, std::string* names, int* hiscore)
{
	bool enterNewScore = false;
	int i = 0;
	for (i; i < 5; i++)
	{
		if (score >= hiscore[i])
		{
			std::cout << "Beat hiscore #" << (i + 1) << "!" << std::endl;
			enterNewScore = true;
			newHiscore = true;
			break;
		}
	}
	int newScoreSlot = i;
	if (enterNewScore)
	{
		for (int j = 4; j > i; j--)
		{
			if (j - 1 > i)
			{
				names[j] = names[j - 1];
				hiscore[j] = hiscore[j - 1];
			}
			else
			{
				hiscore[i + 1] = hiscore[i];
				hiscore[i] = score;
				names[i + 1] = names[i];
				names[i].clear();
			}
		}
		if (i == 4)
		{
			names[i].clear();
			hiscore[i] = score;
		}
	}
	return newScoreSlot;
}

//Reset current score and input name
void ResetCharacterInput()
{
	//Resets score for repeated playthoughs. 
	inputName[0] = '\0';
	inputName[1] = '\0';
	inputName[2] = '\0';
	inputCharacterCount = 0;
}

//Resets the score.bin to default values
void FactoryResetScore()
{
	//Only function is to reset the hiscores to factory settings, then write them to binary file
	highScoreArrayNames[0] = "AAA";
	highScoreArrayNumbers[0] = 50;
	highScoreArrayNames[1] = "BBB";
	highScoreArrayNumbers[1] = 40;
	highScoreArrayNames[2] = "CCC";
	highScoreArrayNumbers[2] = 30;
	highScoreArrayNames[3] = "DDD";
	highScoreArrayNumbers[3] = 20;
	highScoreArrayNames[4] = "EEE";
	highScoreArrayNumbers[4] = 10;
	WriteScore();
}

//These for-loops reads trough the resource folders and loads the textures into the sprite arrays
void LoadAssetsFromFile()
{
	//MainMenuBackgrounds
	for (int i = 0; i < 4; i++)
	{//\Resources\Screens\Menu
		std::string fileAssetsString = "./Resources/Screens/Menu/BGMain" + std::to_string(i) + ".png";
		const char* asset = fileAssetsString.c_str();
		mainMenuTextures[i] = LoadTexture(asset);
	}
	//CharacterSprites, 0-1 is walking right, 2-4 is placing bomb to right, 5-6 is walking left, 7-9 is dropping bomb left
	for (int i = 0; i < 10 ; i++)
	{
		std::string fileAssetsString = "./Resources/Sprites/Character" + std::to_string(i) + ".png";
		const char* asset = fileAssetsString.c_str();
		characterSprites[i] = LoadTexture(asset);
	}
	//BombSprites, 0-3 is the fuse animation, 4-9 bomb rolling
	for (int i = 0; i < 10 ; i++)
	{
		std::string fileAssetsString = "./Resources/Sprites/Bomb" + std::to_string(i) + ".png";
		const char* asset = fileAssetsString.c_str();
		bombSprites[i] = LoadTexture(asset);
	}
	//ExplosionSprites
	for (int i = 0; i < 7 ; i++)
	{
		std::string fileAssetsString = "./Resources/Sprites/Explosion" + std::to_string(i) + ".png";
		const char* asset = fileAssetsString.c_str();
		explosionSprites[i] = LoadTexture(asset);
	}
	//BlastSprites
	for (int i = 0; i < 4; i++)
	{
		std::string fileAssetsString = "./Resources/Sprites/Blasts" + std::to_string(i) + ".png";
		const char* asset = fileAssetsString.c_str();
		blastsSprites[i] = LoadTexture(asset);
	}
	//EnemySprites
	for (int i = 0; i < 4 ; i++)
	{
		std::string fileAssetsString = "./Resources/Sprites/Enemy" + std::to_string(i) + ".png";
		const char* asset = fileAssetsString.c_str();
		enemySprites[i] = LoadTexture(asset);
	}
	//BoxSprites, 0 = Destructible, 1 = Indistructible
	for (int i = 0; i < 2; i++)
	{
		std::string fileAssetsString = "./Resources/Sprites/Box" + std::to_string(i) + ".png";
		const char* asset = fileAssetsString.c_str();
		boxSprites[i] = LoadTexture(asset);
	}

	heartSprite = LoadTexture("./Resources/Sprites/Heart.png");
}
