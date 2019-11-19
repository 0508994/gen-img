#pragma once

namespace gir
{
	template <typename T>
	class Mat
	{
	private:
		unsigned int m_M, m_N;
		T** m_Mat;
	public:
		Mat();
		Mat(unsigned int m, unsigned int n);
		Mat(const Mat<T>& other);
		Mat(Mat<T>&& other);
		~Mat();

		void Resize(unsigned int m, unsigned int n);
		void Value(T value);
		unsigned int ValueCount(T value);

		inline unsigned int Rows() const { return m_M; }
		inline unsigned int Cols() const { return m_N; }

		inline const T* operator[](int i) const { return m_Mat[i]; }
		inline T* operator[](int i) { return m_Mat[i]; }

		Mat<T>& operator=(Mat<T>&& other);
	};


	template<typename T>
	Mat<T>::Mat()
	{
		m_M = m_N = 0;
		m_Mat = nullptr;
	}

	template <typename T>
	Mat<T>::Mat(unsigned int m, unsigned int n)
		:m_M(m), m_N(n), m_Mat((m > 0 && n > 0) ? new T*[m] : nullptr)
	{
		if (m_Mat)
		{
			unsigned int nel = m * n;
			m_Mat[0] = new T[nel];
			for (unsigned int i = 1; i < m; i++)
				m_Mat[i] = m_Mat[i - 1] + n;
		}
	}

	template <typename T>
	void Mat<T>::Resize(unsigned int m, unsigned int n)
	{
		m_M = m;
		m_N = n;
		
		if (m_Mat)
		{
			delete[] m_Mat[0];
			delete[] m_Mat;
		}

		m_Mat = (m > 0 && n > 0) ? new T*[m] : nullptr;
		
		if (m_Mat)
		{
			unsigned int nel = m * n;
			m_Mat[0] = new T[nel];
			for (unsigned int i = 1; i < m; i++)
				m_Mat[i] = m_Mat[i - 1] + n;
		}
	}

	template <typename T>
	Mat<T>::Mat(const Mat<T>& other)
		:m_M(other.m_M), m_N(other.m_N), m_Mat((other.m_M > 0 && other.m_N > 0) ? new T*[other.m_M] : nullptr)
	{
		if (m_Mat)
		{
			unsigned int m = other.m_M;
			unsigned int n = other.m_N;
			unsigned int nel = m * n;

			m_Mat[0] = new T[nel];
			for (unsigned int i = 1; i < m; i++)
				m_Mat[i] = m_Mat[i - 1] + n;

			for (unsigned int i = 0; i < nel; i++)
				m_Mat[0][i] = other.m_Mat[0][i];
		}
	}

	template <typename T>
	Mat<T>::Mat(Mat<T>&& other)
		:m_M(other.m_M), m_N(other.m_N)
	{
		m_Mat = other.m_Mat;
		other.m_Mat = nullptr;
	}

	template <typename T>
	Mat<T>& Mat<T>::operator=(Mat<T>&& other)
	{
		m_M = other.m_M;
		m_N = other.m_N;
		m_Mat = other.m_Mat;
		other.m_Mat = nullptr;

		return *this;
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

	// TODO: Add std::is_fundamental<T> check and use memset from primitive types
	template <typename T>
	void Mat<T>::Value(T value)
	{
		if (m_Mat != nullptr)
		{
			for (unsigned int i = 0; i < m_M; i++)
				for (unsigned int j = 0; j < m_N; j++)
					m_Mat[i][j] = value;
		}
	}

	template <typename T>
	unsigned int Mat<T>::ValueCount(T value)
	{
		unsigned int count = 0;

		if (m_Mat != nullptr)
		{
			for (unsigned int i = 0; i < m_M; i++)
				for (unsigned int j = 0; j < m_N; j++)
					if (m_Mat[i][j] == value)
						count++;
		}

		return count;
	}
}