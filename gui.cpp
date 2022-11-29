#include "gui.hpp"

#define next << ' ' <<

Gui::Gui(sf::RenderWindow& window) : m_window(window) {

	// init imgui
	CANDY_ASSERT(ImGui::SFML::Init(m_window, false), "Failed to init imgui-sfml");

	// configure
	auto& io = ImGui::GetIO();
	io.LogFilename = nullptr;
	io.IniFilename = nullptr;

	// style
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.65f, 0.65f, 1, 0.65f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.9f, 0.9f, 1, 0.9f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.75f, 0.75f, 1, 0.75f);

	// load font
	m_font = io.Fonts->AddFontFromMemoryCompressedTTF(font_compressed_data, font_compressed_size, 128);
	m_font2 = io.Fonts->AddFontFromMemoryCompressedTTF(font2_compressed_data, font2_compressed_size, 96);
	CANDY_ASSERT(ImGui::SFML::UpdateFontTexture(), "Couldn't init font");

	// load save data
	if(!std::filesystem::exists("./data/save")) {
		std::ofstream file("./data/save");

		CANDY_ASSERT(file.is_open(), "Failed to create save at ./data/save");

		file << "0 0 0 0 0 0 0 0 0 0";

		file.close();

	} else {

		std::ifstream file("./data/save");
		CANDY_ASSERT(file.is_open(), "Failed to open save at ./data/save");

		file >> m_save.time_level[0] >> m_save.time_level[1] >> m_save.time_level[2] >> m_save.time_level[3] >> 
			m_save.time_level[4] >> m_save.time_level[5] >> m_save.time_level[6] >> 
			m_save.time_level[7] >> m_save.time_level[8] >> m_save.won;

		m_save.total = m_save.time_level[0] + m_save.time_level[1] + m_save.time_level[2] + m_save.time_level[3] + 
			m_save.time_level[4] + m_save.time_level[5] + m_save.time_level[6] + m_save.time_level[7];

		file.close();
	}


}

Gui::~Gui() {

}

void Gui::update(sf::Time dt) {
	ImGui::SFML::Update(m_window, dt);

	if(!m_lost) {
		if((!m_window.hasFocus() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) && m_state == state_level)
			m_state = state_pause; 

		if(m_state == state_level && !m_won) {
			m_level_timer += dt;
		}
	}

	if(m_state == state_loading) {
		loading();
	} else if(m_state == state_mainmenu) {
		mainmenu();
	} else if(m_state == state_levelselect) {
		levelselect();
	} else if(m_state == state_pause) {
		pause();
	} else if(m_state == state_level) {
		level();
	} else if(m_state == state_best) {
		best();
	} else if(m_state == state_lost) {
		mlost();
	} else if(m_state == state_win) {
		mwon();
	} else if(m_state == state_how) {
		howtoplay();
	}

	if(m_state == state_level && !m_lost) {
		m_player_exposure -= dt.asSeconds()/4.5f;
		if(m_player_exposure < 0) {
			m_player_exposure = 0;
		}
	}

	ImGui::SFML::Render(m_window);
}

void Gui::handleEvent(sf::Event& event) {
	ImGui::SFML::ProcessEvent(event);
}

Gui::State Gui::getState() {
	return m_state;
}

void Gui::setState(Gui::State s) {
	m_state = s;
}

void Gui::centeredText(const char* text) {
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x-ImGui::CalcTextSize(text).x)/2);
	ImGui::Text(text);
}

std::vector<bool> Gui::centeredButtonRow(const std::vector<const char*> titles, const std::vector<ImVec2> sizes)
{
	float culm = 0;
	float winwidth = ImGui::GetWindowWidth();
	std::vector<bool> bitset;
	for (size_t i = 0; i < sizes.size(); i++)
		culm += sizes[i].x;
	ImGui::SetCursorPosX((winwidth-(culm+ImGui::GetStyle().ItemSpacing.x*(titles.size()-1)))*0.5f);
	for (size_t i = 0; i < titles.size(); i++)
	{
		bitset.push_back(ImGui::Button(titles[i], sizes[i]));
		if(i != titles.size()-1)
			ImGui::SameLine();
	}

	return bitset;
	
}

void Gui::centerByWidth(const float width)
{
	ImGui::SetCursorPosX((ImGui::GetWindowWidth()-width)*0.5f);
}

size_t Gui::getLevelId() {
	return m_level_id+1;
}

sf::RenderWindow& Gui::getWindow() {
	return m_window;
}

void Gui::viewingPlayer(float dur) {
	m_player_exposure += dur;
	if(m_player_exposure >= 1) {
		m_player_exposure = 1;
		m_lost = true;
	}
}

void Gui::resetLevel() {
	
	// save data to save struct, then to file
	if(m_won && m_needsave) {
		if(m_save.time_level[m_level_id] == 0 || m_save.time_level[m_level_id] > m_level_timer.asSeconds())
			m_save.time_level[m_level_id] = m_level_timer.asSeconds();
		
		m_save.total = m_save.time_level[0] + m_save.time_level[1] + m_save.time_level[2] + m_save.time_level[3] + 
			m_save.time_level[4] + m_save.time_level[5] + m_save.time_level[6] + m_save.time_level[7];

		bool over = 0;
		for(size_t i = 0; i < 8; i++) {
			if(m_save.time_level[i] == 0) {
				over = 1;
				break;
			}
		}
		

		if(m_save.total < DUR_MAX && !over) {
			m_save.won = true;
		}

		std::ofstream file("./data/save");
		CANDY_ASSERT(file.is_open(), "Failed to edit save at ./data/save");
		
		file << m_save.time_level[0] next m_save.time_level[1] next m_save.time_level[2] next m_save.time_level[3] next
			m_save.time_level[4] next m_save.time_level[5] next m_save.time_level[6] next m_save.time_level[7] next m_save.time_level[8] next m_save.won;

		file.close();
		m_needsave = 0;
	}

	m_player_exposure = 0;
	m_level_timer = sf::seconds(0);
}

bool Gui::lost() {
	return m_lost;
}

void Gui::Lwon() {
	m_won = 1;
}

bool Gui::won() {
	return m_won;
}

bool Gui::cpause() {
	return m_paused;
}

bool Gui::readyStart() {
	return m_readystart;
}

void Gui::resetStartState() {
	m_paused = false;
	m_won = 0;
	m_lost = 0;
	m_readystart = false;
}