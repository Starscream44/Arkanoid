#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <assert.h>
#include <string>

namespace SnakeGame
{
	void InitGameStatePlaying(GameStatePlayingData& data)
	{
		data.paddle.Init(SCREEN_WIDTH, SCREEN_HEGHT);
		data.ball.Init(SCREEN_WIDTH, SCREEN_HEGHT);

		assert(data.font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

		data.background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEGHT));
		data.background.setPosition(0.f, 0.f);
		data.background.setFillColor(sf::Color(15, 15, 25));

		data.titleText.setFont(data.font);
		data.titleText.setCharacterSize(36);
		data.titleText.setFillColor(sf::Color::Cyan);
		data.titleText.setString("Arkanoid Prototype");

		data.score = 0;

		data.scoreText.setFont(data.font);
		data.scoreText.setCharacterSize(24);
		data.scoreText.setFillColor(sf::Color::Yellow);
		data.scoreText.setString("Score: 0");

		data.inputHintText.setFont(data.font);
		data.inputHintText.setCharacterSize(24);
		data.inputHintText.setFillColor(sf::Color::White);
		data.inputHintText.setString("ESC - pause");
		data.inputHintText.setOrigin(GetTextOrigin(data.inputHintText, { 1.f, 0.f }));
	}

	void ShutdownGameStatePlaying(GameStatePlayingData& data)
	{
		// Nothing special here for now
	}

	void HandleGameStatePlayingWindowEvent(GameStatePlayingData& data, const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PushState(GameStateType::ExitDialog, false);
			}
		}
	}

	void UpdateGameStatePlaying(GameStatePlayingData& data, float timeDelta)
	{
		data.paddle.Update(timeDelta);
		data.ball.Update(timeDelta, data.paddle);
		data.scoreText.setString("Score: " + std::to_string(data.score));

	}

	void DrawGameStatePlaying(GameStatePlayingData& data, sf::RenderWindow& window)
	{
		window.draw(data.background);
		data.paddle.Draw(window);
		data.ball.Draw(window);

		sf::Vector2f viewSize = window.getView().getSize();

		data.titleText.setOrigin(GetTextOrigin(data.titleText, { 0.5f, 0.f }));
		data.titleText.setPosition(viewSize.x / 2.f, 20.f);
		window.draw(data.titleText);

		data.scoreText.setOrigin(GetTextOrigin(data.scoreText, { 0.f, 0.f }));
		data.scoreText.setPosition(10.f, 10.f);
		window.draw(data.scoreText);

		data.inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(data.inputHintText);
	}
}