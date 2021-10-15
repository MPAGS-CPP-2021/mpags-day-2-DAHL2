#include <string>
#include <cctype>

// Project headers
#include "TransformChar.hpp"

std::string transformChar (const char in_char)
{
    /* Transliterate input character to uppercase form

    const char in_char: Input character to be transliterated

    return: String containing only upper case letters based on in_char
    */

    std::string out_str;
    // Uppercase alphabetic characters
    if (std::isalpha(in_char)) {
        out_str += std::toupper(in_char);
        return out_str;
    }

    // Transliterate digits to English words
    switch (in_char) {
        case '0':
            out_str += "ZERO";
            return out_str;
        case '1':
            out_str += "ONE";
            return out_str;
        case '2':
            out_str += "TWO";
            return out_str;
        case '3':
            out_str += "THREE";
            return out_str;
        case '4':
            out_str += "FOUR";
            return out_str;
        case '5':
            out_str += "FIVE";
            return out_str;
        case '6':
            out_str += "SIX";
            return out_str;
        case '7':
            out_str += "SEVEN";
            return out_str;
        case '8':
            out_str += "EIGHT";
            return out_str;
        case '9':
            out_str += "NINE";
            return out_str;
    }

    // If the character isn't alphabetic or numeric, DONT add it
    return out_str;
}