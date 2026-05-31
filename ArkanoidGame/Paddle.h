#pragma once
#include "Collidable.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	class Paddle : public Collidable
	{
	public:
		void Init(float fieldWidth, float fieldHeight);
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window) const;

		sf::FloatRect GetBounds() const override;

	protected:
		void OnHit(Collidable& collidable) override;

	private:
		void ClampInsideField();

	private:
		sf::RectangleShape shape;

		float speed = 600.f;

		float fieldWidth = 0.f;
		float fieldHeight = 0.f;
	};
}