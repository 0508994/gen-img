#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <iostream>
#include <utility>
#include <queue>
#include "Mat.h"
#include "SFML/Graphics/Image.hpp"
#include "Kernels.h"

namespace gir
{
	using sf::Uint8;

	template<class T>
	constexpr const T& Clamp(const T& v, const T& lo, const T& hi)
	{
		return v <= lo ? lo : (v >= hi ? hi : v);
	}

	inline int Circular(const int& M, const int& x)
	{
		return x < 0 ? (x + M) : (x >= M ? (x - M) : x);
	}

	void ToGrayscale(const sf::Image& inRgba, Mat<Uint8>& outGray);
	void ToSFMLImage(const Mat<Uint8>& inGray, sf::Image& outRgba);
	void Sobel(const Mat<Uint8>& src, Mat<Uint8>& dst);
	void Prewitt(const Mat<Uint8>& src, Mat<Uint8>& dst);
	void Scharr(const Mat<Uint8>& src, Mat<Uint8>& dst);
	void Canny(const Mat<Uint8>& src, Mat<Uint8>& dst, float sigma, float tmin, float tmax);

}