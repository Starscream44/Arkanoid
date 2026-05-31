#pragma once
#include "SFML/Graphics.hpp"
#include "Paddle.h"
#include "Ball.h"
#include "Block.h"

#include <vector>
#include <memory>

namespace Arkanoid
{
	class Game;

	class GameStatePlayingData
	{
		friend void InitGameStatePlaying(GameStatePlayingData& data);
		friend void ShutdownGameStatePlaying(GameStatePlayingData& data);
		friend void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, const sf::Event& event);
		friend void UpdateGameStatePlaying(GameStatePlayingData& data, float timeDelta);
		friend void DrawGameStatePlaying(GameStatePlayingData& data, sf::RenderWindow& window);

	private:
		sf::Font font;

		sf::RectangleShape background;

		sf::Text titleText;
		sf::Text scoreText;
		sf::Text inputHintText;
		Paddle paddle;
		Ball ball;
		std::vector<std::unique_ptr<Block>> blocks;

		int score = 0;
	};

	void InitGameStatePlaying(GameStatePlayingData& data);
	void ShutdownGameStatePlaying(GameStatePlayingData& data);
	void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, const sf::Event& event);
	void UpdateGameStatePlaying(GameStatePlayingData& data, float timeDelta);
	void DrawGameStatePlaying(GameStatePlayingData& data, sf::RenderWindow& window);
}