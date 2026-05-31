#pragma once
#include "GameObject.h"
#include "Collidable.h"
#include <SFML/Graphics.hpp>

namespace Arkanoid
{
	class Block : public GameObject, public Collidable
	{
	public:
		void Init(const sf::Vector2f& position, const sf::Vector2f& size);

		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) const override;

		sf::FloatRect GetBounds() const override;
		bool GetCollision(const Collidable& collidable) const override;

		void Destroy();
		bool IsDestroyed() const;

	protected:
		void OnHit(Collidable& collidable) override;

	private:
		sf::RectangleShape shape;
		bool isDestroyed = false;
	};
}