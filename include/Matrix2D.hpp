#include <array>       // For std::array
#include <cstddef>     // For size_t
#include <stdexcept>   // For exceptions
#include <iostream>    // For printing
#include <iomanip>     // For formatted output
#include <type_traits> // For static_assert, is_floating_point
#include <limits>      // For numeric_limits (epsilon check)
#include <cmath>       // For std::fabs

template <typename T, size_t Rows, size_t Cols>
struct Matrix
{
    // --- Data Member ---
    std::array<std::array<T, Cols>, Rows> data;

    // --- Constructors ---
    // Default constructor: Initializes all elements to T{} (e.g., 0 for numeric types)
    Matrix()
    {
        for (size_t r = 0; r < Rows; ++r)
        {
            for (size_t c = 0; c < Cols; ++c)
            {
                data[r][c] = T{}; // Zero initialization
            }
        }
    }

    // --- Element Access ---
    const T &operator()(size_t row, size_t col) const
    {
        if (row >= Rows || col >= Cols)
        {
            throw std::out_of_range("Matrix access out of bounds");
        }
        return data[row][col];
    }

    T &operator()(size_t row, size_t col)
    {
        if (row >= Rows || col >= Cols)
        {
            throw std::out_of_range("Matrix access out of bounds");
        }
        return data[row][col];
    }

    // --- Dimension Getters ---
    constexpr size_t numRows() const { return Rows; }
    constexpr size_t numCols() const { return Cols; }

    // --- Matrix Multiplication ---
    template <size_t OtherRows, size_t OtherCols>
    Matrix<T, Rows, OtherCols> operator*(const Matrix<T, OtherRows, OtherCols> &other) const
    {
        static_assert(Cols == OtherRows,
                      "Matrix multiplication dimension mismatch: "
                      "Left matrix columns must equal Right matrix rows.");

        Matrix<T, Rows, OtherCols> result; // Initialized to zeros by default constructor
        for (size_t i = 0; i < Rows; ++i)
        {
            for (size_t j = 0; j < OtherCols; ++j)
            {
                T sum = T{};
                for (size_t k = 0; k < Cols; ++k)
                {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    // --- Matrix Addition ---
    // Adds two matrices of the exact same dimensions element-wise.
    Matrix<T, Rows, Cols> operator+(const Matrix<T, Rows, Cols> &other) const
    {
        Matrix<T, Rows, Cols> result;
        for (size_t r = 0; r < Rows; ++r)
        {
            for (size_t c = 0; c < Cols; ++c)
            {
                result(r, c) = (*this)(r, c) + other(r, c);
            }
        }
        return result;
    }

    // --- Matrix Subtraction ---
    // Subtracts another matrix (of the same dimensions) from this matrix element-wise.
    Matrix<T, Rows, Cols> operator-(const Matrix<T, Rows, Cols> &other) const
    {
        Matrix<T, Rows, Cols> result;
        for (size_t r = 0; r < Rows; ++r)
        {
            for (size_t c = 0; c < Cols; ++c)
            {
                result(r, c) = (*this)(r, c) - other(r, c);
            }
        }
        return result;
    }

    // --- Scalar Division ---
    // Divides every element of the matrix by a scalar value.
    // Throws std::runtime_error on division by zero.
    Matrix<T, Rows, Cols> operator/(const T &scalar) const
    {
        // Runtime check for division by zero
        // Use epsilon for floating point comparisons
        if constexpr (std::is_floating_point_v<T>)
        {
            if (std::fabs(scalar) < std::numeric_limits<T>::epsilon())
            {
                throw std::runtime_error("Matrix division by zero or near-zero scalar");
            }
        }
        else
        { // For integer types
            if (scalar == T{})
            {
                throw std::runtime_error("Matrix division by zero scalar");
            }
        }

        Matrix<T, Rows, Cols> result;
        for (size_t r = 0; r < Rows; ++r)
        {
            for (size_t c = 0; c < Cols; ++c)
            {
                result(r, c) = (*this)(r, c) / scalar;
            }
        }
        return result;
    }

    // --- Populate Helper (Optional) ---
    template <typename... Args>
    void populate(Args... args)
    {
        static_assert(sizeof...(args) == Rows * Cols, "Incorrect number of arguments for populate");
        T values[] = {static_cast<T>(args)...};
        size_t k = 0;
        for (size_t r = 0; r < Rows; ++r)
        {
            for (size_t c = 0; c < Cols; ++c)
            {
                data[r][c] = values[k++];
            }
        }
    }
};

// --- Overload operator<< for easy printing (Optional) ---
template <typename T, size_t Rows, size_t Cols>
std::ostream &operator<<(std::ostream &os, const Matrix<T, Rows, Cols> &matrix)
{
    os << "Matrix (" << Rows << "x" << Cols << "):\n";
    // Determine a reasonable width for alignment, check for very small/large numbers later if needed
    int width = 8;
    os << std::fixed << std::setprecision(2); // Example formatting for floats

    for (size_t r = 0; r < Rows; ++r)
    {
        os << "[";
        for (size_t c = 0; c < Cols; ++c)
        {
            os << std::setw(width) << matrix(r, c) << (c == Cols - 1 ? "" : ", ");
        }
        os << " ]\n";
    }
    os.copyfmt(std::ios(nullptr)); // Reset formatting flags potentially set
    return os;
}