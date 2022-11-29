#include <SFML/Graphics.hpp>

#include "gui.hpp"
#include "assets.hpp"
#include "player.hpp"
#include "level.hpp"
#include "utils.hpp"

#pragma once

class Game {

public:
	Game();
	~Game();

	void mainloop();

private:
	void manageEvent();

	sf::RenderWindow m_window;
	sf::Event m_event;
	sf::Clock m_deltaclock;
	sf::Time m_deltatime;
	float m_dt; // convinence 

	AssetManager m_assetmanager;
	Gui m_gui;
	Player m_player;
	LevelManager m_levelmanager;

	sf::Music m_menu, m_game_music;
};