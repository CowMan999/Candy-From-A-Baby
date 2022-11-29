#pragma once
#include <exception>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#define CANDY_ASSERT(x, msg) if(!x) throw std::runtime_error(msg)
inline const unsigned int DUR_MAX = 135;
sf::Vector2f normalize(const sf::Vector2f& source);