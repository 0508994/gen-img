#pragma once
#include "Mat.h"
#include "SFML/Graphics/Image.hpp"


namespace gir
{
	void ToGrayscale(const sf::Image& inRgba, Mat<sf::Uint8>& outGray);
	void ToSFMLImage(const Mat<sf::Uint8>& inGray, sf::Image& outRgba);
}