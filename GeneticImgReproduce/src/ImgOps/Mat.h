#pragma once

namespace gir
{
    template <typename T>
    class Mat
    {
    private:
        std::size_t m_M, m_N;
        T** m_Mat;
    public:
        Mat();
        Mat(std::size_t m, std::size_t n);
        Mat(const Mat<T>& other);
        Mat(Mat<T>&& other);
        ~Mat();

        void Resize(std::size_t m, std::size_t n);
        void Value(T value);
        std::size_t ValueCount(T value);

        inline std::size_t Rows() const { return m_M; }
        inline std::size_t Cols() const { return m_N; }

        inline const T* operator[](int i) const { return m_Mat[i]; }
        inline T* operator[](int i) { return m_Mat[i]; }

        Mat<T>& operator=(Mat<T>&& other);
    };


    template<typename T>
    Mat<T>::Mat()
        : m_M(0)
        , m_N(0)
        , m_Mat(nullptr)
    {
    }

    template <typename T>
    Mat<T>::Mat(std::size_t m, std::size_t n)
        : m_M(m)
        , m_N(n)
        , m_Mat((m > 0 && n > 0) ? new T * [m] : nullptr)
    {
        if (m_Mat)
        {
            std::size_t nel = m * n;
            m_Mat[0] = new T[nel];
            for (std::size_t i = 1; i < m; i++)
                m_Mat[i] = m_Mat[i - 1] + n;
        }
    }

    template <typename T>
    void Mat<T>::Resize(std::size_t m, std::size_t n)
    {
        m_M = m;
        m_N = n;

        if (m_Mat)
        {
            delete[] m_Mat[0];
            delete[] m_Mat;
        }

        m_Mat = (m > 0 && n > 0) ? new T * [m] : nullptr;

        if (m_Mat)
        {
            std::size_t nel = m * n;
            m_Mat[0] = new T[nel];
            for (std::size_t i = 1; i < m; i++)
                m_Mat[i] = m_Mat[i - 1] + n;
        }
    }

    template <typename T>
    Mat<T>::Mat(const Mat<T>& other)
        : m_M(other.m_M)
        , m_N(other.m_N)
        , m_Mat((other.m_M > 0 && other.m_N > 0) ? new T * [other.m_M] : nullptr)
    {
        if (m_Mat)
        {
            const std::size_t m = other.m_M;
            const std::size_t n = other.m_N;
            const std::size_t nel = m * n;

            m_Mat[0] = new T[nel];
            for (std::size_t i = 1; i < m; i++)
                m_Mat[i] = m_Mat[i - 1] + n;

            for (std::size_t i = 0; i < nel; i++)
                m_Mat[0][i] = other.m_Mat[0][i];
        }
    }

    template <typename T>
    Mat<T>::Mat(Mat<T>&& other)
        : m_M(other.m_M)
        , m_N(other.m_N)
    {
        m_Mat = other.m_Mat;
        other.m_Mat = nullptr;
    }

    template <typename T>
    Mat<T>& Mat<T>::operator=(Mat<T>&& other)
    {
        if (this != &other)
        {
            if (m_Mat)
            {
                delete[] m_Mat[0];
                delete[] m_Mat;
            }

            m_M = other.m_M;
            m_N = other.m_N;
            m_Mat = other.m_Mat;
            other.m_Mat = nullptr;
        }

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

    template <typename T>
    void Mat<T>::Value(T value)
    {
        if (m_Mat)
        {
            std::fill(m_Mat[0], m_Mat[0] + (m_M * m_N), value);
        }
    }

    template <typename T>
    std::size_t Mat<T>::ValueCount(T value)
    {
        if (m_Mat)
        {
            return std::count(m_Mat[0], m_Mat[0] + (m_M * m_N), value);
        }
        else
        {
            return 0;
        }
    }
}