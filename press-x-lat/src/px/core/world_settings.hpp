// name: world_settings.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_WORLD_SETTINGS_HPP
#define PX_CORE_WORLD_SETTINGS_HPP

#include <px/core/world_aspect.hpp>

#include <array>

namespace px
{
	namespace core
	{
		class world_settings
		{
		public:
			static const size_t attribute_count = static_cast<size_t>(world_aspect::max_value) + 1;
			static const int min_value = 0;
			static const int max_value = 10;
			static const int median_value = (min_value + max_value) / 2;


		public:
			template <world_aspect Attribute>
			int get() const noexcept
			{
				return m_attributes[static_cast<size_t>(Attribute)];
			}
			template <world_aspect Attribute>
			int& set(int n) noexcept
			{
				return m_attributes[static_cast<size_t>(Attribute)] = n;
			}

			int& increment(world_aspect attribute) noexcept
			{
				size_t index = static_cast<size_t>(attribute);
				return m_attributes[index] = (std::min)(m_attributes[index] + 1, max_value);
			}
			int& decrement(world_aspect attribute) noexcept
			{
				size_t index = static_cast<size_t>(attribute);
				return m_attributes[index] = (std::max)(m_attributes[index] - 1, min_value);
			}
			template <world_aspect Attribute>
			int& increment() noexcept
			{
				size_t index = static_cast<size_t>(Attribute);
				return m_attributes[index] = (std::min)(m_attributes[index] + 1, max_value);
			}
			template <world_aspect Attribute>
			int& decrement() noexcept
			{
				size_t index = static_cast<size_t>(Attribute);
				return m_attributes[index] = (std::max)(m_attributes[index] - 1, min_value);
			}
			int& mod(world_aspect attribute, int val) noexcept
			{
				size_t index = static_cast<size_t>(attribute);
				return m_attributes[index] = (std::min)((std::max)(m_attributes[index] + val, min_value), max_value);
			}

			int& operator[](world_aspect attribute) noexcept
			{
				return m_attributes[static_cast<size_t>(attribute)];
			}
			const int& operator[](world_aspect attribute) const noexcept
			{
				return m_attributes[static_cast<size_t>(attribute)];
			}

			unsigned int seed() const
			{
				unsigned int seed = 0;
				int mult = 1;
				for (auto attribute : m_attributes)
				{
					seed += attribute * mult;
					mult *= static_cast<int>(attribute_count);
				}
				return seed;
			}

		public:
			world_settings() { m_attributes.fill(median_value); }

		private:
			std::array<int, attribute_count> m_attributes;
		};
	}
}

#endif