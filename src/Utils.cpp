#include <Utils.hpp>

#include <algorithm>
#include <sstream>
#include <vector>

namespace MO
{
    namespace Utils
    {
        std::vector<Symbol> split(const std::string &a_string, char a_delimiter)
        {
            std::stringstream ss(a_string);
            std::string item;
            Symbols elems;

            while (std::getline(ss, item, a_delimiter))
            {
                elems.push_back(item.c_str());
            }
            return elems;
        }
    } // namespace Utils
} // namespace MO
