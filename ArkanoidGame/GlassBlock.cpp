#include "GlassBlock.h"

namespace Arkanoid
{
	void GlassBlock::Init(const sf::Vector2f& position, const sf::Vector2f& size)
	{
		Block::Init(position, size);

		shape.setFillColor(sf::Color(255, 255, 255, 100));
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(2.f);
	}

	bool GlassBlock::ShouldBounceBall() const
	{
		return false;
	}

	void GlassBlock::OnHit(Collidable& collidable)
	{
		Destroy();
	}
}