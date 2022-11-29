#include "player.hpp"

Player::Player(AssetManager& assetmanager, Gui& gui) : m_assetmanager(assetmanager), m_gui(gui), m_window(m_gui.getWindow()) {
	m_sprite.setScale(2.f,2.f);
	m_sprite.setOrigin(16,16);
	m_sprite.setPosition(32,32);
}

Player::~Player() {

}

void Player::ready() {
	m_sprite.setTexture(m_assetmanager.getTexture(AssetManager::texture_player));

}

void Player::draw() {
	m_window.draw(m_sprite);
}

bool Player::colliding(sf::Sprite& contender) {
	return Collision::pixelPerfectTest(m_sprite, contender);
}

void Player::update(float dt) {
	
	m_animtimer += dt;

	if(!m_gui.lost()) {
		
		if(!m_gui.won()) {
			
			// update velocity based on inputs
			if(clickingW()) m_velocity.y -= PLAYER_ACCEL*dt;
			if(clickingA()) m_velocity.x -= PLAYER_ACCEL*dt;
			if(clickingS()) m_velocity.y += PLAYER_ACCEL*dt;
			if(clickingD()) m_velocity.x += PLAYER_ACCEL*dt;

		} else {
			m_wintimer += dt;
			if(m_wintimer > 1.75f && m_gui.getState()) {
				m_gui.setState(Gui::state_win);
			}
		}

		// update animation sequence
		AssetManager::Animation anim = AssetManager::anim_count;
		     if(m_velocity.x <= -PLAYER_ANIM_THRESHOLD) anim = AssetManager::anim_player_runleft;
		else if(m_velocity.x >= PLAYER_ANIM_THRESHOLD) anim = AssetManager::anim_player_runright;
		else if(m_velocity.y <= -PLAYER_ANIM_THRESHOLD) anim = AssetManager::anim_player_runup;
		else if(m_velocity.y >= PLAYER_ANIM_THRESHOLD) anim = AssetManager::anim_player_rundown;
		else anim = AssetManager::anim_player_idle;

		// set animation frame from sequence
		auto& animation = m_assetmanager.getAnimation(anim);
		int frame = std::round(m_animtimer/PLAYER_ANIM_SPEED);

		if((size_t)frame >= animation.size()) {
			m_animtimer = 0;
			frame = 0;
		}

		m_sprite.setTextureRect(animation[frame]);
	
	} else { // player lost, shitty death animation is a go
		m_sprite.setTextureRect(m_assetmanager.getAnimation(AssetManager::anim_player_dead)[0]);

		m_deadtimer += dt;

		if(m_deadtimer > 1 && m_gui.getState() != Gui::state_waiting_levelload) {
			m_gui.setState(Gui::state_lost);
		}
	}

	// decrease velocity based on deceleration value
	m_velocity -= m_velocity*PLAYER_DECEL*dt;

	// modify position based on velocity
	m_old = m_position;
	m_position += m_velocity*dt;

	// check collision
	collision();

	// update sprite position for future rendering
	m_sprite.setPosition(m_position);

}

bool Player::clickingW() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}
bool Player::clickingA() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
}
bool Player::clickingS() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
}
bool Player::clickingD() {
	return sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

sf::Vector2f Player::getPos() {
	return m_sprite.getPosition();
}

void Player::setColisionMap(std::vector<sf::FloatRect>& map, sf::FloatRect win) {
	m_collision_map = &map;
	m_wining_rect = win;
}

void Player::reset() {
	m_position = sf::Vector2f(32,32);
	m_velocity = sf::Vector2f(0,0);

	m_sprite.setPosition(m_position);
	m_animtimer = 0;
	m_deadtimer = 0;
	m_wintimer = 0;
}