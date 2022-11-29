#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <fstream>
#include <filesystem>

#include "font"
#include "font2"
#include "utils.hpp"

#pragma once

struct Save {
	float time_level[9] = {};
	bool won = false;

	float total = 0;
};

class Gui {

public:
	Gui(sf::RenderWindow& window);
	~Gui();

	void update(sf::Time dt);
	void handleEvent(sf::Event& event);

	enum State {
		state_none = -1,
		state_loading,
		state_mainmenu,
		state_how,
		state_levelselect,
		state_best,
		state_pause,
		state_lost,
		state_win,

		state_level,
		state_waiting_levelload,
	};

	State getState();
	void setState(State s);
	size_t getLevelId();

	void viewingPlayer(float dur);
	void resetLevel();

	// player won level
	void Lwon();

	// check if player lost
	bool lost();

	// check if player won
	bool won();

	// check if prev pause
	bool cpause();

	// check if start ready after win
	bool readyStart();

	// don't wanna document any more shit functions
	// check this one yourself
	void resetStartState();

	sf::RenderWindow& getWindow();

private:
	// pages
	void loading();
	void mainmenu();
	void levelselect();
	void pause();
	void level();
	void best();
	void mlost();
	void mwon();
	void howtoplay();

	// utility
	void centeredText(const char* text);
	std::vector<bool> centeredButtonRow(const std::vector<const char*> titles, const std::vector<ImVec2> widths);
	void centerByWidth(const float width);

	sf::RenderWindow& m_window;
	ImFont* m_font, *m_font2;

	State m_state = state_loading;
	size_t m_level_id = -1;

	float m_player_exposure = 0;
	sf::Time m_level_timer;

	Save m_save;

	bool m_won = 0;
	bool m_lost = 0;
	bool m_paused = 0;
	bool m_needsave = 0;

	bool m_readystart = 0;

};