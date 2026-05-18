#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	class Block : public GameObject
	{
	public:
		void Init(const sf::Vector2f& position, const sf::Vector2f& size);

		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) const override;

		sf::FloatRect GetBounds() const override;

		void Destroy();
		bool IsDestroyed() const;

	private:
		sf::RectangleShape shape;
		bool isDestroyed = false;
	};
}
