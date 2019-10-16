#pragma once
#include "Mat.h"
#include "SFML/Graphics/Image.hpp"


namespace gir
{

	Mat<sf::Uint8> GrayscaleFromSFMLImage(sf::Image& img);
}