#ifndef UTILS_H
#define UTILS_H

namespace dae::Utils
{
    /// @brief Compile-time SDBM hash implementation over a fixed-length string.
    ///
    /// Recursively processes characters from left to right using the SDBM formula:
    ///   `hash = char + (hash << 6) + (hash << 16) - hash`
    ///
    /// This primary template handles strings of @p length > 1 by delegating to
    /// the length-1 base-case specialisation and then folding each character in.
    ///
    /// Prefer the make_sdbm_hash() helper over using this struct directly.
    ///
    /// @tparam length Number of characters remaining to hash (excludes null terminator).
    template <int length>
    struct sdbm_hash
    {
        /// @brief Recursive step: hashes characters [0, length-2] then folds in character [length-1].
        /// @param text  Pointer to the start of the original string.
        /// @param value Running hash accumulator (modified in place).
        /// @return The character at position [length-1], ready to be folded by the caller.
        consteval static unsigned int _calculate(const char* const text, unsigned int& value) {
            const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
            value = character + (value << 6) + (value << 16) - value;
            return text[length - 1];
        }

        /// @brief Entry point: initialises the accumulator and returns the final hash.
        /// @param text Null-terminated string to hash.
        /// @return 32-bit SDBM hash of the entire string.
        consteval static unsigned int calculate(const char* const text) {
            unsigned int value = 0;
            const auto character = _calculate(text, value);
            return character + (value << 6) + (value << 16) - value;
        }
    };

    /// @brief Base-case specialisation for a single-character string.
    ///
    /// Simply returns the first character; the accumulator is left unchanged
    /// since there are no preceding characters to fold in.
    template <>
    struct sdbm_hash<1>
    {
        /// @brief Returns the sole character of the string without modifying the accumulator.
        /// @param text Pointer to the start of the string.
        /// @param      Unused accumulator (required by the recursive interface).
        /// @return The single character as an int.
        consteval static int _calculate(const char* const text, unsigned int&) { return text[0]; }
    };

    /// @brief Convenience wrapper that computes the SDBM hash of a string literal at compile time.
    ///
    /// The array size @p N is deduced automatically from the string literal, so the
    /// null terminator is excluded from the hash (length passed = N - 1).
    ///
    /// Example usage:
    /// @code
    ///   constexpr unsigned int id = dae::Utils::make_sdbm_hash("ActorDied");
    /// @endcode
    ///
    /// @tparam N   Length of the character array including the null terminator (deduced).
    /// @param text The string literal to hash.
    /// @return     Compile-time 32-bit SDBM hash of @p text.
    template <size_t N>
    consteval unsigned int make_sdbm_hash(const char(&text)[N]) {
        return sdbm_hash<N - 1>::calculate(text);
    }

} // namespace dae::Utils
#endif