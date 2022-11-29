#include "gui.hpp"

void Gui::loading() {
	ImGui::Begin("##mainwindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(m_window.getSize(), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(0,0));
	
	// title
	ImGui::SetWindowFontScale(0.65f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	ImGui::SetCursorPosY((ImGui::GetWindowHeight()-ImGui::CalcTextSize("Loading...").y)*.5f);
	centeredText("Loading...");
	ImGui::PopStyleColor();
	
	ImGui::End();
}

void Gui::mainmenu() {
	ImGui::Begin("##mainwindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(m_window.getSize(), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(0,0));
	
	// title
	ImGui::SetWindowFontScale(0.55f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	centeredText("Like taking candy from a baby");
	ImGui::PopStyleColor();
	ImGui::Separator();
	
	centerByWidth(300);
	ImGui::SetCursorPosY((ImGui::GetWindowHeight()-(350+ImGui::GetStyle().ItemSpacing.y))/2.f);
	std::vector<bool> val = centeredButtonRow({"Play", "Quit"}, {ImVec2(300, 125), ImVec2(300, 125)}); // main button row
	centerByWidth(600+ImGui::GetStyle().ItemSpacing.x);
	bool how = ImGui::Button("How To Play", ImVec2(600+ImGui::GetStyle().ItemSpacing.x, 125));
	if(val[0]) {
		m_state = state_levelselect;
	} else if(val[1]) {
		m_window.close();
	} else if(how) {
		m_state = state_how;
	}

	ImGui::End();
}

void Gui::levelselect() {

	if(m_won || m_lost) {
		m_readystart = true;
	}

	ImGui::Begin("##mainwindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(m_window.getSize(), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(0,0));
	
	// title
	ImGui::SetWindowFontScale(0.55f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	centeredText("Like taking candy from a baby");
	ImGui::PopStyleColor();
	ImGui::Separator();
	
	ImGui::SetCursorPosY(ImGui::GetWindowHeight()/4.5f);
	ImGui::SetWindowFontScale(0.75f);
	centeredText("Level select");
	ImGui::SetWindowFontScale(0.45f);
	std::vector<bool> val = centeredButtonRow({"Level 1", "Level 2", "Level 3"}, {ImVec2(300,95), ImVec2(300,95), ImVec2(300,95)});


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.25f, 0.25f, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.35f, 0.35f, 1)); // complete dogshit button style code
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.95f, 0.25f, 0.25f, 1));
	std::vector<bool> b = centeredButtonRow({"Level 4", "Level 5", "Level 6"}, {ImVec2(300,95), ImVec2(300,95), ImVec2(300,95)});
	ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.65f, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.75f, 1)); // complete dogshit copypaste
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.95f, 1));
	centerByWidth(900+ImGui::GetStyle().ItemSpacing.x*2);
	bool lev7 = ImGui::Button("Level 7", ImVec2(300,95));
	ImGui::SameLine();
	bool lev8 = ImGui::Button("Level 8", ImVec2(300,95));
	ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();

	if(m_save.won) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.15f, 0.15f, 1)); // complete dogshit optional style
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1));

	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1)); // complete dogshi~ ok this is the last time I swear
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.65f, 0.65f, 0.65f, 1));
	}

	ImGui::SameLine();
	bool lev9 = ImGui::Button(m_save.won ? "Level 9" : "Locked", ImVec2(300,95));
	ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();


	val.insert(val.end(), b.begin(), b.end());
	val.push_back(lev7); val.push_back(lev8); 
	if(m_save.won) val.push_back(lev9);

	centerByWidth(300);
	if(ImGui::Button("Back", ImVec2(300,0))) {
		m_state = state_mainmenu;
	}

	ImGui::SetCursorPosY(ImGui::GetWindowHeight()/1.35f);
	centerByWidth(500);
	if(ImGui::Button("Best Times", ImVec2(500,0))) {
		m_state = state_best;
	}


	size_t id = -1;
	for (size_t i = 0; i < val.size(); i++) {
		if(val[i]) {
			id = i;
			break;
		}
	}

	if(id != static_cast<size_t>(-1)) {
		m_level_id = id;
		m_state = state_level;
	}

	ImGui::End();
}

void Gui::pause() {

	if(m_paused == false) {
		m_paused = true;
	}

	ImGui::Begin("##pausewindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(m_window.getSize()-sf::Vector2u(400,400), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(200,200));

	// title
	ImGui::SetWindowFontScale(0.55f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	centeredText("Paused");
	ImGui::PopStyleColor();
	ImGui::Separator();

	ImGui::SetCursorPosY(ImGui::GetWindowHeight()/2.5f);
	std::vector<bool> b = centeredButtonRow({"menu", "resume", "quit"}, {ImVec2(250,125), ImVec2(425,125), ImVec2(250,125)});
	centerByWidth(425);
	bool x = ImGui::Button("restart", ImVec2(425, 125));

	if(b[0]) {
		m_state = state_levelselect;
	} else if(b[1]) {
		m_paused = false;
		m_state = state_level;
	} else if(b[2]) {
		m_window.close();
	} else if(x) {
		m_state = state_waiting_levelload;
	}


	ImGui::End();
}

void Gui::level() {
	ImGui::Begin("##levelwindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(sf::Vector2u(500, 300), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(15,15));

	// title
	ImGui::SetWindowFontScale(0.35f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x-ImGui::CalcTextSize("Level 0").x)/2);
	ImGui::Text("Level %llu", m_level_id+1);
	ImGui::Separator();

	ImGui::Text("Time: %.2f", m_level_timer.asSeconds());

	centerByWidth(450);

	
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.85f, 0.15f, 0.15f, 0.75f));
	ImGui::ProgressBar(m_player_exposure, ImVec2(450, 0), "Attention");
	ImGui::PopStyleColor();


	ImGui::PopStyleColor();

	ImGui::End();
}

