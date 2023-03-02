//
// Created by Marco Silva
//

#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>

class mstring
{

    public:

    // trim from start (in place)
    static inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string& s) {
        rtrim(s);
        ltrim(s);
    }

    static const bool iequals(const std::string& a, const std::string& b)
    {
        return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b)
        {
            return tolower(a) == tolower(b);
        });
    }
};