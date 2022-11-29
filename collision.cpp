#include "player.hpp"

void Player::collision() {

	sf::Vector2f size = m_sprite.getGlobalBounds().getSize();
	sf::FloatRect hitbox(m_position - size/2.f, size); // define and adjust hitbox
	hitbox.left += 6; hitbox.top += 6; 
	hitbox.width -= 12; hitbox.height -= 12;

	// pre requisite variables
	sf::Vector2f ppos = m_old;
	sf::Vector2f papos = hitbox.getPosition() - (m_position-ppos);


	for(size_t i = 0; i < m_collision_map->size(); i++)
	{
		sf::FloatRect& rect = m_collision_map->at(i);
		sf::Vector2f pos = rect.getPosition() + rect.getSize()/2.f;

		// check collision
		if(hitbox.intersects(rect)) {

			bool within_vert = papos.y+hitbox.height > rect.top && papos.y < rect.top+rect.height;
			bool within_hori = papos.x+hitbox.width > rect.left && papos.x < rect.left+rect.width;

			//std::cout << papos.y+hitbox.height << " > " << rect.top << " && " << papos.y << " < " << rect.top+rect.height << '\n';
			//std::cout << papos.x+hitbox.width << " > " << rect.left << " && " << papos.x << " < " << rect.left+rect.width << "\n\n";

			// snap position to nearest wall
			if(ppos.x < pos.x && within_vert) { //to the left (all your things in a box to the left)
				m_position.x = pos.x - rect.width/2 - hitbox.width/2; m_velocity.x = 0;
			} 
			
			if(ppos.x > pos.x && within_vert) { // right
				m_position.x = pos.x + rect.width/2 + hitbox.width/2; m_velocity.x = 0;
			} 
			
			if(ppos.y < pos.y && within_hori) { // above
				m_position.y = pos.y - rect.height/2 - hitbox.height/2; m_velocity.y = 0;
			} 
			
			if(ppos.y > pos.y && within_hori) { // below
				m_position.y = pos.y + rect.height/2 + hitbox.height/2; m_velocity.y = 0;
			}

			// you need someone with a masters in cryptography to read that shit, send help
		}
	}
	
	if(hitbox.intersects(m_wining_rect)) {
		m_gui.Lwon();
	}
}