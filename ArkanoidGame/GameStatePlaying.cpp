#include "GameStatePlaying.h"

#include "Application.h"
#include "Game.h"
#include "Text.h"
#include "Ball.h"
#include "Block.h"
#include "BlockFactory.h"

#include <assert.h>
#include <memory>
#include <string>


namespace Arkanoid
{
	void InitBlockFactories(GameStatePlayingData& data)
	{
		data.factories.clear();

		data.factories.emplace(BlockType::Regular, std::make_unique<RegularBlockFactory>());
		data.factories.emplace(BlockType::ThreeHit, std::make_unique<ThreeHitBlockFactory>());
		data.factories.emplace(BlockType::Glass, std::make_unique<GlassBlockFactory>());
		data.factories.emplace(BlockType::Unbreakable, std::make_unique<UnbreakableBlockFactory>());
	}

	void ResetBlockFactoryCounters(GameStatePlayingData& data)
	{
		for (auto& item : data.factories)
		{
			item.second->ClearCounter();
		}
	}

	void LoadGameLevel(GameStatePlayingData& data, int levelIndex)
	{
		assert(levelIndex >= 0 && levelIndex < data.levelLoader.GetLevelsCount());

		data.currentLevel = levelIndex;
		data.breakableBlocksCount = 0;
		data.destroyedBreakableBlocksCount = 0;
		data.blocks.clear();
		ResetBlockFactoryCounters(data);

		data.paddle.Init(static_cast<float>(SETTINGS.SCREEN_WIDTH), static_cast<float>(SETTINGS.SCREEN_HEGHT));
		data.ball.Init(static_cast<float>(SETTINGS.SCREEN_WIDTH), static_cast<float>(SETTINGS.SCREEN_HEGHT));

		const LevelData& level = data.levelLoader.GetLevel(levelIndex);

		if (level.columns <= 0 || level.rows <= 0)
		{
			return;
		}

		const float marginX = 40.f;
		const float startY = 90.f;
		const float spacingX = 5.f;
		const float spacingY = 8.f;

		const float blockWidth =
			(SETTINGS.SCREEN_WIDTH - marginX * 2.f - spacingX * (level.columns - 1)) / level.columns;

		const sf::Vector2f blockSize = { blockWidth, 25.f };

		for (const LevelBlockData& blockData : level.blocks)
		{
			const sf::Vector2f position =
			{
				marginX + blockData.gridPosition.x * (blockSize.x + spacingX),
				startY + blockData.gridPosition.y * (blockSize.y + spacingY)
			};

			BlockFactory& factory = *data.factories.at(blockData.blockType);
			data.blocks.push_back(factory.CreateBlock(position, blockSize));
		}

		for (const auto& item : data.factories)
		{
			data.breakableBlocksCount += item.second->GetCreatedBreakableBlocksCount();
		}
	}

	void InitGameStatePlaying(GameStatePlayingData& data)
	{
		data.score = 0;
		InitBlockFactories(data);
		assert(data.levelLoader.LoadLevelsFromFile(SETTINGS.LEVELS_CONFIG_PATH));
		LoadGameLevel(data, 0);

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
				Application::Instance().GetGame().PauseGame();
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
					data.destroyedBreakableBlocksCount += 1;
				}

				break;
			}
		}

		if (data.breakableBlocksCount > 0 && data.destroyedBreakableBlocksCount >= data.breakableBlocksCount)
		{
			const int nextLevel = data.currentLevel + 1;
			if (nextLevel < data.levelLoader.GetLevelsCount())
			{
				LoadGameLevel(data, nextLevel);
			}
			else
			{
				Application::Instance().GetGame().WinGame();
			}

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
