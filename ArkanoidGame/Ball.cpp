#include "Ball.h"
#include "Paddle.h"
#include <cmath>

namespace SnakeGame
{
	void Ball::Init(float newFieldWidth, float newFieldHeight)
	{
		fieldWidth = newFieldWidth;
		fieldHeight = newFieldHeight;

		shape.setRadius(10.f);
		shape.setFillColor(sf::Color::White);
		shape.setOrigin(shape.getRadius(), shape.getRadius());
		shape.setPosition(fieldWidth * 0.5f, fieldHeight * 0.5f);
	}

	void Ball::Update(float timeDelta, const Paddle& paddle)
	{
		shape.move(velocity * timeDelta);

		ClampAndBounceFromWalls();
		BounceFromPaddle(paddle);
	}

	void Ball::Draw(sf::RenderWindow& window) const
	{
		window.draw(shape);
	}

	sf::FloatRect Ball::GetBounds() const
	{
		return shape.getGlobalBounds();
	}

	void Ball::ClampAndBounceFromWalls()
	{
		const float radius = shape.getRadius();
		sf::Vector2f position = shape.getPosition();

		if (position.x - radius <= 0.f && velocity.x < 0.f)
		{
			position.x = radius;
			velocity.x = -velocity.x;
		}
		else if (position.x + radius >= fieldWidth && velocity.x > 0.f)
		{
			position.x = fieldWidth - radius;
			velocity.x = -velocity.x;
		}

		if (position.y - radius <= 0.f && velocity.y < 0.f)
		{
			position.y = radius;
			velocity.y = -velocity.y;
		}
		else if (position.y + radius >= fieldHeight && velocity.y > 0.f)
		{
			position.y = fieldHeight - radius;
			velocity.y = -velocity.y;
		}

		shape.setPosition(position);
	}

	void Ball::BounceFromPaddle(const Paddle& paddle)
	{
		const sf::FloatRect paddleBounds = paddle.GetBounds();

		if (GetBounds().intersects(paddleBounds) && velocity.y > 0.f)
		{
			const float paddleCenterX = paddleBounds.left + paddleBounds.width * 0.5f;
			const float ballX = shape.getPosition().x;

			float hitOffset = (ballX - paddleCenterX) / (paddleBounds.width * 0.5f);

			if (hitOffset < -1.f)
			{
				hitOffset = -1.f;
			}
			else if (hitOffset > 1.f)
			{
				hitOffset = 1.f;
			}

			velocity.x = hitOffset * 300.f;
			velocity.y = -std::abs(velocity.y);

			shape.setPosition(ballX, paddleBounds.top - shape.getRadius());
		}
	}
}