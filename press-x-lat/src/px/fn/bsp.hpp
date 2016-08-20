// name: bsp.hpp
// type: c++ header
// desc: template class
// auth: is0urce

// binary space partition

#ifndef PX_FN_BSP_HPP
#define PX_FN_BSP_HPP

#include <px/common/rectangle.hpp>

#include <memory>
#include <random>

// Random - rng device
// Props - user-defined data associated to node

namespace px
{
	namespace fn
	{
		template <typename Random = std::mt19937, typename Props = void*>
		class bsp
		{
		public:
			typedef Random rng_type;
			typedef Props user_type;
			struct node
			{
			public:
				rectangle bounds;
				user_type data;
				std::unique_ptr<node> l;
				std::unique_ptr<node> r;
				unsigned int count;

			public:
				bool leaf() const noexcept
				{
					return (l && r);
				}
				unsigned int split(rng_type &rng, int min) // returns number of created leaves
				{
					int w = bounds.width();
					int h = bounds.height();

					// select axis to split
					std::uniform_int_distribution<int> even(0, 1);
					bool horisontal = even(rng) == 0;
					if (w * 3 >= h * 4)
					{
						horisontal = true;
					}
					else if (h * 3 > w * 4)
					{
						horisontal = false;
					}

					int max = (horisontal ? w : h) - min;
					if (max >= min)
					{
						std::uniform_int_distribution<int> split(min, max);
						l = std::make_unique<node>();
						r = std::make_unique<node>();
						int c = split(rng);

						if (horisontal)
						{
							l->bounds = { bounds.start(), { c , h } };
							r->bounds = { bounds.start().moved(point2(c, 0)), { w - c, h } };
						}
						else
						{
							l->bounds = { bounds.start(), { w , c } };
							r->bounds = { bounds.start().moved(point2(0, c)), { w, h - c } };
						}

						count = l->split(rng, min) + r->split(rng, min);
					}
					else
					{
						count = 1;
					}

					return count;
				}
				template <typename Operator> void traverse(Operator &fn) const
				{
					if (l && r)
					{
						l->traverse(fn);
						r->traverse(fn);
					}
					else
					{
						fn(*this);
					}
				}
				template <typename Operator> void traverse(Operator &fn)
				{
					if (l && r)
					{
						l->traverse(fn);
						r->traverse(fn);
					}
					else
					{
						fn(*this);
					}
				}
			};

		public:
			bsp(rng_type &rng, rectangle bounds, int min)
			{
				m_root.bounds = bounds;
				m_root.split(rng, min);
			}
			template <typename Operator> void enumerate(Operator &fn) const
			{
				m_root.traverse(fn);
			}
			template <typename Operator> void enumerate(Operator &fn)
			{
				m_root.traverse(fn);
			}
			unsigned int count() const
			{
				return m_root.count;
			}
			template <typename Operator>
			static void enumerate(rng_type &rng, rectangle bounds, int min, Operator &fn)
			{
				bsp partition(rng, bounds, min);
				partition.enumerate(fn);
			}

		private:
			node m_root;
		};
	}
}

#endif