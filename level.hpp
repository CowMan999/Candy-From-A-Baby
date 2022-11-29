#include <SFML/Graphics.hpp>
#include <sstream>

#include "assets.hpp"
#include "player.hpp"
#include "utils.hpp"
#include "enemy.hpp"

#pragma once

inline const int CAM_SPEED = 5;
inline const int TILE_SIZE = 64;

class LevelManager {

public:
	LevelManager(sf::RenderWindow& window, AssetManager& assets, Player& player, Gui& gui);
	~LevelManager();

	enum Level {
		level_one = 1,
		level_two,
		level_three,
		level_four,
		level_five,
		level_six,
		level_seven,
		level_eight,
		level_nine,

		level_count
	};

	void loadLevel(Level id);
	void update(float dt);
	void draw();

	bool loaded(Level l);

private:
	sf::RenderWindow& m_window;
	AssetManager& m_assets;
	Player& m_player;
	Gui& m_gui;

	std::vector<sf::Sprite*> m_sprites;
	Level m_loaded = level_count;
	sf::View m_view;

	std::vector<sf::FloatRect> m_colision_map;
	sf::FloatRect m_win;
	std::vector<Enemy*> m_enemies;
};