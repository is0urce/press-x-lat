// name: assert.hpp
// type: c++ header
// desc: assert macro

#ifndef PX_ASSERT_HPP
#define PX_ASSERT_HPP

#include <stdexcept>
#include <string>
#include <vector>
#include <set>

namespace px
{
	class debug
	{
	public:
		std::vector<std::string> splitpath(const std::string& str, const std::set<char> delimiters)
		{
			std::vector<std::string> result;

			char const* pch = str.c_str();
			char const* start = pch;
			for (; *pch; ++pch)
			{
				if (delimiters.find(*pch) != delimiters.end())
				{
					if (start != pch)
					{
						std::string str(start, pch);
						result.push_back(str);
					}
					else
					{
						result.push_back("");
					}
					start = pch + 1;
				}
			}
			result.push_back(start);

			return result;
		}
		static void assert(std::string condition, std::string text, std::string file, unsigned int line, std::string function_name) 
		{
			throw std::runtime_error
				( std::string("px_assert ") + condition
				+ std::string(" in ") + file
				+ std::string(" line ") + std::to_string(line)
				+ std::string(" in ") + function_name
				+ std::string(" with ") + text
				);
		}
	};
}

#define px_markdown (*(splitpach(__FILE__, {'/', '\\'}).rend()) + std::string(__LINE__ __func__))
#define px_assert(condition,text) if (!condition) px::debug::assert(#condition, std::string(text), __FILE__, __LINE__, std::string(__func__));

#endif