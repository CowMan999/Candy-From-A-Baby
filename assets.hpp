#include <SFML/Graphics.hpp>
#include <sfcollider.hpp>
#include <SFML/Audio.hpp>

#include <fstream>
#include <sstream>
#include <thread>

#include "utils.hpp"

#pragma once

class AssetManager {

public:
	AssetManager();
	~AssetManager();

	// async load function
	void load();
	void cleanup();

	enum Status {
		status_none = -1,
		status_loading,
		status_done,
		status_failed,
	};

	enum Texture {
		texture_player,
		texture_enviroment,
		texture_enemies,

		texture_count
	};

	enum Animation {
		anim_enviroment_brick,
		anim_enviroment_metal_vertical,
		anim_enviroment_metal_horizontal,
		anim_enviroment_staircase,
		anim_enviroment_brick_red,
		anim_enviroment_metal_vertical_red,
		anim_enviroment_metal_horizontal_red,
		anim_enviroment_staircase_red,
		anim_enviroment_metal_corner_red,
		anim_enviroment_brick_blue,
		anim_enviroment_metal_vertical_blue,
		anim_enviroment_metal_horizontal_blue,
		anim_enviroment_staircase_blue,
		anim_enviroment_metal_corner_blue,
		anim_enviroment_candy,

		anim_player_idle,
		anim_player_runleft,
		anim_player_runright,
		anim_player_runup,
		anim_player_rundown,
		anim_player_dead,

		anim_enemy_camera,
		anim_enemy_laser_pass_vert,
		anim_enemy_laser_pass_hori,
		anim_enemy_laser_agro_vert,
		anim_enemy_laser_agro_hori,
		anim_enemy_robot,
		anim_enemy_baby,

		anim_count
	};

	// get status to check when all textures are loaded
	Status getStatus();
	// error handling
	std::string getFail();

	// get textures/animations
	sf::Texture& getTexture(Texture tex);
	std::vector<sf::IntRect>& getAnimation(Animation anim);

private:
	bool loadAnimation(std::vector<sf::IntRect>& out, const char* name);

	std::array<sf::Texture, texture_count> m_textures;
	std::array<const char*, texture_count> m_texture_names;

	std::array<std::vector<sf::IntRect>, anim_count> m_animations;
	std::array<const char*, anim_count> m_animation_names;

	Status m_status = status_none;
	size_t m_id_fail_tex = -1;
	size_t m_id_fail_anim = -1;

	std::thread* m_worker;
};