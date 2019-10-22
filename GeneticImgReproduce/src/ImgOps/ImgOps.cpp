#include "ImgOps.h"


namespace gir
{
	using sf::Uint8;

	static void Convolution(const Mat<Uint8>& src, Mat<float>& dst, const Kernel<float, 3, 3>& kernel)
	{
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
					for (int j = -kernelOffset; j <= kernelOffset; j++)
					{
						x1 = Circular(cols, x - j);
						y1 = Circular(rows, y - k);
						sum += kernel[j + kernelOffset][k + kernelOffset] * src[y1][x1];
					}
				}

				dst[y][x] = sum;
			}
		}
	}

	static Kernel<float, 3, 3> Gaussian(float sigma = 0.5)
	{
		assert(sigma >= 0 && sigma <= 0.5);

		Kernel<float, 3, 3> gauss;

		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
			{
				gauss[y][x] = exp(-0.5 * (x * x + y * y) / (sigma * sigma))
					/ (2.0 * M_PI * sigma * sigma);
			}

		//for (int y = 0; y < 3; y++) {
		//	for (int x = 0; x < 3; x++)
		//		std::cout << gauss[y][x] << "\t";
		//	std::cout << std::endl;
		//}

		return gauss;
	}

	static void Hysteresis(const Mat<float>& nms, Mat<Uint8>& out, float tmin, float tmax)
	{
		int ni, nj;
		std::queue<std::pair<int, int>> edges;

		out.Value(0);

		for (int y = 1; y < nms.Rows() - 1; y++)
		{
			for (int x = 1; x < nms.Cols() - 1; x++)
			{
				if (nms[y][x] >= tmax && out[y][x] == 0)
				{
					out[y][x] = 255;
					edges.push(std::make_pair(y, x));

					while (!edges.empty())
					{
						auto point = edges.front();
						edges.pop();

						for (int i = -1; i <= 1; i++)
						{
							ni = point.first + i;

							for (int j = -1; j <= 1; j++)
							{
								nj = point.second + j;

								if (nms[ni][nj] >= tmin && out[ni][nj] == 0)
								{
									out[ni][nj] = 255;
									edges.push(std::make_pair(ni, nj));
								}
							}
						}
					}
				}
			}
		}
	}

	static void SimpleEdge(const Mat<Uint8>& src, Mat<Uint8>& dst, const Kernel<int, 3, 3>& kx, const Kernel<int, 3, 3>& ky)
	{
		float dx, dy;
		int x1, y1;
		unsigned int cols = src.Cols();
		unsigned int rows = src.Rows();
		int kernelSize = kx.size();
		int kernelOffset = kernelSize / 2;

		for (int y = 0; y < rows; y++)
		{
			for (int x = 0; x < cols; x++)
			{
				dx = 0.0f;
				dy = 0.0f;

				for (int k = -kernelOffset; k <= kernelOffset; k++)
				{
					for (int j = -kernelOffset; j <= kernelOffset; j++)
					{
						x1 = Circular(cols, x - j);
						y1 = Circular(rows, y - k);

						dx += kx[j + kernelOffset][k + kernelOffset] * src[y1][x1];
						dy += ky[j + kernelOffset][k + kernelOffset] * src[y1][x1];
					}
				}

				dst[y][x] = static_cast<Uint8>(Clamp(hypot(dx, dy), 0.0f, 255.0f));
				//dst[y][x] = dst[y][x] >= 70 ? 255 : 0;
			}
		}
	}


	void ToGrayscale(const sf::Image& inRgba, Mat<Uint8>& outGray)
	{
		const Uint8* pByteBuffer = inRgba.getPixelsPtr();
		auto sfSize = inRgba.getSize();
		
		int numPixels = sfSize.x * sfSize.y;

		for (int i = 0; i < numPixels; ++i)
		{
			float red = pByteBuffer[4 * i] * 0.299f;
			float green = pByteBuffer[4 * i + 1] * 0.587;
			float blue = pByteBuffer[4 * i + 2] * 0.144;
			//sf::Uint8 alpha = pByteBuffer[4 * i + 3];

			float gray = std::floor((red + green + blue + 0.5));
			gray = Clamp(gray, 0.0f, 255.0f);

			outGray[0][i] = static_cast<Uint8>(gray);
		}
	}

	void ToSFMLImage(const Mat<Uint8>& inGray, sf::Image& outRgba)
	{
		for (int y = 0; y < inGray.Rows(); y++)
			for (int x = 0; x < inGray.Cols(); x++)
			{
				Uint8 g = inGray[y][x];
				outRgba.setPixel(x, y, sf::Color(g, g, g, 255));
			}
	}


	void Sobel(const Mat<Uint8>& src, Mat<Uint8>& dst)
	{
		SimpleEdge(src, dst, sobelx, sobely);
	}

	void Prewitt(const Mat<Uint8>& src, Mat<Uint8>& dst)
	{
		SimpleEdge(src, dst, prewittx, prewitty);
	}

	void Scharr(const Mat<Uint8>& src, Mat<Uint8>& dst)
	{
		SimpleEdge(src, dst, scharrx, scharry);
	}

	// https://rosettacode.org/wiki/Canny_edge_detector#C
	// https://towardsdatascience.com/canny-edge-detection-step-by-step-in-python-computer-vision-b49c3a2d8123
	void Canny(const Mat<Uint8>& src, Mat<Uint8>& dst, float sigma, float tmin, float tmax)
	{
		float dx, dy, g, dir;
		int x1, y1;
		unsigned int cols = src.Cols();
		unsigned int rows = src.Rows();
		int kernelSize = sobelx.size();
		int kernelOffset = kernelSize / 2;

		Mat<float> temp(rows, cols);
		Mat<std::pair<float, float>> gradDir(rows, cols);
		Mat<float> nms(rows, cols);

		// # Blur
		Convolution(src, temp, Gaussian(sigma));
		
		// # Gradients
		for (int y = 0; y < rows; y++)
		{
			for (int x = 0; x < cols; x++)
			{
				dx = 0.0f;
				dy = 0.0f;

				for (int k = -kernelOffset; k <= kernelOffset; k++)
				{
					for (int j = -kernelOffset; j <= kernelOffset; j++)
					{
						x1 = Circular(cols, x - j);
						y1 = Circular(rows, y - k);

						dx += sobelx[j + kernelOffset][k + kernelOffset] * temp[y1][x1];
						dy += sobely[j + kernelOffset][k + kernelOffset] * temp[y1][x1];
					}
				}

				gradDir[y][x] = std::make_pair(hypot(dx, dy), fmod(atan2(dy, dx) + M_PI, M_PI) / M_PI * 8);
			}
		}

		// # Non-maximum suppression
		for (int y = 1; y < rows - 1; y++)
		{
			for (int x = 1; x < cols - 1; x++)
			{
				g = gradDir[y][x].first;
				dir = gradDir[y][x].second;
				
				if ((1 >= dir || dir > 7) && gradDir[y][x - 1].first < g && gradDir[y][x + 1].first < g ||
					(1 < dir || dir <= 3) && gradDir[y - 1][x - 1].first < g && gradDir[y + 1][x + 1].first < g ||
					(3 < dir || dir <= 5) && gradDir[y - 1][x].first < g && gradDir[y + 1][x].first < g ||
					(5 < dir || dir <= 7) && gradDir[y - 1][x + 1].first < g && gradDir[y + 1][x - 1].first < g)
					nms[y][x] = g;
				else
					nms[y][x] = 0.0;
			}
		}

		// # Edge Tracking by Hysteresis
		Hysteresis(nms, dst, tmin, tmax);
	}

}
