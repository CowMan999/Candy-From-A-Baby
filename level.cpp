#include "level.hpp"

LevelManager::LevelManager(sf::RenderWindow& window, AssetManager& assets, Player& player, Gui& gui) : m_window(window), m_assets(assets), m_player(player), m_gui(gui) {
	m_view = m_window.getDefaultView();
	m_view.setCenter(m_player.getPos());
}

LevelManager::~LevelManager() {
	for(size_t i = 0; i < m_sprites.size(); i++)
		delete m_sprites[i];

	m_sprites.clear();
}

void LevelManager::loadLevel(Level id) {

	std::string name = "./assets/levels/"+std::to_string((int)id)+".dat";
	std::ifstream file(name);
	CANDY_ASSERT(file.is_open(), "Could not access external asset "+name);

	// clear out vectors, reset level
	m_player.reset();
	m_view.setCenter(m_player.getPos());
	m_gui.resetLevel();
	m_colision_map.clear();
	
	for(size_t i = 0; i < m_enemies.size(); i++)
		delete m_enemies[i];
	
	for(size_t i = 0; i < m_enemies.size(); i++)
		delete m_sprites[i];

	m_enemies.clear();
	m_sprites.clear();
	
	m_win = sf::FloatRect(0,0,0,0);

	std::string line;
	while(std::getline(file, line)) {
		if(line == "") continue;

		std::stringstream stream(line);

		char t;
		stream >> t;
		if(t == 'x') {
			long long int x, y, type;
			stream >> x >> y >> type;

			sf::Sprite* tile = new sf::Sprite();
			tile->setScale(2, 2);
			tile->setPosition(x*TILE_SIZE, y*TILE_SIZE);
			tile->setTexture(m_assets.getTexture(AssetManager::texture_enviroment));
			tile->setTextureRect(m_assets.getAnimation((AssetManager::Animation)type)[0]);

			m_sprites.push_back(tile);

			// wining tile
			if((AssetManager::Animation)type == AssetManager::anim_enviroment_staircase || (AssetManager::Animation)type == AssetManager::anim_enviroment_staircase_red || 
				(AssetManager::Animation)type == AssetManager::anim_enviroment_staircase_blue || (AssetManager::Animation)type == AssetManager::anim_enviroment_candy)
			{
				m_win = tile->getGlobalBounds();
			} else {
				m_colision_map.push_back(tile->getGlobalBounds());
			}
		}

		if(t == 'r') {
			long long int x, y, x2, y2, type;
			stream >> x >> y >> x2 >> y2 >> type;

			for (long long int i = x; i <= x2; i++)
			{
				for (long long int j = y; j <= y2; j++)
				{
					sf::Sprite* tile = new sf::Sprite();
					tile->setScale(2, 2);
					tile->setPosition(i*TILE_SIZE, j*TILE_SIZE);
					tile->setTexture(m_assets.getTexture(AssetManager::texture_enviroment));
					tile->setTextureRect(m_assets.getAnimation((AssetManager::Animation)type)[0]);
					m_sprites.push_back(tile);
				}
			}
			
			m_colision_map.push_back(sf::FloatRect(x*TILE_SIZE, y*TILE_SIZE, (x2-(x-1))*TILE_SIZE, (y2-(y-1))*TILE_SIZE));
		}

		if(t == 'h') {
			long long int x, y, x2, y2, type;
			stream >> x >> y >> x2 >> y2 >> type;

			for (long long int i = x; i <= x2; i++)
			{
				for (long long int j = y; j <= y2; j++)
				{
					if((i != x && i != x2) && (j != y && j != y2)) { // not on initial/final collumn or top/bottom, skip
						continue;
					}

					sf::Sprite* tile = new sf::Sprite();
					tile->setScale(2, 2);
					tile->setPosition(i*TILE_SIZE, j*TILE_SIZE);
					tile->setTexture(m_assets.getTexture(AssetManager::texture_enviroment));
					tile->setTextureRect(m_assets.getAnimation((AssetManager::Animation)type)[0]);
					m_sprites.push_back(tile);
				}
			}
			
			// add hollow box to collision maps
			m_colision_map.push_back(sf::FloatRect(x*TILE_SIZE, y*TILE_SIZE, (x2-x)*TILE_SIZE, TILE_SIZE));
			m_colision_map.push_back(sf::FloatRect(x*TILE_SIZE, (y+1)*TILE_SIZE, TILE_SIZE, ((y2-y)-1)*TILE_SIZE)); // somehow, this eyeballed bullshit works (bet there is overlap tho -_-)
			m_colision_map.push_back(sf::FloatRect((x2*TILE_SIZE), y*TILE_SIZE, TILE_SIZE, (y2-y)*TILE_SIZE));
			m_colision_map.push_back(sf::FloatRect(x*TILE_SIZE, (y2*TILE_SIZE), (x2-x)*TILE_SIZE + TILE_SIZE, TILE_SIZE));

		}

		if(t == 'e') {
			float x, y;
			long long int type;
			stream >> x >> y >> type;

			if(type == Enemy::enemy_camera) {

				// handle custom paramaters
				float dir, speed;
				unsigned int mag;

				stream >> dir >> speed >> mag;
				m_enemies.push_back(new Camera(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE), m_player, m_gui, m_assets));
				((Camera*)m_enemies[m_enemies.size()-1])->params(dir, speed, mag);
			}

			else if(type == Enemy::enemy_laser) {

				// handle custom paramaters
				bool hori;
				float speed;
				sf::Vector2f pos2;

				stream >> hori >> speed >> pos2.x >> pos2.y;
				pos2 *= (float)TILE_SIZE;
				m_enemies.push_back(new Laser(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE), m_player, m_gui, m_assets));
				((Laser*)m_enemies[m_enemies.size()-1])->params(hori, speed, pos2);
			}

			else if(type == Enemy::enemy_robot) {

				// handle custom paramaters
				float speed, rot_speed, range, acc, dir;

				stream >> speed >> rot_speed >> acc >> range >> dir;
				range *= (float)TILE_SIZE;
				m_enemies.push_back(new Robot(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE), m_player, m_gui, m_assets));
				((Robot*)m_enemies[m_enemies.size()-1])->params(speed, rot_speed, acc, range, dir);
			}

			else if(type == Enemy::enemy_baby) {

				// handle custom paramaters
				float speed, rot_speed, range, acc;

				stream >> speed >> rot_speed >> acc >> range;
				range *= (float)TILE_SIZE;
				m_enemies.push_back(new Baby(sf::Vector2f(x*TILE_SIZE,y*TILE_SIZE), m_player, m_gui, m_assets));
				((Baby*)m_enemies[m_enemies.size()-1])->params(speed, rot_speed, acc, range);
			}

		}

	}

	// file loaded, give player collision map location
	m_player.setColisionMap(m_colision_map, m_win);
	m_loaded = id;
}

