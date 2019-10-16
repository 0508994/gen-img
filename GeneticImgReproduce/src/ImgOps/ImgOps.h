#pragma once
#include "Mat.h"

typedef unsigned char uchar;

namespace gir
{
	Mat<uchar> GrayscaleFromSFMLPtr(uchar* ptr);
}