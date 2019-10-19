#pragma once
#include <cmath>
#include "Mat.h"
#include "SFML/Graphics/Image.hpp"
#include "Kernels.h"

namespace gir
{
	using sf::Uint8;

	template<class T>
	constexpr const T& clamp(const T& v, const T& lo, const T& hi)
	{
		return v <= lo ? lo : (v >= hi ? hi : v);
	}

	inline int circular(const int& M, const int& x)
	{
		return x < 0 ? (x + M) : (x >= M ? (x - M) : x);
	}

	void ToGrayscale(const sf::Image& inRgba, Mat<Uint8>& outGray);
	void ToSFMLImage(const Mat<Uint8>& inGray, sf::Image& outRgba);
	void Convolution(const Mat<Uint8>& src, Mat<float>& dst, const Kernel<int, 3, 3>& kernel );
	void Magnitude(const Mat<float>& gx, const Mat<float>& gy, Mat<Uint8>& result);
	void Sobel(const Mat<Uint8>& src, Mat<Uint8>& dst);
	void Prewitt(const Mat<Uint8>& src, Mat<Uint8>& dst);
	void Scharr(const Mat<Uint8>& src, Mat<Uint8>& dst);

}