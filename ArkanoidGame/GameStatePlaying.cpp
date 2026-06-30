#include "GameStatePlaying.h"

#include "Application.h"
#include "Game.h"
#include "Text.h"
#include "Ball.h"
#include "Block.h"
#include "BlockFactory.h"

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
		data.paddle.Init(static_cast<float>(SETTINGS.SCREEN_WIDTH), static_cast<float>(SETTINGS.SCREEN_HEGHT));
		data.ball.Init(static_cast<float>(SETTINGS.SCREEN_WIDTH), static_cast<float>(SETTINGS.SCREEN_HEGHT));

		data.score = 0;
		data.breakableBlocksCount = 0;
		data.blocks.clear();
		data.factories.clear();

		data.factories.emplace(BlockType::Regular, std::make_unique<RegularBlockFactory>());
		data.factories.emplace(BlockType::ThreeHit, std::make_unique<ThreeHitBlockFactory>());
		data.factories.emplace(BlockType::Glass, std::make_unique<GlassBlockFactory>());
		data.factories.emplace(BlockType::Unbreakable, std::make_unique<UnbreakableBlockFactory>());

		const int columns = 10;
		const int rows = 3;
		const int totalBlocks = rows * columns;

		const int threeHitBlocksCount = 6;
		const int glassBlocksCount = 4;
		const int unbreakableBlocksCount = 3;

		const int actualThreeHitBlocksCount = std::min(threeHitBlocksCount, totalBlocks);
		const int actualGlassBlocksCount = std::min(glassBlocksCount, totalBlocks - actualThreeHitBlocksCount);
		const int actualUnbreakableBlocksCount = std::min(
			unbreakableBlocksCount,
			totalBlocks - actualThreeHitBlocksCount - actualGlassBlocksCount
		);

		const float marginX = 40.f;
		const float startY = 90.f;
		const float spacingX = 5.f;
		const float spacingY = 8.f;

		const float blockWidth =
			(SETTINGS.SCREEN_WIDTH - marginX * 2.f - spacingX * (columns - 1)) / columns;

		const sf::Vector2f blockSize = { blockWidth, 25.f };

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

		for (int i = 0; i < actualThreeHitBlocksCount; ++i)
		{
			blockTypes[blockIndexes[i]] = BlockType::ThreeHit;
		}

		for (int i = 0; i < actualGlassBlocksCount; ++i)
		{
			const int index = actualThreeHitBlocksCount + i;
			blockTypes[blockIndexes[index]] = BlockType::Glass;
		}

		for (int i = 0; i < actualUnbreakableBlocksCount; ++i)
		{
			const int index = actualThreeHitBlocksCount + actualGlassBlocksCount + i;
			blockTypes[blockIndexes[index]] = BlockType::Unbreakable;
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

				BlockFactory& factory = *data.factories.at(blockTypes[blockIndex]);
				data.blocks.push_back(factory.CreateBlock(position, blockSize));
			}
		}

		for (const auto& item : data.factories)
		{
			data.breakableBlocksCount += item.second->GetCreatedBreakableBlocksCount();
		}

		assert(data.font.loadFromFile(SETTINGS.FONTS_PATH + "Roboto-Regular.ttf"));

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

		if (data.breakableBlocksCount > 0 && data.score >= data.breakableBlocksCount)
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
