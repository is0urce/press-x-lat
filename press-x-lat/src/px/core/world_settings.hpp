// name: world_settings.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_WORLD_SETTINGS_HPP
#define PX_CORE_WORLD_SETTINGS_HPP

#include <array>

namespace px
{
	namespace core
	{
		class world_settings
		{
		public:
			static const size_t attribute_count = 5;
			static const int min_value = 0;
			static const int max_value = 10;
			static const int median_value = (min_value + max_value) / 2;

		public:
			template <size_t Attribute>
			int get() const noexcept
			{
				static_assert(Attribute < attribute_count, "Attribute < attribute_count");
				return m_attributes[Attribute];
			}
			template <size_t Attribute>
			int& set(int n) noexcept
			{
				static_assert(Attribute < attribute_count, "Attribute < attribute_count");
				return m_attributes[Attribute] = n;
			}

			int& increment(size_t attribute) noexcept
			{
				return m_attributes[attribute] = (std::min)(m_attributes[attribute] + 1, max_value);
			}
			int& decrement(size_t attribute) noexcept
			{
				return m_attributes[attribute] = (std::max)(m_attributes[attribute] - 1, min_value);
			}

			int& operator[](size_t attribute) noexcept
			{
				return m_attributes[attribute];
			}
			const int& operator[](size_t attribute) const noexcept
			{
				return m_attributes[attribute];
			}

		public:
			world_settings() { m_attributes.fill(median_value); }

		private:
			std::array<int, attribute_count> m_attributes;
		};
	}
}

#endif