void LevelManager::update(float dt) {

	// player won, freeze robots and baby
	if(m_gui.won()) {
		for (size_t i = 0; i < m_enemies.size(); i++)
		{
			if(m_enemies[i]->getType() == Enemy::enemy_robot) {
				((Robot*)m_enemies[i])->params(0, 0, 0, 0, 0);
			}

			else if(m_enemies[i]->getType() == Enemy::enemy_baby) {
				((Baby*)m_enemies[i])->over();
			}
		}
	}

	if(m_gui.getState() == Gui::state_waiting_levelload) {
		m_loaded = level_count;
		m_gui.setState(Gui::state_level);
	}

	// adjust cam position based on player's
	sf::Vector2f distance = m_player.getPos() - m_view.getCenter();

	if(distance.x > 15 || distance.x < -15)
		m_view.setCenter(sf::Vector2f((m_view.getCenter() + normalize(m_player.getPos() - m_view.getCenter()) * (float)CAM_SPEED*abs(distance.x)*dt).x, m_view.getCenter().y));
	if(distance.y > 15 || distance.y < -15)
		m_view.setCenter(sf::Vector2f(m_view.getCenter().x, (m_view.getCenter() + normalize(m_player.getPos() - m_view.getCenter()) * (float)CAM_SPEED*abs(distance.y)*dt).y));

	// update enemies
	for(size_t i = 0; i < m_enemies.size(); i++)
		m_enemies[i]->update(dt);
	

	m_window.setView(m_view);
}

void LevelManager::draw() {
	auto& view = m_window.getView();
	sf::FloatRect rect(view.getCenter()-view.getSize()/2.f, view.getSize());

	for(size_t i = 0; i < m_sprites.size(); i++)
	{
		if(rect.intersects(m_sprites[i]->getGlobalBounds())) {
			m_window.draw(*m_sprites[i]);
		}
	}

	// draw enemies
	for(size_t i = 0; i < m_enemies.size(); i++)
	{
		if(rect.intersects(m_enemies[i]->getSprite().getGlobalBounds())) {
			m_enemies[i]->draw();
		}
	}
	
}

bool LevelManager::loaded(Level l) {
	return m_loaded == l;
}