#pragma once
#include <string>

namespace Arkanoid
{
	 class GameWorld
	 {
	 private:
	 	GameWorld()=default;
	 	
	 public:
	 	static GameWorld& Instance()
	 	{
	 		static GameWorld gameWorld;
	 		return gameWorld;
	 	}
	 
	 	// Resources path
	 	const std::string RESOURCES_PATH = "Resources/";
	 	const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
	 	const std::string LEVELS_CONFIG_PATH = RESOURCES_PATH + "levels.config";
	 	const std::string SAVE_FILE_PATH = RESOURCES_PATH + "savegame.txt";
	 	const std::string RECORDS_FILE_PATH = RESOURCES_PATH + "records.txt";

	 	// Game settings constants
	 	const unsigned int SCREEN_WIDTH = 800;
	 	const unsigned int SCREEN_HEIGHT = 600;
	 	const float TIME_PER_FRAME = 1.f / 60.f; // 60 fps

	 	const int MAX_RECORDS_TABLE_SIZE = 5;
	 	const char* PLAYER_NAME = "Player";

	 	const std::string GAME_NAME = "Arkanoid";
	 };
}

#define SETTINGS GameWorld :: Instance()
