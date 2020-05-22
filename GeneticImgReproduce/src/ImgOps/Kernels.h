#pragma once
#include <array>

namespace gir
{
	template<class T, size_t Rows, size_t Cols>
	using Kernel = std::array<std::array<T, Cols>, Rows>;

	constexpr Kernel<int, 3, 3> sobelx = { { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} } };
	constexpr Kernel<int, 3, 3> sobely = { { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} } };
	constexpr Kernel<int, 3, 3> prewittx = { { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} } };
	constexpr Kernel<int, 3, 3> prewitty = { { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} } };
	constexpr Kernel<int, 3, 3> scharrx = { { {3, 10, 3}, {0, 0, 0}, {-3, -10, -3} } };
	constexpr Kernel<int, 3, 3> scharry = { { {3, 0, -3}, {10, 0, -10}, {3, 0, -3} } };
}