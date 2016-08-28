// name: bsp.hpp
// type: c++ header
// desc: template class
// auth: is0urce

// binary space partition generator

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
				unsigned int split(rng_type &rng, int min, int margin) // returns number of created leaves
				{
					int w = bounds.width();
					int h = bounds.height();

					// select axis to split
					bool horisontal = std::uniform_int_distribution<int>{ 0, 1 }(rng) == 0;
					if (w * 3 >= h * 4)
					{
						horisontal = true;
					}
					else if (h * 3 > w * 4)
					{
						horisontal = false;
					}

					int max = (horisontal ? w : h) - min - margin;
					if (max >= min)
					{
						l = std::make_unique<node>();
						r = std::make_unique<node>();
						int split_point = std::uniform_int_distribution<int>{ min, max }(rng);

						if (horisontal)
						{
							l->bounds = { bounds.start(), { split_point, h } };
							r->bounds = { bounds.start().moved(split_point + margin, 0), { w - split_point - margin, h } };
						}
						else
						{
							l->bounds = { bounds.start(), { w, split_point } };
							r->bounds = { bounds.start().moved(0, split_point + margin), { w, h - split_point - margin} };
						}

						count = l->split(rng, min, margin) + r->split(rng, min, margin);
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
				template <typename Operator> void traverse_bounds(Operator &fn)
				{
					if (l && r)
					{
						l->traverse_bounds(fn);
						r->traverse_bounds(fn);
					}
					else
					{
						fn(bounds);
					}
				}
				template <typename Operator> void traverse_bounds(Operator &fn) const
				{
					if (l && r)
					{
						l->traverse_bounds(fn);
						r->traverse_bounds(fn);
					}
					else
					{
						fn(bounds);
					}
				}
				template <typename Operator> void traverse_data(Operator &fn)
				{
					if (l && r)
					{
						l->traverse_data(fn);
						r->traverse_data(fn);
					}
					else
					{
						fn(data);
					}
				}
				template <typename Operator> void traverse_data(Operator &fn) const
				{
					if (l && r)
					{
						l->traverse_data(fn);
						r->traverse_data(fn);
					}
					else
					{
						fn(data);
					}
				}
			};

		public:
			bsp(rng_type &rng, rectangle bounds, int min, int margin)
			{
				m_root.bounds = bounds;
				m_root.bounds.deflate(margin);
				m_root.split(rng, min, margin);
			}
			bsp(rng_type &rng, rectangle bounds, int min) : bsp(rng, bounds, min, 0)
			{
			}
			template <typename Operator> void enumerate(Operator &fn) const
			{
				m_root.traverse(fn);
			}
			template <typename Operator> void enumerate(Operator &fn)
			{
				m_root.traverse(fn);
			}
			template <typename Operator> void enumerate_bounds(Operator &fn) const
			{
				m_root.traverse_bounds(fn);
			}
			template <typename Operator> void enumerate_bounds(Operator &fn)
			{
				m_root.traverse_bounds(fn);
			}
			template <typename Operator> void enumerate_data(Operator &fn) const
			{
				m_root.traverse_data(fn);
			}
			template <typename Operator> void enumerate_data(Operator &fn)
			{
				m_root.traverse_data(fn);
			}
			unsigned int count() const noexcept
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