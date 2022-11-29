#include <SFML/Graphics.hpp>
#include <sfcollider.hpp>
#include <cmath>

#include "assets.hpp"
#include "utils.hpp"
#include "gui.hpp"

#pragma once

inline const int PLAYER_ACCEL = 2450;
inline const float PLAYER_DECEL = 6.5f;
inline const int PLAYER_CAP = 450;
inline const float PLAYER_ANIM_SPEED = 0.25f;
inline const int PLAYER_ANIM_THRESHOLD = 100;

class Player {

public:
	Player(AssetManager& assetmanager, Gui& gui);
	~Player();

	// inform player game is ready; load and set up assets and such
	void ready();

	void update(float dt);

	void draw();

	void reset();

	sf::Vector2f getPos();

	void setColisionMap(std::vector<sf::FloatRect>& map, sf::FloatRect win);

	bool colliding(sf::Sprite& contender);

private:
	bool clickingW();
	bool clickingA();
	bool clickingS();
	bool clickingD();
	void collision();

	AssetManager& m_assetmanager;
	Gui& m_gui;
	sf::Sprite m_sprite;
	sf::RenderWindow& m_window;

	sf::Vector2f m_velocity;
	sf::Vector2f m_position, m_old;
	float m_animtimer = 0;

	float m_deadtimer = 0;
	float m_wintimer = 0;

	std::vector<sf::FloatRect>* m_collision_map;
	sf::FloatRect m_wining_rect;
};