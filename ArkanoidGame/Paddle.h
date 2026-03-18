#pragma once
#include <SFML/Graphics.hpp>

namespace SnakeGame
{
	class Paddle
	{
	public:
		void Init(float fieldWidth, float fieldHeight);
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window) const;

		sf::FloatRect GetBounds() const;

	private:
		void ClampInsideField();

	private:
		sf::RectangleShape shape;

		float speed = 600.f;

		float fieldWidth = 0.f;
		float fieldHeight = 0.f;
	};
}