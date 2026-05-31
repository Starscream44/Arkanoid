#include "Block.h"

namespace Arkanoid
{
	void Block::Init(const sf::Vector2f& position, const sf::Vector2f& size)
	{
		shape.setSize(size);
		shape.setFillColor(sf::Color::Blue);
		shape.setOutlineColor(sf::Color::White);
		shape.setOutlineThickness(2.f);
		shape.setPosition(position);

		isDestroyed = false;
	}

	void Block::Update(float timeDelta)
	{

	}

	void Block::Draw(sf::RenderWindow& window) const
	{
		if (!isDestroyed)
		{
			window.draw(shape);
		}
	}

	sf::FloatRect Block::GetBounds() const
	{
		return shape.getGlobalBounds();
	}

	bool Block::GetCollision(const Collidable& collidable) const
	{
		return !isDestroyed && Collidable::GetCollision(collidable);
	}

	bool Block::ShouldBounceBall() const
	{
		return true;
	}

	void Block::OnHit(Collidable& collidable)
	{
		Destroy();
	}


	void Block::Destroy()
	{
		isDestroyed = true;
	}

	bool Block::IsDestroyed() const
	{
		return isDestroyed;
	}
}