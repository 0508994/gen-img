#include "ImgOps.h"

template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	return v <= lo ? lo : (v >= hi ? hi : v);
}

namespace gir
{
	void ToGrayscale(const sf::Image& inRgba, Mat<sf::Uint8>& outGray)
	{
		const sf::Uint8* pByteBuffer = inRgba.getPixelsPtr();
		auto sfSize = inRgba.getSize();
		
		size_t numPixels = sfSize.x * sfSize.y;

		for (int i = 0; i < numPixels; ++i)
		{
			float red = pByteBuffer[4 * i] * 0.299f;
			float green = pByteBuffer[4 * i + 1] * 0.587;
			float blue = pByteBuffer[4 * i + 2] * 0.144;
			//sf::Uint8 alpha = pByteBuffer[4 * i + 3];

			float gray = std::floor((red + green + blue + 0.5));
			gray = clamp(gray, 0.0f, 255.0f);

			outGray[0][i] = static_cast<sf::Uint8>(gray);
		}
	}

	void ToSFMLImage(const Mat<sf::Uint8>& inGray, sf::Image& outRgba)
	{
		for (int y = 0; y < inGray.Rows(); y++)
			for (int x = 0; x < inGray.Cols(); x++)
			{
				sf::Uint8 g = inGray[y][x];
				outRgba.setPixel(x, y, sf::Color(g, g, g));
			}
	}
}
