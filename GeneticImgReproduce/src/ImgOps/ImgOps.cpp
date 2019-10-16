#include "ImgOps.h"


namespace gir
{
	void ToGrayscale(const sf::Image& inRgba, Mat<sf::Uint8>& outGray)
	{
		const sf::Uint8* pByteBuffer = inRgba.getPixelsPtr();
		auto sfSize = inRgba.getSize();
		
		int numPixels = sfSize.x * sfSize.y;

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

	void Convolution(const Mat<sf::Uint8>& src, Mat<float>& dst, const Kernel& kernel)
	{
		assert(kernel.size() == kernel[0].size() && kernel.size() % 2 != 0);

		float sum;
		int x1, y1;
		unsigned int cols = src.Cols();
		unsigned int rows = src.Rows();
		int kernelSize = kernel.size();				
		int kernelOffset = kernelSize / 2;

		for (int y = 0; y < rows; y++)
		{
			for (int x = 0; x < cols; x++)
			{
				sum = 0.0f;
				for (int k = -kernelOffset; k <= kernelOffset; k++)
				{
					for (int j = -kernelOffset; j < kernelOffset; j++)
					{
						x1 = circular(cols, x - j);
						y1 = circular(rows, y - k);
						sum += kernel[j + 1][k + 1] * src[y1][x1];
					}
				}

				//sum = clamp(sum, 0.0f, 255.0f);
				//dst[y][x] = static_cast<sf::Uint8>(sum);
				dst[y][x] = sum;
			}
		}
	}

	// https://dsp.stackexchange.com/questions/51726/is-it-possible-to-combine-two-sobel-kernels-into-one
	void Magnitude(const Mat<float>& gx, const Mat<float>& gy, Mat<sf::Uint8>& result)
	{
		unsigned int cols = result.Cols();
		unsigned int rows = result.Rows();
		
		for (int y = 0; y < rows; y++)
		{
			for (int x = 0; x < cols; x++)
			{
				float value = clamp(hypot((gx[y][x]), (gy[y][x])), 0.0f, 255.0f);

				//sf::Uint8 threshold = 70;
				//result[y][x] = std::min(static_cast<sf::Uint8>(value), threshold);
				result[y][x] = static_cast<sf::Uint8>(value);
			}
		}
	}

	static void SimpleEdge(const Mat<sf::Uint8>& src, Mat<sf::Uint8>& dst, const Kernel& kx, const Kernel& ky)
	{
		unsigned int cols = src.Cols();
		unsigned int rows = src.Rows();

		Mat<float> gx(rows, cols);
		Mat<float> gy(rows, cols);

		Convolution(src, gx, kx);
		Convolution(src, gy, ky);

		Magnitude(gx, gy, dst);
	}

	void Sobel(const Mat<sf::Uint8>& src, Mat<sf::Uint8>& dst)
	{
		SimpleEdge(src, dst, sobelx3, sobely3);
	}

	void Prewitt(const Mat<sf::Uint8>& src, Mat<sf::Uint8>& dst)
	{
		SimpleEdge(src, dst, prewittx3, prewitty3);
	}

	void Scharr(const Mat<sf::Uint8>& src, Mat<sf::Uint8>& dst)
	{
		SimpleEdge(src, dst, scharrx3, scharry3);
	}
}
