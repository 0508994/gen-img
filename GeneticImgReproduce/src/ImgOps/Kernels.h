#pragma once
#include <vector>

namespace gir
{
	typedef std::vector<std::vector<int>> Kernel;

	//constexpr const int sobelx3[3][3] =  { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} } ;
	//constexpr const int sobely3[3][3] =  { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	//constexpr const int prewittx3[3][3] =  { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
	//constexpr const int prewitty3[3][3] =  { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
	//constexpr const int scharrx3[3][3] =  { {3, 10, 3}, {0, 0, 0}, {-3, -10, -3} };
	//constexpr const int scharry3[3][3] =  { {3, 0, -3}, {10, 0, -10}, {3, 0, -3} };


	const Kernel sobelx3 = { {-1, 0, 1}, {2, 0, -2}, {-1, 0, 1} };
	const Kernel sobely3 = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	const Kernel prewittx3 = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
	const Kernel prewitty3 = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
	const Kernel scharrx3 = { {3, 10, 3}, {0, 0, 0}, {-3, -10, -3} };
	const Kernel scharry3 = { {3, 0, -3}, {10, 0, -10}, {3, 0, -3} };
}