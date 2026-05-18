#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

namespace Arkanoid
{
	class Game;

	class GameStateGameOverData
	{
		friend void InitGameStateGameOver(GameStateGameOverData& data);
		friend void ShutdownGameStateGameOver(GameStateGameOverData& data);
		friend void HandleGameStateGameOverWindowEvent(GameStateGameOverData& data, const sf::Event& event);
		friend void UpdateGameStateGameOver(GameStateGameOverData& data, float timeDelta);
		friend void DrawGameStateGameOver(GameStateGameOverData& data, sf::RenderWindow& window);

	private:
		// Resources
		sf::Font font;

		float timeSinceGameOver = 0.f;

		// UI data
		sf::RectangleShape background;
		sf::Text gameOverText;
		sf::Text hintText;
		std::vector<sf::Text> recordsTableTexts;
	};

	void InitGameStateGameOver(GameStateGameOverData& data);
	void ShutdownGameStateGameOver(GameStateGameOverData& data);
	void HandleGameStateGameOverWindowEvent(GameStateGameOverData& data, const sf::Event& event);
	void UpdateGameStateGameOver(GameStateGameOverData& data, float timeDelta);
	void DrawGameStateGameOver(GameStateGameOverData& data, sf::RenderWindow& window);
}