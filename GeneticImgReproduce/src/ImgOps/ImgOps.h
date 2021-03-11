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
    using Line = std::pair<sf::Vector2f, sf::Vector2f>;
    using LineRef = std::pair<sf::Vector2f&, sf::Vector2f&>;
    using LineConstRef = std::pair<const sf::Vector2f&, const sf::Vector2f&>; 
    
    constexpr inline int Circular(const int& M, const int& x)
    {
        return x < 0 ? (x + M) : (x >= M ? (x - M) : x);
    }

    void ToGrayscale(const sf::Image& inRgba, Mat<Uint8>& outGray);
    void ToSFMLImage(const Mat<Uint8>& inGray, sf::Image& outRgba);
    void Sobel(const Mat<Uint8>& src, Mat<Uint8>& dst);
    void Prewitt(const Mat<Uint8>& src, Mat<Uint8>& dst);
    void Scharr(const Mat<Uint8>& src, Mat<Uint8>& dst);
    void Canny(const Mat<Uint8>& src, Mat<Uint8>& dst, float sigma, float tmin, float tmax);
    void Threshold(Mat<Uint8>& src, Uint8 value);

    void HoughTransform(const Mat<Uint8>& edges, Mat<std::size_t>& accumulator);
    std::vector<Line> HoughLines(const Mat<Uint8>& edges, std::size_t threshold);
}