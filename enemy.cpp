#include "enemy.hpp"

Enemy::Enemy(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets, Type type) :
	m_player(player), m_gui(gui), m_assets(assets), m_type(type), m_window(m_gui.getWindow())
{
	m_sprite.setScale(2,2);
	m_sprite.setPosition(position);
}

Enemy::~Enemy() {

}

void Enemy::draw() {
	m_window.draw(m_sprite);
}

Enemy::Type Enemy::getType() {
	return m_type;
}

sf::Sprite& Enemy::getSprite() {
	return m_sprite;
}

sf::Vector2f Enemy::getPos() {
	return m_sprite.getPosition();
}
