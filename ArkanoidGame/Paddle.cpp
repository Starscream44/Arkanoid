#include "Paddle.h"
#include <algorithm>

namespace Arkanoid
{
	void Paddle::Init(float newFieldWidth, float newFieldHeight)
	{
		fieldWidth = newFieldWidth;
		fieldHeight = newFieldHeight;

		shape.setSize(sf::Vector2f(140.f, 20.f));
		shape.setFillColor(sf::Color::Green);

		shape.setOrigin(shape.getSize().x * 0.5f, shape.getSize().y * 0.5f);
		shape.setPosition(fieldWidth * 0.5f, fieldHeight - 40.f);
	}

	void Paddle::Update(float timeDelta)
	{
		float directionX = 0.f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			directionX -= 1.f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			directionX += 1.f;
		}

		shape.move(directionX * speed * timeDelta, 0.f);

		ClampInsideField();
	}

	void Paddle::Draw(sf::RenderWindow& window) const
	{
		window.draw(shape);
	}

	sf::FloatRect Paddle::GetBounds() const
	{
		return shape.getGlobalBounds();
	}

	void Paddle::OnHit(Collidable& collidable)
	{
		// Paddle does not need to react.
		// Ball reacts to the paddle.
	}

	void Paddle::ClampInsideField()
	{
		const float halfWidth = shape.getSize().x * 0.5f;

		sf::Vector2f position = shape.getPosition();

		position.x = std::max(halfWidth, position.x);
		position.x = std::min(fieldWidth - halfWidth, position.x);

		shape.setPosition(position);
	}
}