void Gui::best() {
	ImGui::Begin("##mainwindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(m_window.getSize(), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(0,0));
	
	// title
	ImGui::SetWindowFontScale(0.55f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	centeredText("Like taking candy from a baby");
	ImGui::PopStyleColor();
	ImGui::Separator();
	
	ImGui::SetCursorPosY(ImGui::GetWindowHeight()/4.5f);
	ImGui::SetWindowFontScale(0.85f);
	centeredText("Best Scores");
	ImGui::SetWindowFontScale(0.45f);

	centerByWidth(900);
	if(ImGui::BeginChild("##scores", ImVec2(900, 400), true)) {

		ImGui::Text("Level 1: %.2f", m_save.time_level[0]);
		ImGui::Text("Level 2: %.2f", m_save.time_level[1]);
		ImGui::Text("Level 3: %.2f", m_save.time_level[2]);
		ImGui::Text("Level 4: %.2f", m_save.time_level[3]);
		ImGui::Text("Level 5: %.2f", m_save.time_level[4]);
		ImGui::Text("Level 6: %.2f", m_save.time_level[5]);
		ImGui::Text("Level 7: %.2f", m_save.time_level[6]);
		ImGui::Text("Level 8: %.2f", m_save.time_level[7]);
		ImGui::Text("Total: %.2f/%d", m_save.total, DUR_MAX);

		if(m_save.won) {
			ImGui::Text("Level 9: %.4f", m_save.time_level[8]);
		}

	} 
	ImGui::EndChild();

	centerByWidth(300);
	if(ImGui::Button("Back", ImVec2(300,0))) {
		m_state = state_levelselect;
	}

	ImGui::End();
}

void Gui::mlost() {
	ImGui::Begin("##lsoewindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(m_window.getSize()-sf::Vector2u(400,400), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(200,200));

	// title
	ImGui::SetWindowFontScale(0.55f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	centeredText("You Lost!");
	ImGui::PopStyleColor();
	ImGui::Separator();

	ImGui::SetCursorPosY(ImGui::GetWindowHeight()/2.5f);
	std::vector<bool> b = centeredButtonRow({"menu", "retry", "quit"}, {ImVec2(250,125), ImVec2(425,125), ImVec2(250,125)});

	if(b[0]) {
		m_state = state_levelselect;
	} else if(b[1] || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::R)) && m_window.hasFocus())) {
		m_state = state_waiting_levelload;
	} else if(b[2]) {
		m_window.close();
	}


	ImGui::End();
}

void Gui::mwon() {
	m_needsave = 1;
	ImGui::Begin("##pwonwinm_needsavedow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(m_window.getSize()-sf::Vector2u(400,400), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(200,200));

	// title
	ImGui::SetWindowFontScale(0.55f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	centeredText("You Won!");
	ImGui::Separator();
	ImGui::PopStyleColor();

	centerByWidth(600);
	if(ImGui::BeginChild("##scores", ImVec2(600, 300), true)) {

		ImGui::Text("Level %llu", m_level_id+1);
		ImGui::Text("Time: %.2f", m_level_timer.asSeconds());

		if(m_level_id == 8) {
			ImGui::SetWindowFontScale(0.55f);
			ImGui::PushFont(m_font2);
			ImGui::Separator();
			ImGui::TextWrapped("Congrats on the epic victory! Stealling that candy wasn't as easy as they make it out to be now, was it? If you made it this far, you might be up to the challenge of playtesting my other games, hit me up on discord at CowMan9999#6717.");
			ImGui::PopFont();
			ImGui::SetWindowFontScale(0.55f);
		}
	}
	ImGui::EndChild();

	std::vector<bool> b = centeredButtonRow({"menu", "quit"}, {ImVec2(250,125), ImVec2(250,125)});

	if(b[0] || b[1]) {
		if(b[0]) {
			resetLevel();
			m_state = state_levelselect;
		} else if(b[1]) {
			resetLevel();
			m_window.close();
		}
	}

	ImGui::End();
}

void Gui::howtoplay() {
	ImGui::Begin("##mainwindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowSize(m_window.getSize(), ImGuiCond_Once);
	ImGui::SetWindowPos(ImVec2(0,0));
	
	// title
	ImGui::SetWindowFontScale(0.55f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 1, 1));
	centeredText("Like taking candy from a baby");
	ImGui::PopStyleColor();
	ImGui::Separator();
	
	// menu name
	ImGui::SetCursorPosY(ImGui::GetWindowHeight()/4.5f);
	ImGui::SetWindowFontScale(0.75f);
	centeredText("How to play");
	ImGui::SetWindowFontScale(0.45f);

	centerByWidth(900);
	if(ImGui::BeginChild("##info", ImVec2(900, 400), true)) {
		
		ImGui::SetWindowFontScale(1);
		ImGui::PushFont(m_font2);
		ImGui::TextWrapped("Use the WASD/arrow keys to move the player.\n\nThe goal is to reach the staircase at the end of any level, and not to get caught by the security systems.\n\nThe attention bar (top left) fills up while cameras and lasers can see you, and if it fills fully, you will lose the level.\n\nIt is also important to be quick, as to unlock level 9 you need to beat all other levels under the quota specified in the best times menu.\n\nSee if you can speed through all the levels and steal that candy!\n\nIf you experience any errors, check the log file at {game directory}/data/log, or contact me on discord at CowMan9999#6717.");
		ImGui::PopFont();
		ImGui::SetWindowFontScale(0.45f);
	} 
	ImGui::EndChild();

	centerByWidth(300);
	if(ImGui::Button("Back", ImVec2(300,0))) {
		m_state = state_mainmenu;
	}
	ImGui::End();
}