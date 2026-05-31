#pragma once
#include "Collidable.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	class Paddle;

	class Ball : public Collidable
	{
	public:
		void Init(float fieldWidth, float fieldHeight);
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window) const;

		sf::FloatRect GetBounds() const override;

		void BounceFromBlock(const sf::FloatRect& blockBounds);

	protected:
		void OnHit(Collidable& collidable) override;

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