#pragma once
#include <cassert>
#include <cmath>
#include "Mat.h"
#include "SFML/Graphics/Image.hpp"
#include "Kernels.h"

namespace gir
{
	template<class T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		return v <= lo ? lo : (v >= hi ? hi : v);
	}

	inline int circular(const int& M, const int& x)
	{
		return x < 0 ? (x + M) : (x >= M ? (x - M) : x);
	}

	void ToGrayscale(const sf::Image& inRgba, Mat<sf::Uint8>& outGray);
	void ToSFMLImage(const Mat<sf::Uint8>& inGray, sf::Image& outRgba);
	void Convolution(const Mat<sf::Uint8>& src, Mat<float>& dst, const Kernel& kernel );
	void Magnitude(const Mat<float>& gx, const Mat<float>& gy, Mat<sf::Uint8>& result);
	void Sobel(const Mat<sf::Uint8>& src, Mat<sf::Uint8>& dst);
	void Prewitt(const Mat<sf::Uint8>& src, Mat<sf::Uint8>& dst);
	void Scharr(const Mat<sf::Uint8>& src, Mat<sf::Uint8>& dst);

}