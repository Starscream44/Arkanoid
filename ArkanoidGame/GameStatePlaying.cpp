#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <assert.h>
#include "Ball.h"
#include <string>

namespace Arkanoid
{
	void InitGameStatePlaying(GameStatePlayingData& data)
	{
		data.paddle.Init(SCREEN_WIDTH, SCREEN_HEGHT);
		data.ball.Init(SCREEN_WIDTH, SCREEN_HEGHT);
	
		data.blocks.clear();

		const sf::Vector2f blockSize = { 120.f, 30.f };
		const float startX = 75.f;
		const float startY = 100.f;
		const float spacingX = 10.f;
		const float spacingY = 10.f;

		const int columns = 5;
		const int rows = 2;

		for (int row = 0; row < rows; ++row)
		{
			for (int column = 0; column < columns; ++column)
			{
				Block block;

				const sf::Vector2f position =
				{
					startX + column * (blockSize.x + spacingX),
					startY + row * (blockSize.y + spacingY)
				};

				block.Init(position, blockSize);
				data.blocks.push_back(block);
			}
		}

		assert(data.font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));
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

		for (Block& block : data.blocks)
		{
			block.Update(timeDelta);

			if (block.IsDestroyed())
			{
				continue;
			}

			if (data.ball.GetBounds().intersects(block.GetBounds()))
			{
				data.ball.BounceFromBlock(block.GetBounds());
				block.Destroy();

				data.score += 1;

				break;
			}
		}

		bool allBlocksDestroyed = !data.blocks.empty();

		for (const Block& block : data.blocks)
		{
			if (!block.IsDestroyed())
			{
				allBlocksDestroyed = false;
				break;
			}
		}

		if (allBlocksDestroyed)
		{
			Application::Instance().GetGame().SwitchStateTo(GameStateType::Victory);
			return;
		}

		data.scoreText.setString("Score: " + std::to_string(data.score));
	}

	void DrawGameStatePlaying(GameStatePlayingData& data, sf::RenderWindow& window)
	{
		window.draw(data.background);

		for (const Block& block : data.blocks)
		{
			block.Draw(window);
		}

		data.paddle.Draw(window);
		data.ball.Draw(window);
	}
}