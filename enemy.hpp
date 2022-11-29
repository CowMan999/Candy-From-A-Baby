#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "assets.hpp"
#include "gui.hpp"
#include "utils.hpp"

#pragma once

inline const float CAMERA_DAMAGE = 4.f;
inline const float LASER_DAMAGE = 6.5f;
inline const float LASER_ANIM_SPEED = 14.5f;
inline const float ROBOT_ANIM_SPEED = 1.25f;
inline const unsigned int ROBOT_BASE_SPEED = 300;

class Enemy {

public:
	enum Type {
		enemy_camera,
		enemy_laser,
		enemy_robot,
		enemy_baby,
	};

	Enemy(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets, Type type);
	virtual ~Enemy();

	virtual void update(float dt) = 0;

	Type getType();
	sf::Sprite& getSprite();
	sf::Vector2f getPos();

	void draw();

protected:
	sf::Sprite m_sprite;

	Player& m_player;
	Gui& m_gui;
	AssetManager& m_assets;

private:
	Type m_type;

protected:
	sf::RenderWindow& m_window;
	bool m_viewing = 0;

};

// enemy child classes
class Camera : public Enemy {
public:
	Camera(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets);
	~Camera();

	void update(float dt);

	void params(float dir, float speed, unsigned int mag);

private:
	float m_timer = 0;

	float m_dir = 0, m_speed = 0;
	unsigned int m_mag = 0;
};

class Laser : public Enemy {
public:
	Laser(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets);
	~Laser();

	void update(float dt);

	void params(bool hori, float speed, sf::Vector2f pos2);

private:
	float m_timer = 0, m_animtimer = 0;

	bool m_hori = 0;
	float m_speed = 0;

	sf::Vector2f m_pos, m_pos2;
};

class Robot : public Enemy {
public:
	Robot(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets);
	~Robot();

	void update(float dt);

	void params(float speed, float rot_speed, float acc, float range, float dir);

private:
	float m_timer = 0, m_animtimer = 0;

	float m_speed = 0, m_range = 0, m_rot_speed = 0, m_acc = 0;
	float m_angle = 0, m_save_angle = 0;

	bool m_enraged = 0;

	std::vector<sf::IntRect>& m_animation;
};

class Baby : public Enemy {
public:
	Baby(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets);
	~Baby();

	void update(float dt);

	void params(float speed, float rot_speed, float acc, float range);

	void over();

private:
	float m_timer = 0, m_animtimer = 0;

	float m_speed = 0, m_range = 0, m_rot_speed = 0, m_acc = 0;
	float m_angle = 0, m_save_angle = 0, m_move_angle = 0;

	bool m_enraged = 0;
	bool m_over = 0;

	std::vector<sf::IntRect>& m_animation;
};
