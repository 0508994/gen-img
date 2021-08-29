#include "ImgOps.h"

namespace gir {
using sf::Uint8;
constexpr double deg2rad = M_PI / 180.0;

static void Convolution(const Mat<Uint8>& src, Mat<float>& dst, const Kernel<float, 3, 3>& kernel) {
    float sum;
    int x1, y1;
    const std::size_t cols = src.Cols();
    const std::size_t rows = src.Rows();
    const std::size_t kernelSize = kernel.size();
    const int kernelOffset = kernelSize / 2;

    for (std::size_t y = 0; y < rows; y++) {
        for (std::size_t x = 0; x < cols; x++) {
            sum = 0.0f;
            for (int k = -kernelOffset; k <= kernelOffset; k++) {
                for (int j = -kernelOffset; j <= kernelOffset; j++) {
                    x1 = Circular(cols, x - j);
                    y1 = Circular(rows, y - k);
                    sum += kernel[j + kernelOffset][k + kernelOffset] * src[y1][x1];
                }
            }

            dst[y][x] = sum;
        }
    }
}

static Kernel<float, 3, 3> Gaussian(float sigma = 0.5) {
    assert(sigma >= 0 && sigma <= 0.5);

    Kernel<float, 3, 3> gauss;

    for (std::size_t y = 0; y < 3; y++)
        for (std::size_t x = 0; x < 3; x++) {
            gauss[y][x] = exp(-0.5 * (x * x + y * y) / (sigma * sigma)) / (2.0 * M_PI * sigma * sigma);
        }

    //for (int y = 0; y < 3; y++) {
    //	for (int x = 0; x < 3; x++)
    //		std::cout << gauss[y][x] << "\t";
    //	std::cout << std::endl;
    //}

    return gauss;
}

static void Hysteresis(const Mat<float>& nms, Mat<Uint8>& out, float tmin, float tmax) {
    int ni, nj;
    std::queue<std::pair<int, int>> edges;

    out.Value(0);

    for (std::size_t y = 1; y < nms.Rows() - 1; y++) {
        for (std::size_t x = 1; x < nms.Cols() - 1; x++) {
            if (nms[y][x] >= tmax && out[y][x] == 0) {
                out[y][x] = 255;
                edges.push(std::make_pair(y, x));

                while (!edges.empty()) {
                    auto point = edges.front();
                    edges.pop();

                    for (int i = -1; i <= 1; i++) {
                        ni = point.first + i;

                        for (int j = -1; j <= 1; j++) {
                            nj = point.second + j;

                            if (nms[ni][nj] >= tmin && out[ni][nj] == 0) {
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

static void SimpleEdge(const Mat<Uint8>& src,
                       Mat<Uint8>& dst,
                       const Kernel<int, 3, 3>& kx,
                       const Kernel<int, 3, 3>& ky) {
    float dx, dy;
    int x1, y1;
    const std::size_t cols = src.Cols();
    const std::size_t rows = src.Rows();
    const std::size_t kernelSize = kx.size();
    const int kernelOffset = kernelSize / 2;

    for (std::size_t y = 0; y < rows; y++) {
        for (std::size_t x = 0; x < cols; x++) {
            dx = 0.0f;
            dy = 0.0f;

            for (int k = -kernelOffset; k <= kernelOffset; k++) {
                for (int j = -kernelOffset; j <= kernelOffset; j++) {
                    x1 = Circular(cols, x - j);
                    y1 = Circular(rows, y - k);

                    dx += kx[j + kernelOffset][k + kernelOffset] * src[y1][x1];
                    dy += ky[j + kernelOffset][k + kernelOffset] * src[y1][x1];
                }
            }

            dst[y][x] = static_cast<Uint8>(std::clamp(hypot(dx, dy), 0.0f, 255.0f));
            //dst[y][x] = dst[y][x] >= 70 ? 255 : 0;
        }
    }
}

void ToGrayscale(const sf::Image& inRgba, Mat<Uint8>& outGray) {
    const Uint8* pByteBuffer = inRgba.getPixelsPtr();
    const auto sfSize = inRgba.getSize();

    const std::size_t numPixels = sfSize.x * sfSize.y;

    for (std::size_t i = 0; i < numPixels; ++i) {
        float red = pByteBuffer[4 * i] * 0.299f;
        float green = pByteBuffer[4 * i + 1] * 0.587;
        float blue = pByteBuffer[4 * i + 2] * 0.144;
        //sf::Uint8 alpha = pByteBuffer[4 * i + 3];

        float gray = std::floor((red + green + blue + 0.5));
        gray = std::clamp(gray, 0.0f, 255.0f);

        outGray[0][i] = static_cast<Uint8>(gray);
    }
}

void ToSFMLImage(const Mat<Uint8>& inGray, sf::Image& outRgba) {
    for (std::size_t y = 0; y < inGray.Rows(); y++) {
        for (std::size_t x = 0; x < inGray.Cols(); x++) {
            Uint8 g = inGray[y][x];
            outRgba.setPixel(x, y, sf::Color(g, g, g, 255));
        }
    }
}

void Sobel(const Mat<Uint8>& src, Mat<Uint8>& dst) {
    SimpleEdge(src, dst, sobelx, sobely);
}

void Prewitt(const Mat<Uint8>& src, Mat<Uint8>& dst) {
    SimpleEdge(src, dst, prewittx, prewitty);
}

void Scharr(const Mat<Uint8>& src, Mat<Uint8>& dst) {
    SimpleEdge(src, dst, scharrx, scharry);
}

/*
        Canny edge detector implementation based on the following tutorials:
        https://rosettacode.org/wiki/Canny_edge_detector#C
        https://towardsdatascience.com/canny-edge-detection-step-by-step-in-python-computer-vision-b49c3a2d8123
    */
void Canny(const Mat<Uint8>& src, Mat<Uint8>& dst, float sigma, float tmin, float tmax) {
    float dx, dy, g, dir;
    int x1, y1;
    const std::size_t cols = src.Cols();
    const std::size_t rows = src.Rows();
    const std::size_t kernelSize = sobelx.size();
    int kernelOffset = kernelSize / 2;

    Mat<float> temp(rows, cols);
    Mat<std::pair<float, float>> gradDir(rows, cols);
    Mat<float> nms(rows, cols);

    // # Blur
    Convolution(src, temp, Gaussian(sigma));

    // # Gradients
    for (std::size_t y = 0; y < rows; y++) {
        for (std::size_t x = 0; x < cols; x++) {
            dx = 0.0f;
            dy = 0.0f;

            for (int k = -kernelOffset; k <= kernelOffset; k++) {
                for (int j = -kernelOffset; j <= kernelOffset; j++) {
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
    for (std::size_t y = 1; y < rows - 1; y++) {
        for (std::size_t x = 1; x < cols - 1; x++) {
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

void Threshold(Mat<Uint8>& src, Uint8 value) {
    for (std::size_t y = 0; y < src.Rows(); y++) {
        for (std::size_t x = 0; x < src.Cols(); x++) {
            src[y][x] = src[y][x] >= value ? 255 : 0;
        }
    }
}

/*
        HoughTransform and HoughLines - taken and slightly modified from the following tutorial:
        http://www.keymolen.com/2013/05/hough-transformation-c-implementation.html


        also:
        opencv/modules/imgproc/src/hough.cpp
    */

void HoughTransform(const Mat<Uint8>& edges, Mat<std::size_t>& accumulator) {
    double r;
    const std::size_t rows = edges.Rows();
    const std::size_t cols = edges.Cols();
    const double centerY = rows / 2.0;
    const double centerX = cols / 2.0;

    const double houghH = (sqrt(2.0) * (rows > cols ? rows : cols)) / 2.0;   // max distance [rect diag]
    const std::size_t accH = houghH * 2.0;
    const std::size_t accW = 180;

    accumulator.Resize(accH, accW);
    accumulator.Value(0);
    //memset(accumulator[0], 0, rows * cols * sizeof(std::size_t));

    for (std::size_t y = 0; y < rows; y++) {
        for (std::size_t x = 0; x < cols; x++) {
            if (edges[y][x])   // assume edges are threshed
            {
                for (std::size_t a = 0; a < 180; a++) {
                    r = ((x - centerX) * cos(a * deg2rad)) + ((y - centerY) * sin(a * deg2rad));
                    accumulator[static_cast<int>(r + houghH)][a]++;
                }
            }
        }
    }
}

std::vector<Line> HoughLines(const Mat<Uint8>& edges, std::size_t threshold) {
    int max, r1, t1;
    double radAngle, x1, y1, x2, y2;
    std::vector<Line> lines;
    Mat<std::size_t> accumulator;

    HoughTransform(edges, accumulator);

    const std::size_t accH = accumulator.Rows();
    const std::size_t accW = accumulator.Cols();
    const double rows = static_cast<double>(edges.Rows());
    const double cols = static_cast<double>(edges.Cols());

    for (std::size_t r = 0; r < accH; r++) {
        for (std::size_t t = 0; t < accW; t++) {
            if (accumulator[r][t] >= threshold) {
                max = accumulator[r][t];

                for (int ly = -4; ly <= 4; ly++)   // search a 9x9 patch
                {
                    r1 = ly + r;
                    for (int lx = -4; lx <= 4; lx++) {
                        t1 = lx + t;
                        if (r1 >= 0 && r1 < accH && t1 >= 0 && t1 < accW) {
                            if (accumulator[r1][t1] > max) {
                                max = accumulator[r1][t1];
                                ly = lx = 5;   // exit both loops
                            }
                        }
                    }
                }

                if (max > accumulator[r][t])
                    continue;

                x1 = x2 = y1 = y2 = 0.0;
                radAngle = t * deg2rad;

                // https://stackoverflow.com/questions/13663545/does-one-double-promote-every-int-in-the-equation-to-double !!!!!!!!!!!!!!!!!!!!!
                if (t >= 45 && t <= 135) {
                    x1 = 0.0;
                    y1 = ((r - accH / 2.0) - ((x1 - cols / 2.0) * cos(radAngle))) / sin(radAngle) + (rows / 2.0);
                    x2 = cols;
                    y2 = ((r - accH / 2.0) - ((x2 - cols / 2.0) * cos(radAngle))) / sin(radAngle) + (rows / 2.0);
                } else {
                    y1 = 0.0;
                    x1 = ((r - accH / 2.0) - ((y1 - rows / 2.0) * sin(radAngle))) / cos(radAngle) + (cols / 2.0);
                    y2 = rows;
                    x2 = ((r - accH / 2.0) - ((y2 - rows / 2.0) * sin(radAngle))) / cos(radAngle) + (cols / 2.0);
                }

                lines.emplace_back(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2));
            }
        }
    }

    return lines;
}
}   // namespace gir
