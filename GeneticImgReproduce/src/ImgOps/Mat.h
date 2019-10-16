#pragma once

namespace gir
{
	template <typename T>
	class Mat
	{
	private:
		int m_M, m_N;
		T** m_Mat;
	public:
		Mat();
		Mat(int m, int n);
		Mat(const Mat<T>& other);
		~Mat();

		void Value(T value);

		inline int Rows() const { return m_M; }
		inline int Cols() const { return m_N; }

		inline const T* operator[](int i) const { return m_Mat[i]; }
		inline T* operator[](int i) { return m_Mat[i]; }
	};


	template<typename T>
	Mat<T>::Mat()
	{
		m_M = m_N = 0;
		m_Mat = nullptr;
	}


	template <typename T>
	Mat<T>::Mat(int m, int n)
		:m_M(m), m_N(n), m_Mat((m > 0 && n > 0) ? new T*[m] : nullptr)
	{
		if (m_Mat)
		{
			int nel = m * n;
			m_Mat[0] = new T[nel];
			for (int i = 1; i < m; i++)
				m_Mat[i] = m_Mat[i - 1] + n;
		}
	}

	template <typename T>
	Mat<T>::Mat(const Mat<T>& other)
		:m_M(other.m_M), m_N(other.m_N), m_Mat((other.m_M > 0 && other.m_N > 0) ? new T*[other.m_M] : nullptr)
	{
		if (m_Mat)
		{
			int m = other.m_M;
			int n = other.m_N;
			int nel = m * n;

			m_Mat[0] = new T[nel];
			for (int i = 1; i < m; i++)
				m_Mat[i] = m_Mat[i - 1] + n;

			for (int i = 0; i < nel; i++)
				m_Mat[0][i] = other.m_Mat[0][i];
		}
	}

	template <typename T>
	Mat<T>::~Mat()
	{
		if (m_Mat)
		{
			delete[] m_Mat[0];
			delete[] m_Mat;
		}
	}

	template<typename T>
	inline void Mat<T>::Value(T value)
	{
		if (m_M != nullptr)
		{
			for (int i = 0; i < m_M; i++)
				for (int j = 0; j < m_N; j++)
					m_Mat[i][j] = value;
		}
	}
}