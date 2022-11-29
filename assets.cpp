#include "assets.hpp"

AssetManager::AssetManager() {
	
	// hardcoded texture names
	m_texture_names[texture_player] = "./assets/player/spritesheet.png";
	m_texture_names[texture_enviroment] = "./assets/enviroment/spritesheet.png";
	m_texture_names[texture_enemies] = "./assets/enemies/spritesheet.png";
	
	// hardcoded animation names
	m_animation_names[anim_player_idle] = "./assets/player/idle.dat";
	m_animation_names[anim_player_runleft] = "./assets/player/left.dat";
	m_animation_names[anim_player_runright] = "./assets/player/right.dat";
	m_animation_names[anim_player_rundown] = "./assets/player/down.dat";
	m_animation_names[anim_player_runup] = "./assets/player/up.dat";
	m_animation_names[anim_player_dead] = "./assets/player/dead.dat";

	m_animation_names[anim_enviroment_brick] = "./assets/enviroment/brick.dat";
	m_animation_names[anim_enviroment_metal_vertical] = "./assets/enviroment/metal_vertical.dat";
	m_animation_names[anim_enviroment_metal_horizontal] = "./assets/enviroment/metal_horizontal.dat";
	m_animation_names[anim_enviroment_staircase] = "./assets/enviroment/staircase.dat";
	m_animation_names[anim_enviroment_brick_red] = "./assets/enviroment/brick_red.dat";
	m_animation_names[anim_enviroment_metal_vertical_red] = "./assets/enviroment/metal_vertical_red.dat";
	m_animation_names[anim_enviroment_metal_horizontal_red] = "./assets/enviroment/metal_horizontal_red.dat";
	m_animation_names[anim_enviroment_staircase_red] = "./assets/enviroment/staircase_red.dat";
	m_animation_names[anim_enviroment_metal_corner_red] = "./assets/enviroment/metal_corner_red.dat";
	m_animation_names[anim_enviroment_brick_blue] = "./assets/enviroment/brick_blue.dat";
	m_animation_names[anim_enviroment_metal_vertical_blue] = "./assets/enviroment/metal_vertical_blue.dat";
	m_animation_names[anim_enviroment_metal_horizontal_blue] = "./assets/enviroment/metal_horizontal_blue.dat";
	m_animation_names[anim_enviroment_staircase_blue] = "./assets/enviroment/staircase_blue.dat";
	m_animation_names[anim_enviroment_metal_corner_blue] = "./assets/enviroment/metal_corner_blue.dat";
	m_animation_names[anim_enviroment_candy] = "./assets/enviroment/candy.dat";

	m_animation_names[anim_enemy_camera] = "./assets/enemies/camera.dat";
	m_animation_names[anim_enemy_laser_pass_vert] = "./assets/enemies/laserpv.dat";
	m_animation_names[anim_enemy_laser_pass_hori] = "./assets/enemies/laserph.dat";
	m_animation_names[anim_enemy_laser_agro_vert] = "./assets/enemies/laserav.dat";
	m_animation_names[anim_enemy_laser_agro_hori] = "./assets/enemies/laserah.dat";
	m_animation_names[anim_enemy_robot] = "./assets/enemies/robot.dat";
	m_animation_names[anim_enemy_baby] = "./assets/enemies/baby.dat";
}

AssetManager::~AssetManager() {

}

// async load with info given to outside
void AssetManager::load() {
	m_worker = new std::thread([this]() {
		m_status = status_loading;

		for (size_t i = 0; i < texture_count; i++)
		{
			if(i != texture_enemies && i != texture_player) {
				bool done = m_textures[i].loadFromFile(m_texture_names[i]);

				if(!done) {
					m_id_fail_tex = i;
					break;
				}
			} else {
				bool done = Collision::createTextureAndBitmask(m_textures[i], m_texture_names[i]);

				if(!done) {
					m_id_fail_tex = i;
					break;
				}
			}
		}

		for (size_t i = 0; i < anim_count; i++)
		{
			bool done = loadAnimation(m_animations[i], m_animation_names[i]);
			
			if(!done) {
				m_id_fail_anim = i;
				break;
			}
		}

		if(m_id_fail_tex == static_cast<size_t>(-1) && m_id_fail_anim == static_cast<size_t>(-1))
			m_status = status_done;
		else m_status = status_failed;
	});
}

AssetManager::Status AssetManager::getStatus() {
	return m_status;
}

void AssetManager::cleanup()
{
	m_worker->join();
	delete m_worker;
	m_status = status_none;
}

std::string AssetManager::getFail() {
	std::string msg;
	if(m_id_fail_tex != static_cast<size_t>(-1))
		msg = m_texture_names[m_id_fail_tex];
	else if(m_id_fail_anim != static_cast<size_t>(-1))
		msg = m_animation_names[m_id_fail_anim];

	return msg;
}

sf::Texture& AssetManager::getTexture(Texture tex) {
	return m_textures[tex];
}

std::vector<sf::IntRect>& AssetManager::getAnimation(Animation anim) {
	return m_animations[anim];
}

bool AssetManager::loadAnimation(std::vector<sf::IntRect>& out, const char* name) {
	try {
		std::ifstream file(name);

		CANDY_ASSERT(file.is_open(), "");

		std::string line;
		while(std::getline(file, line)) {
			std::stringstream stream(line);

			unsigned int x{}, y{}, w{}, h{};
			stream >> x >> y >> w >> h;
			out.push_back(sf::IntRect(x, y, w, h));
		}

	} catch(const std::exception& e) {
		return false;
	}
	return true;
}