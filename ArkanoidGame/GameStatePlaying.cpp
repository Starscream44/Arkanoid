#include "GameStatePlaying.h"

#include "Application.h"
#include "Game.h"
#include "Text.h"
#include "Ball.h"
#include "Block.h"
#include "DurableBlock.h"
#include "GlassBlock.h"

#include <assert.h>
#include <algorithm>
#include <memory>
#include <random>
#include <string>
#include <vector>

namespace Arkanoid
{
	void InitGameStatePlaying(GameStatePlayingData& data)
	{
		data.paddle.Init(SCREEN_WIDTH, SCREEN_HEGHT);
		data.ball.Init(SCREEN_WIDTH, SCREEN_HEGHT);

		data.score = 0;
		data.blocks.clear();

		const int columns = 10;
		const int rows = 3;
		const int totalBlocks = rows * columns;

		const int durableBlocksCount = 6;
		const int glassBlocksCount = 4;

		const int actualDurableBlocksCount = std::min(durableBlocksCount, totalBlocks);
		const int actualGlassBlocksCount = std::min(glassBlocksCount, totalBlocks - actualDurableBlocksCount);

		const float marginX = 40.f;
		const float startY = 90.f;
		const float spacingX = 5.f;
		const float spacingY = 8.f;

		const float blockWidth =
			(SCREEN_WIDTH - marginX * 2.f - spacingX * (columns - 1)) / columns;

		const sf::Vector2f blockSize = { blockWidth, 25.f };

		enum class BlockType
		{
			Regular,
			Durable,
			Glass
		};

		std::vector<BlockType> blockTypes(totalBlocks, BlockType::Regular);

		std::vector<int> blockIndexes;

		for (int i = 0; i < totalBlocks; ++i)
		{
			blockIndexes.push_back(i);
		}

		std::random_device randomDevice;
		std::mt19937 randomGenerator(randomDevice());

		std::shuffle(
			blockIndexes.begin(),
			blockIndexes.end(),
			randomGenerator
		);

		for (int i = 0; i < actualDurableBlocksCount; ++i)
		{
			blockTypes[blockIndexes[i]] = BlockType::Durable;
		}

		for (int i = 0; i < actualGlassBlocksCount; ++i)
		{
			const int index = actualDurableBlocksCount + i;
			blockTypes[blockIndexes[index]] = BlockType::Glass;
		}

		for (int row = 0; row < rows; ++row)
		{
			for (int column = 0; column < columns; ++column)
			{
				const int blockIndex = row * columns + column;

				const sf::Vector2f position =
				{
					marginX + column * (blockSize.x + spacingX),
					startY + row * (blockSize.y + spacingY)
				};

				switch (blockTypes[blockIndex])
				{
				case BlockType::Durable:
				{
					auto durableBlock = std::make_unique<DurableBlock>();

					durableBlock->Init(
						position,
						blockSize,
						3,
						{
							sf::Color::Red,
							sf::Color::Yellow,
							sf::Color::Blue
						}
					);

					data.blocks.push_back(std::move(durableBlock));
					break;
				}

				case BlockType::Glass:
				{
					auto glassBlock = std::make_unique<GlassBlock>();

					glassBlock->Init(position, blockSize);

					data.blocks.push_back(std::move(glassBlock));
					break;
				}

				case BlockType::Regular:
				default:
				{
					auto block = std::make_unique<Block>();

					block->Init(position, blockSize);

					data.blocks.push_back(std::move(block));
					break;
				}
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