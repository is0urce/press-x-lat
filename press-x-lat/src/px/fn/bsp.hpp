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
				rectangle bounds;
				user_type data;
				std::unique_ptr<node> l;
				std::unique_ptr<node> r;

				bool leaf() const
				{
					return (l && r);
				}
				void split(rng_type &rng, int min)
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

						l->split(rng, min);
						r->split(rng, min);
					}
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
			//class iterator
			//{
			//	node* operator*() { return m_branch == 0 ? m_node->l : m_node->r; }
			//	iterator& operator++()
			//	{
			//		++m_branch;
			//		if (m_branch > 1)
			//		{
			//			iterator* next = m_upper;
			//			while (m_next && m_next.branch > 1)
			//			{
			//				m_next = m_next->upper;
			//			}
			//			
			//			iterator i;
			//			i.m_upper = m_next;////
			//		}
			//		return *this;
			//	}

			//	iterator() : m_node(nullptr), m_upper(nullptr), m_branch(0) {}
			//	iterator(const iterator&) = default;
			//	iterator& operator=(iterator&) = default;
			//	iterator(iterator&& s) : iterator() { std::swap(m_node, s.m_node); std::swap(m_upper, s.m_upper); std::swap(m_branch, s.m_branch); }

			//	node* m_node;
			//	iterator* m_upper;
			//	int m_branch;
			//};
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

		private:
			node m_root;
		};
	}
}

#endif