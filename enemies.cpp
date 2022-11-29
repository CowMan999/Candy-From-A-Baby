#include "enemy.hpp"
#include <dir_to_coords.hpp>

Camera::Camera(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets) : Enemy(position, player, gui, assets, enemy_camera) {
	m_sprite.setTexture(m_assets.getTexture(AssetManager::texture_enemies));
	m_sprite.setTextureRect(m_assets.getAnimation(AssetManager::anim_enemy_camera)[0]);

	m_sprite.setOrigin(sf::Vector2f(128, 32));
}

Camera::~Camera() {
	
}

void Camera::params(float dir, float speed, unsigned int mag) {
	m_dir = dir;
	m_speed = speed;
	m_mag = mag;
}

void Camera::update(float dt) {
	m_timer += dt*m_speed;

	m_sprite.setRotation(m_dir+sin(m_timer)*m_mag);

	// collision
	if(m_player.colliding(m_sprite)) {
		m_gui.viewingPlayer(CAMERA_DAMAGE*dt);

		if(!m_viewing) {
			m_sprite.setTextureRect(m_assets.getAnimation(AssetManager::anim_enemy_camera)[1]);
			m_viewing = true;
		}
	} else {
		if(m_viewing) {
			m_sprite.setTextureRect(m_assets.getAnimation(AssetManager::anim_enemy_camera)[0]);
			m_viewing = false;
		}
	}

	
}


Laser::Laser(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets) : Enemy(position, player, gui, assets, enemy_laser) {
	m_sprite.setTexture(m_assets.getTexture(AssetManager::texture_enemies));

	m_pos = position;
}

Laser::~Laser() {
	
}

void Laser::params(bool hori, float speed, sf::Vector2f pos2) {
	m_hori = hori;
	m_speed = speed;
	m_pos2 = pos2;

	if(m_hori)m_sprite.setOrigin(sf::Vector2f(0, 3));
	else m_sprite.setOrigin(sf::Vector2f(3, 0));
} 

void Laser::update(float dt) {
	m_timer += dt*m_speed;
	m_animtimer += dt;

	m_sprite.setPosition(m_pos + (m_pos2-m_pos)*(float)((sin(m_timer)+1)/2.f));

	// animation
	AssetManager::Animation anim;
	
	// select animation 
	if(m_hori) {
		if(m_viewing) {
			anim = AssetManager::anim_enemy_laser_agro_hori;
		} else {
			anim = AssetManager::anim_enemy_laser_pass_hori;
		}
	} else {
		if(m_viewing) {
			anim = AssetManager::anim_enemy_laser_agro_vert;
		} else {
			anim = AssetManager::anim_enemy_laser_pass_vert;
		}
	}
	
	// get animation
	std::vector<sf::IntRect>& animation = m_assets.getAnimation(anim);
	int frame = std::round(m_animtimer*LASER_ANIM_SPEED);

	if((size_t)frame >= animation.size()) {
		m_animtimer = 0;
		frame = 0;
	}

	m_sprite.setTextureRect(animation[frame]);

	// collision
	m_viewing = m_player.colliding(m_sprite);
	if(m_viewing) {
		m_gui.viewingPlayer(LASER_DAMAGE*dt);

	}
	
}

Robot::Robot(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets) : Enemy(position, player, gui, assets, enemy_robot),
	m_animation(m_assets.getAnimation(AssetManager::anim_enemy_robot))
{
	m_sprite.setTexture(m_assets.getTexture(AssetManager::texture_enemies));
	m_sprite.setOrigin(sf::Vector2f(32, 32));
}

Robot::~Robot() {

}

void Robot::params(float speed, float rot_speed, float acc, float range, float dir) {
	m_speed = speed;
	m_range = range;
	m_acc = acc;
	m_rot_speed = rot_speed;

	if(dir != 0) { 
		m_sprite.setRotation(dir);
		m_angle = dir;
		m_save_angle = dir;
	}
}

