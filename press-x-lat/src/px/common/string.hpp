// name: string.hpp
// type: c++ header
// desc: string functions
// auth: is0urce

#ifndef PX_COMMON_STRING_HPP
#define PX_COMMON_STRING_HPP

#include <string>
#include <functional>

namespace
{
	const unsigned int replacement_character = 0xFFFD;
	const unsigned int byte_order_mark = 0xfeff;
}

namespace px
{
	class string
	{
	public:
		// _O operator should support type void(unsigned int)
		template<typename _O>
		static void enum_utf8(const std::string &text, _O fn)
		{
			unsigned int i = 0;
			unsigned int len = text.length();
			while (i < len)
			{
				unsigned char ch = text[i];
				unsigned int letter = replacement_character;

				if (ch < 192)
				{
					letter = ch;
					++i;
				}
				else if (i + 1 < len)
				{
					unsigned char ch2 = text[i + 1];
					if (ch < 224 && ch2 > 127)
					{
						letter = ((ch & 0x1f) << 6) | (ch2 & 0x3f);
						i += 2;
					}
					else if (i + 2 < len)
					{
						unsigned char ch3 = text[i + 2];
						if (ch < 240 && ch2 > 127 && ch3 > 127)
						{
							letter = ((ch & 0x0f) << 12) | ((ch2 & 0x3f) << 6) | (ch3 & 0x3f);
							i += 3;
						}
						else if (i + 3 < len)
						{
							unsigned char ch4 = text[i + 3];
							if (ch < 248 && ch2 > 127 && ch3 > 127 && ch4 > 127)
							{
								letter = ((ch & 0x07) << 18) | ((ch2 & 0x3f) << 12) | ((ch3 & 0x3f) << 6) | (ch4 & 0x3f);
								i += 4;
							}
							else
							{
								++i;
							}
						}
						else
						{
							return;
						}
					}
					else
					{
						return;
					}
				}
				else
				{
					return;
				}

				if (letter == byte_order_mark)
				{
					// skip byte-order-mark
				}
				else
				{
					fn(letter);
				}
			}
		}
	};
}

#endif