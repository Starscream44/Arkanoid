#include "DurableBlock.h"

namespace Arkanoid
{
	void DurableBlock::Init(
		const sf::Vector2f& position,
		const sf::Vector2f& size,
		const std::vector<sf::Color>& damageColors
	)
	{
		Block::Init(position, size);

		this->damageColors = damageColors;
		hitPoints = static_cast<int>(damageColors.size());

		UpdateColor();
	}

	void DurableBlock::OnHit(Collidable& collidable)
	{
		if (isDestroyed)
		{
			return;
		}

		--hitPoints;

		if (hitPoints <= 0)
		{
			Destroy();
			return;
		}

		UpdateColor();
	}

	void DurableBlock::UpdateColor()
	{
		if (damageColors.empty())
		{
			return;
		}

		const int colorIndex = static_cast<int>(damageColors.size()) - hitPoints;

		if (colorIndex >= 0 && colorIndex < static_cast<int>(damageColors.size()))
		{
			shape.setFillColor(damageColors[colorIndex]);
		}
	}
}