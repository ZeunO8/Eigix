#include <array>       // For std::array
#include <cstddef>     // For size_t
#include <stdexcept>   // For exceptions
#include <iostream>    // For printing
#include <iomanip>     // For formatted output
#include <vector>      // Useful for initialization if needed (though populate uses variadic)
#include <type_traits> // For static_assert, is_floating_point
#include <limits>      // For numeric_limits (epsilon check)
#include <cmath>       // For std::fabs

template <typename T, size_t Depth, size_t Rows, size_t Cols>
struct Tensor3D
{
    // --- Data Member ---
    // Nested array: Depth contains 2D slices (Rows x Cols)
    std::array<std::array<std::array<T, Cols>, Rows>, Depth> data;

    // --- Constructors ---
    // Default constructor: Initializes all elements to T{}
    Tensor3D()
    {
        for (size_t d = 0; d < Depth; ++d)
        {
            for (size_t r = 0; r < Rows; ++r)
            {
                for (size_t c = 0; c < Cols; ++c)
                {
                    data[d][r][c] = T{}; // Zero initialization
                }
            }
        }
    }

    // --- Element Access ---
    // Accessor (const version) - Throws if out of bounds
    const T &operator()(size_t depth, size_t row, size_t col) const
    {
        if (depth >= Depth || row >= Rows || col >= Cols)
        {
            throw std::out_of_range("Tensor3D access out of bounds");
        }
        return data[depth][row][col];
    }

    // Mutator (non-const version) - Throws if out of bounds
    T &operator()(size_t depth, size_t row, size_t col)
    {
        if (depth >= Depth || row >= Rows || col >= Cols)
        {
            throw std::out_of_range("Tensor3D access out of bounds");
        }
        return data[depth][row][col];
    }

    // --- Dimension Getters ---
    constexpr size_t numDepth() const { return Depth; }
    constexpr size_t numRows() const { return Rows; }
    constexpr size_t numCols() const { return Cols; }

    // --- Element-wise Addition ---
    // Adds two tensors of the exact same dimensions element-wise.
    Tensor3D<T, Depth, Rows, Cols> operator+(const Tensor3D<T, Depth, Rows, Cols> &other) const
    {
        Tensor3D<T, Depth, Rows, Cols> result;
        for (size_t d = 0; d < Depth; ++d)
        {
            for (size_t r = 0; r < Rows; ++r)
            {
                for (size_t c = 0; c < Cols; ++c)
                {
                    result(d, r, c) = (*this)(d, r, c) + other(d, r, c);
                }
            }
        }
        return result;
    }

    // --- Element-wise Subtraction ---
    // Subtracts another tensor (of the same dimensions) from this tensor element-wise.
    Tensor3D<T, Depth, Rows, Cols> operator-(const Tensor3D<T, Depth, Rows, Cols> &other) const
    {
        Tensor3D<T, Depth, Rows, Cols> result;
        for (size_t d = 0; d < Depth; ++d)
        {
            for (size_t r = 0; r < Rows; ++r)
            {
                for (size_t c = 0; c < Cols; ++c)
                {
                    result(d, r, c) = (*this)(d, r, c) - other(d, r, c);
                }
            }
        }
        return result;
    }

    // --- Scalar Multiplication ---
    // Multiplies every element of the tensor by a scalar value.
    Tensor3D<T, Depth, Rows, Cols> operator*(const T &scalar) const
    {
        Tensor3D<T, Depth, Rows, Cols> result;
        for (size_t d = 0; d < Depth; ++d)
        {
            for (size_t r = 0; r < Rows; ++r)
            {
                for (size_t c = 0; c < Cols; ++c)
                {
                    result(d, r, c) = (*this)(d, r, c) * scalar;
                }
            }
        }
        return result;
    }

    // --- Scalar Division ---
    // Divides every element of the tensor by a scalar value.
    // Throws std::runtime_error on division by zero.
    Tensor3D<T, Depth, Rows, Cols> operator/(const T &scalar) const
    {
        // Runtime check for division by zero
        if constexpr (std::is_floating_point_v<T>)
        {
            if (std::fabs(scalar) < std::numeric_limits<T>::epsilon())
            {
                throw std::runtime_error("Tensor3D division by zero or near-zero scalar");
            }
        }
        else
        { // For integer types
            if (scalar == T{})
            {
                throw std::runtime_error("Tensor3D division by zero scalar");
            }
        }

        Tensor3D<T, Depth, Rows, Cols> result;
        for (size_t d = 0; d < Depth; ++d)
        {
            for (size_t r = 0; r < Rows; ++r)
            {
                for (size_t c = 0; c < Cols; ++c)
                {
                    result(d, r, c) = (*this)(d, r, c) / scalar;
                }
            }
        }
        return result;
    }

    // --- Populate Helper ---
    // Fills the tensor element by element (depth varies fastest, then row, then col).
    // Expects exactly Depth * Rows * Cols arguments.
    template <typename... Args>
    void populate(Args... args)
    {
        static_assert(sizeof...(args) == Depth * Rows * Cols, "Incorrect number of arguments for populate");

        T values[] = {static_cast<T>(args)...}; // Use static_cast for type safety
        size_t k = 0;
        for (size_t d = 0; d < Depth; ++d)
        {
            for (size_t r = 0; r < Rows; ++r)
            {
                for (size_t c = 0; c < Cols; ++c)
                {
                    data[d][r][c] = values[k++];
                }
            }
        }
    }
};

// --- Overload operator<< for printing ---
// Prints the Tensor3D slice by slice along the depth dimension.
template <typename T, size_t Depth, size_t Rows, size_t Cols>
std::ostream &operator<<(std::ostream &os, const Tensor3D<T, Depth, Rows, Cols> &tensor)
{
    os << "Tensor3D (" << Depth << "x" << Rows << "x" << Cols << "):\n";
    int width = 8;                                       // Adjust width as needed
    std::ios_base::fmtflags original_flags = os.flags(); // Save original formatting
    os << std::fixed << std::setprecision(2);

    for (size_t d = 0; d < Depth; ++d)
    {
        os << "Depth Slice [" << d << "]:\n";
        for (size_t r = 0; r < Rows; ++r)
        {
            os << "  [";
            for (size_t c = 0; c < Cols; ++c)
            {
                os << std::setw(width) << tensor(d, r, c) << (c == Cols - 1 ? "" : ", ");
            }
            os << " ]\n";
        }
        if (d < Depth - 1)
        {
            os << "\n"; // Add space between slices
        }
    }
    os.flags(original_flags); // Restore original formatting
    return os;
}

// Allow scalar * Tensor3D (requires a free function)
template <typename T, size_t Depth, size_t Rows, size_t Cols>
Tensor3D<T, Depth, Rows, Cols> operator*(const T &scalar, const Tensor3D<T, Depth, Rows, Cols> &tensor)
{
    return tensor * scalar; // Reuse the member operator*
}