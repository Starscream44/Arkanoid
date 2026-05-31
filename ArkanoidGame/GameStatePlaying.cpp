#include "GameStatePlaying.h"

#include "Application.h"
#include "Game.h"
#include "Text.h"
#include "Ball.h"
#include "Block.h"
#include "DurableBlock.h"

#include <assert.h>
#include <memory>
#include <string>
#include <algorithm>
#include <random>

namespace Arkanoid
{
	void InitGameStatePlaying(GameStatePlayingData& data)
	{
		data.paddle.Init(SCREEN_WIDTH, SCREEN_HEGHT);
		data.ball.Init(SCREEN_WIDTH, SCREEN_HEGHT);

		data.score = 0;
		data.blocks.clear();

		const sf::Vector2f blockSize = { 120.f, 30.f };

		const float startX = 75.f;
		const float startY = 100.f;
		const float spacingX = 10.f;
		const float spacingY = 10.f;

		const int columns = 5;
		const int rows = 2;
		const int totalBlocks = rows * columns;
		const int durableBlocksCount = 3;

		std::vector<int> durableBlockIndexes;

		for (int i = 0; i < totalBlocks; ++i)
		{
			durableBlockIndexes.push_back(i);
		}

		std::random_device randomDevice;
		std::mt19937 randomGenerator(randomDevice());

		std::shuffle(
			durableBlockIndexes.begin(),
			durableBlockIndexes.end(),
			randomGenerator
		);

		durableBlockIndexes.resize(durableBlocksCount);

		for (int row = 0; row < rows; ++row)
		{
			for (int column = 0; column < columns; ++column)
			{
				const int blockIndex = row * columns + column;

				const sf::Vector2f position =
				{
					startX + column * (blockSize.x + spacingX),
					startY + row * (blockSize.y + spacingY)
				};

				const bool isDurableBlock =
					std::find(
						durableBlockIndexes.begin(),
						durableBlockIndexes.end(),
						blockIndex
					) != durableBlockIndexes.end();

				if (isDurableBlock)
				{
					auto durableBlock = std::make_unique<DurableBlock>();

					durableBlock->Init(
						position,
						blockSize,
						{
							sf::Color::Red,
							sf::Color::Yellow,
							sf::Color::Blue
						}
					);

					data.blocks.push_back(std::move(durableBlock));
				}
				else
				{
					auto block = std::make_unique<Block>();

					block->Init(position, blockSize);

					data.blocks.push_back(std::move(block));
				}
			}
		}
		assert(data.font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));

		data.scoreText.setFont(data.font);
		data.scoreText.setCharacterSize(24);
		data.scoreText.setFillColor(sf::Color::White);
		data.scoreText.setPosition(20.f, 20.f);
		data.scoreText.setString("Score: " + std::to_string(data.score));
	}

	void ShutdownGameStatePlaying(GameStatePlayingData& data)
	{
		data.blocks.clear();
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

		data.ball.Update(timeDelta);

		data.ball.CheckCollision(data.paddle);

		for (std::unique_ptr<Block>& block : data.blocks)
		{
			block->Update(timeDelta);

			if (block->IsDestroyed())
			{
				continue;
			}

			if (data.ball.CheckCollision(*block))
			{
				if (block->IsDestroyed())
				{
					data.score += 1;
				}

				break;
			}
		}

		bool allBlocksDestroyed = !data.blocks.empty();

		for (const std::unique_ptr<Block>& block : data.blocks)
		{
			if (!block->IsDestroyed())
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

		for (const std::unique_ptr<Block>& block : data.blocks)
		{
			block->Draw(window);
		}

		data.paddle.Draw(window);
		data.ball.Draw(window);

		window.draw(data.scoreText);
	}
}