void Robot::update(float dt) {
	m_animtimer += dt;
	m_timer += dt;

	// check if player in range (pythagoras saved my ass)
	if(sqrt(pow(m_sprite.getPosition().x - m_player.getPos().x, 2) + pow(m_sprite.getPosition().y - m_player.getPos().y, 2)) < m_range) {
		
		if(!m_enraged) {
			m_enraged = true;

			// last frame is agro
			m_sprite.setTextureRect(m_animation[m_animation.size()-1]);
		}

		if(m_timer > m_acc) {

			// check dir to rotate
			m_save_angle = coords_to_dir(m_sprite.getPosition().x, m_sprite.getPosition().y, m_player.getPos().x, m_player.getPos().y);
			m_angle = m_sprite.getRotation();
			m_timer=0;
		}

		// rotate
		m_sprite.setRotation(lerp_angle(m_angle, m_save_angle, m_timer/m_acc));

		// move
		sf::Vector2f move(dir_to_coords_x(m_angle), dir_to_coords_y(m_angle));
		move *= ROBOT_BASE_SPEED * m_speed * dt;
		m_sprite.move(move);

	} else {
		// generic spin idle animation
		m_sprite.rotate(m_rot_speed*dt*m_speed);

		if(m_enraged) {
			m_enraged = false;
		}
	}

	if(!m_enraged) {

		// get animation
		int frame = std::round(m_animtimer*ROBOT_ANIM_SPEED);

		if((size_t)frame >= m_animation.size()-1) {
			m_animtimer = 0;
			frame = 0;
		}

		m_sprite.setTextureRect(m_animation[frame]);
	}


	// collision
	m_viewing = m_player.colliding(m_sprite);
	if(m_viewing) {
		m_gui.viewingPlayer(1 /* insta kill */);
	}
	
}

// baby, similar to robot
Baby::Baby(sf::Vector2f position, Player& player, Gui& gui, AssetManager& assets) : Enemy(position, player, gui, assets, enemy_baby),
	m_animation(m_assets.getAnimation(AssetManager::anim_enemy_baby))
{
	m_sprite.setTexture(m_assets.getTexture(AssetManager::texture_enemies));
	m_sprite.setOrigin(sf::Vector2f(16, 16));
	m_sprite.setTextureRect(m_animation[0]);
}

Baby::~Baby() {

}

void Baby::params(float speed, float rot_speed, float acc, float range) {
	m_speed = speed;
	m_range = range;
	m_acc = acc;
	m_rot_speed = rot_speed;
}

void Baby::update(float dt) {

	if(m_over) return;

	m_animtimer += dt;
	m_timer += dt;

	if(sqrt(pow(m_sprite.getPosition().x - m_player.getPos().x, 2) + pow(m_sprite.getPosition().y - m_player.getPos().y, 2)) < m_range) {
		
		if(!m_enraged) {
			m_enraged = true;

			// frame 1 is agro
			m_sprite.setTextureRect(m_animation[1]);
		}

		if(m_timer > m_acc) {

			// check dir to rotate
			m_angle = m_save_angle;
			m_save_angle = coords_to_dir(m_sprite.getPosition().x, m_sprite.getPosition().y, m_player.getPos().x, m_player.getPos().y);
			m_timer=0;
		}

		// rotate
		m_move_angle = lerp_angle(m_angle, m_save_angle, m_timer/m_acc);

		// move
		sf::Vector2f move(dir_to_coords_x(m_move_angle), dir_to_coords_y(m_move_angle));
		move *= ROBOT_BASE_SPEED * m_speed * dt;
		m_sprite.move(move);

	} else {
		if(m_enraged) {
			m_enraged = false;
			m_sprite.setTextureRect(m_animation[0]);
		}
	}

	m_viewing = m_player.colliding(m_sprite);
	if(m_viewing) {
		m_gui.viewingPlayer(1 /* insta kill */);
	}
	
}

void Baby::over() {
	if(!m_over) {
		m_sprite.setTextureRect(m_animation[2]); // cry animation
		m_over = true;
	}
}