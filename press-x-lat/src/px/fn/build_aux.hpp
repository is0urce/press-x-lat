// name: build_aux.hpp
// type: c++
// desc: auxilary template functions
// auth: is0urce

#ifndef PX_FN_BUILD_AUX
#define PX_FN_BUILD_AUX

#include <px/common/rectangle.hpp>

#include <random>
#include <type_traits>

namespace px
{
	namespace fn
	{
		namespace
		{
			// get random item from vector
			template <typename Generator, typename Item>
			Item const& random_item(std::vector<Item> const& vec, Generator &rng)
			{
				if (vec.empty()) throw std::runtime_error("px::fn::random_item(vector<i> v, generator rng) - vector size = 0");
				return vec[std::uniform_int_distribution<std::vector<Item>::size_type>(0, vec.size() - 1)(rng)];
			}

			// get random range in bigger range (inclusive, both 0 and max)
			template <typename Generator>
			point2 random_range(point2 const& range, Generator &rng)
			{
				return point2(std::uniform_int_distribution<point2::component>(0, range.x())(rng), std::uniform_int_distribution<point2::component>(0, range.y())(rng));
			}
			template <typename Generator>
			point2 random_block_direction(Generator &rng)
			{
				point2 result;
				switch (std::uniform_int_distribution<int>{0, 3}(rng))
				{
				case 0: return{ 0, 1 };
				case 1: return{ 1, 0 };
				case 2: return{ -1, 0 };
				case 3: return{ 0, -1 };
				}
				throw std::logic_error("px::fn::random_block_direction - wrong rng logic");
			}

			// get random point in a rectangle
			template <typename Generator>
			point2 random_point(rectangle const& rect, Generator &rng)
			{
				return rect.start() + point2(std::uniform_int_distribution<point2::component>(0, rect.range().x() - 1)(rng), std::uniform_int_distribution<point2::component>(0, rect.range().y() - 1)(rng));
			}

			// select random rectangle int bigger rectangle, cropped by max 1 / x, where x is Divisor template argument
			template <unsigned int Divisor = 1, typename Generator>
			rectangle random_rectangle(rectangle const& rect, Generator &rng)
			{
				static_assert(Divisor > 0, "Divisor == 0");
				point2 shrink = random_range(rect.range() / Divisor, rng);
				return rectangle(rect.start() + random_range(shrink, rng), rect.range() - shrink);
			}

			// get random item in enum class (it has to have min_value and max_value)
			template <typename E, typename Generator>
			E random_enum(Generator &rng)
			{
				typedef std::underlying_type<E>::type u_type;
				return static_cast<E>(std::uniform_int_distribution<u_type>(static_cast<u_type>(E::min_value), static_cast<u_type>(E::max_value))(rng));
			}
		}
	}
}

#endif // !PX_FN_BUILD_AUX