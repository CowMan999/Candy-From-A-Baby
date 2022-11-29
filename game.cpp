#include "game.hpp"

Game::Game() : m_window(sf::VideoMode(1920, 1080), "Candy From A Baby"/*, sf::Style::Fullscreen*/), 
	m_assetmanager(), m_gui(m_window), m_player(m_assetmanager, m_gui), m_levelmanager(m_window, m_assetmanager, m_player, m_gui)
{

	// begin load sequence
	m_assetmanager.load();
	
	m_window.setVerticalSyncEnabled(true);
	m_deltaclock.restart();

	CANDY_ASSERT(m_menu.openFromFile("./assets/soundtrack/menu.mp3"), "failed to load external resource ./assets/soundtrack/menu.mp3");
	CANDY_ASSERT(m_game_music.openFromFile("./assets/soundtrack/main.mp3"), "failed to load external resource ./assets/soundtrack/main.mp3");
}

Game::~Game() {

}

void Game::mainloop() {

	// mainloop
	while(m_window.isOpen())
	{
		// evaluate deltatime
		m_deltatime = m_deltaclock.restart();
		m_dt = m_deltatime.asSeconds();

		// event loop
		while(m_window.pollEvent(m_event)) {
			manageEvent();
		}

		// wait for loading to finish
		if(m_assetmanager.getStatus() == AssetManager::status_done) {

			// inform sub classes
			m_gui.setState(Gui::state_mainmenu);
			m_player.ready();

			// cleanup
			m_assetmanager.cleanup();
			m_menu.play();

		} else if(m_assetmanager.getStatus() == AssetManager::status_failed) {

			// setup error message
			std::string msg("Failed to load external asset ");
			msg += m_assetmanager.getFail();

			CANDY_ASSERT(false, msg.c_str());
		}

		// draw sequence
		m_window.clear();


		// check for level start from gui
		if(m_gui.getState() == Gui::state_level || m_gui.getState() == Gui::state_lost || m_gui.getState() == Gui::state_waiting_levelload) {
			
			// load level
			if(!m_levelmanager.loaded((LevelManager::Level)m_gui.getLevelId()) || ((m_gui.won() || m_gui.lost()) && m_gui.readyStart()) || m_gui.cpause()) {
				m_levelmanager.loadLevel((LevelManager::Level)m_gui.getLevelId());
				m_gui.resetStartState();
				
				if(m_game_music.getStatus() != sf::Music::Playing) {
					m_game_music.play();
					m_menu.pause();
				}
			}

			m_player.update(m_dt);
			m_levelmanager.update(m_dt);
		}

		// update
		if(m_gui.getState() == Gui::state_level || m_gui.getState() == Gui::state_pause || m_gui.getState() == Gui::state_lost) {
			m_player.draw();
			m_levelmanager.draw();
		} else {
			if(m_menu.getStatus() == sf::Music::Paused) {
				m_game_music.pause();
				m_menu.play();
			}
		}

		m_gui.update(m_deltatime);

		m_window.display();
	}
}

void Game::manageEvent() {

	m_gui.handleEvent(m_event);

	switch (m_event.type)
	{
	case sf::Event::Closed:
		m_window.close();
		break;
	
	default:
		break;
	}
}