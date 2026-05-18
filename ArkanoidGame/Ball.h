#pragma once
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	class Paddle;

	class Ball
	{
	public:
		void Init(float fieldWidth, float fieldHeight);
		void Update(float timeDelta, const Paddle& paddle);
		void Draw(sf::RenderWindow& window) const;

		sf::FloatRect GetBounds() const;

		void BounceFromBlock(const sf::FloatRect& blockBounds);

	private:
		void ClampAndBounceFromWalls();
		void BounceFromPaddle(const Paddle& paddle);

	private:
		sf::CircleShape shape;
		sf::Vector2f velocity = { 250.f, -250.f };

		float fieldWidth = 0.f;
		float fieldHeight = 0.f;
	};
}