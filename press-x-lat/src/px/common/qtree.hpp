// name: qtree.hpp
// type: c++ header
// desc: class
// auth: is0urce

// quad tree space partition
// template with element type
// multiple elements can occupy same spot

#ifndef PX_COMMON_QTREE_H
#define PX_COMMON_QTREE_H

#include <list>
#include <memory>
#include <functional>
#include <stdexcept>

#include <px/common/point.hpp>

namespace px
{
	template<typename _E>
	class qtree
	{
	public:
		typedef _E element;
		typedef std::unique_ptr<qtree> ptr;

		struct bucket
		{
		private:
			std::list<element> list;
			int m_x;
			int m_y;
		public:
			bool match(int x0, int y0) const
			{
				return m_x == x0 && m_y == y0;
			}
			unsigned int size() const
			{
				return list.size();
			}
			bool empty() const
			{
				return list.empty();
			}
			void add(element e, int x, int y)
			{
				list.push_back(e);
				m_x = x;
				m_y = y;
			}
			bool remove(element e)
			{
				for (auto it = list.begin(), last = list.end(); it != last;)
				{
					if (*it == e)
					{
						it = list.erase(it);
						return true;
					}
					++it;
				}
				return false;
			}
			int x() const
			{
				return m_x;
			}
			int y() const
			{
				return m_y;
			}
		};

	private:
		ptr nw, ne, se, sw; // leaves

		// partition data
		int m_center_x;
		int m_center_y;
		unsigned int m_range;

		std::unique_ptr<bucket> m_bucket; // value, not null if it is a leaf

	public:
		qtree(int x, int y, unsigned int range)
			: m_center_x(x)
			, m_center_y(y)
			, m_range(range)
			, m_bucket(std::make_unique<bucket>())
		{
		}
		qtree(unsigned int range) : qtree(0, 0, range)
		{
		}
		qtree(const qtree&) = delete;
		qtree(qtree&& that)
		{
			std::swap(m_center_x, that.m_center_x);
			std::swap(m_center_y, that.m_center_y);
			std::swap(m_range, that.m_range);
			std::swap(m_bucket, that.m_bucket);
		}

	private:
		// select branch for specified coordinates
		// internal garantee coordinates are in quad
		ptr& select(int x, int y)
		{
			if (x >= m_center_x)
			{
				return y >= m_center_y ? ne : se;
			}
			else
			{
				return y >= m_center_y ? nw : sw;
			}
		}
		const ptr& select(int x, int y) const
		{
			if (x >= m_center_x)
			{
				return y >= m_center_y ? ne : se;
			}
			else
			{
				return y >= m_center_y ? nw : sw;
			}
		}

		// select branch, if none available, create one
		// souldn't used by itself on a leaves as it interfere with hierarchy leaves detection
		ptr& access(int x, int y)
		{
			auto &branch = select(x, y);
			if (!branch)
			{
				int range = m_range / 2;
				auto result = std::make_unique<qtree>(m_center_x + ((x >= m_center_x) ? range : -range), m_center_y + ((y >= m_center_y) ? range : -range), range);
				std::swap(branch, result);
			}
			return branch;
		}

		// partition current leaf
		void split()
		{
			if (!m_bucket) throw std::runtime_error("px::qtree:split() - can't split non-leaf");

			auto &branch = access(m_bucket->x(), m_bucket->y());
			std::swap(m_bucket, branch->m_bucket);
			m_bucket.release();
		}

		// empty or not-created leaf
		bool static is_empty(const ptr& tree)
		{
			return !tree || tree && tree->m_bucket && tree->m_bucket->empty();
		}

		// opposite command to split
		// combine leaves back
		// all branches must be leaves
		// only one branch can have elements - collapse it and put that branch on top
		bool combine(ptr& cleaned)
		{
			if (!cleaned->m_bucket) throw std::runtime_error("can't collapse at non-leaf");

			bool enw = is_empty(nw);
			bool ene = is_empty(ne);
			bool esw = is_empty(sw);
			bool ese = is_empty(se);

			if (nw && nw->m_bucket && ene && esw && ese)
			{
				std::swap(m_bucket, nw->m_bucket);
				nw.reset();
				return true;
			}
			if (ne && ne->m_bucket && enw && esw && ese)
			{
				std::swap(m_bucket, ne->m_bucket);
				ne.reset();
				return true;
			}
			if (sw && sw->m_bucket && ene && enw && ese)
			{
				std::swap(m_bucket, sw->m_bucket);
				sw.reset();
				return true;
			}
			if (se && se->m_bucket && ene && esw && enw)
			{
				std::swap(m_bucket, se->m_bucket);
				se.reset();
				return true;
			}
			return false;
		}

		// helper function for expansion
		// old - part for expanding (internal not null garantee)
		// expansion - bigger area, not null
		// update - area in bigger area for swap to old, is null
		void static expand(ptr& old, ptr& expansion, ptr& update)
		{
			expansion->m_bucket.reset();
			if (old->m_bucket)
			{
				std::swap(old->m_bucket, expansion->m_bucket);
				old->m_bucket.reset();
			}
			else
			{
				std::swap(update, old);
			}
			std::swap(old, expansion);
		}

		// expand range
		void expand()
		{
			int range = m_range;
			if (nw)
			{
				auto expand = std::make_unique<qtree>(m_center_x - range, m_center_y + range, range);
				qtree::expand(nw, expand, expand->se);
			}
			if (ne)
			{
				auto expand = std::make_unique<qtree>(m_center_x + range, m_center_y + range, range);
				qtree::expand(ne, expand, expand->sw);
			}
			if (sw)
			{
				auto expand = std::make_unique<qtree>(m_center_x - range, m_center_y - range, range);
				qtree::expand(sw, expand, expand->ne);
			}
			if (se)
			{
				auto expand = std::make_unique<qtree>(m_center_x + range, m_center_y - range, range);
				qtree::expand(se, expand, expand->nw);
			}
			m_range *= 2;
		}

		// adding element, internal guarantee x and y in range
		void insert(int x, int y, element e)
		{
			if (m_bucket) // it is a leaf
			{
				if (m_bucket->empty() || m_bucket->match(x, y))
				{
					m_bucket->add(e, x, y);
				}
				else
				{
					// partition and try again
					split();
					insert(x, y, e);
				}
			}
			else
			{
				auto &branch = access(x, y);
				branch->insert(x, y, e);
			}
		}
		bool erase(int x, int y, element e)
		{
			if (m_bucket)
			{
				if (!m_bucket->match(x, y)) throw std::runtime_error("px::qtree::remove - bucket coordinates not match");
				if (!m_bucket->remove(e)) throw std::runtime_error("px::qtree::remove - item not found");

				return true;
			}
			else
			{
				auto &branch = select(x, y);

				if (!branch) throw std::runtime_error("px::qtree::remove - no branch");

				if (branch->erase(x, y, e))
				{
					return combine(branch); // try to optimize
				}
				return false;
			}
		}
		qtree* move_hint(int sx, int sy, element e, int dx, int dy)
		{
			if (m_bucket)
			{
				if (!m_bucket->match(sx, sy)) throw std::runtime_error("px::qtree::move_hint - bucket coordinates not match");
				if (!m_bucket->remove(e)) throw std::runtime_error("px::qtree::move_hint - item not found");

				add(dx, dy, e);
				return this;
			}
			else
			{
				auto &branch = select(sx, sy);

				if (!branch) throw std::runtime_error("px::qtree::move_hint - no branch");

				if (branch->contains(sx, sy))
				{
					return branch->move_hint(sx, sy, e, dx, dy);
				}
				else
				{
					erase(sx, sy, e);
					return this;
				}
			}
		}

	public:
		// this partition contains point in range (there could be no elements)
		bool contains(int x, int y) const
		{
			return x >= m_center_x - (int)m_range
				&& x < m_center_x + (int)m_range
				&& y >= m_center_y - (int)m_range
				&& y < m_center_y + (int)m_range;
		}
		bool leaf() const
		{
			return m_bucket;
		}

		void add(int x, int y, element e)
		{
			while (!contains(x, y))
			{
				expand();
			}
			insert(x, y, e);
		}
		void remove(int x, int y, element e)
		{
			erase(x, y, e);
		}

		// _Op fn sould match bool(int, int, _C)
		template <typename _Op>
		void find(int x, int y, unsigned int radius, _Op fn) const
		{
			if (m_bucket)
			{
				int cx = m_bucket->x();
				int cy = m_bucket->y();
				if (std::abs(cx - x) <= (int)radius && std::abs(cy - y) <= (int)radius)
				{
					for (auto it = m_bucket->list.begin(), last = m_bucket->list.end(); it != last; ++it)
					{
						if (!fn(cx, cy, *it)) return;
					}
				}
			}
			else
			{
				bool w = x - (int)radius <= m_center_x;
				bool e = x + (int)radius >= m_center_x;
				bool n = y + (int)radius >= m_center_y;
				bool s = y - (int)radius <= m_center_y;
				if (n)
				{
					if (w && nw) nw->find(x, y, radius, fn);
					if (e && ne) ne->find(x, y, radius, fn);
				}
				if (s)
				{
					if (w && sw) sw->find(x, y, radius, fn);
					if (e && se) se->find(x, y, radius, fn);
				}
			}
		}
		template <typename _Op>
		void find(int x, int y, _Op fn) const
		{
			if (m_bucket && m_bucket.match(x, y))
			{
				for (auto it = m_bucket->list.begin(), last = m_bucket->list.end(); it != last; ++it)
				{
					if (!fn(cx, cy, *it)) return;
				}
			}
			else
			{
				bool w = x <= m_center_x;
				bool e = x >= m_center_x;
				bool n = y >= m_center_y;
				bool s = y <= m_center_y;
				if (n)
				{
					if (w && nw) nw->find(x, y, radius, fn);
					if (e && ne) ne->find(x, y, radius, fn);
				}
				if (s)
				{
					if (w && sw) sw->find(x, y, radius, fn);
					if (e && se) se->find(x, y, radius, fn);
				}
			}
		}
		bool exists(int x, int y) const
		{
			if (m_bucket)
			{
				return !m_bucket->empty() && m_bucket->match(x, y);
			}
			else
			{
				const auto &branch = select(x, y);
				return branch ? branch->exists(x, y) : false;
			}
		}
		void move(int sx, int sy, element e, int dx, int dy)
		{
			move_hint(sx, sy, e, dx, dy)->add(dx, dy, e);
		}

		void move(point2 from, element e, point2 destination)
		{
			move(from.x(), from.y(), e, destination.x(), destination.y());
		}
		void add(point2 position, element e)
		{
			add(position.x(), position.y(), e);
		}
		void remove(point2 position, element e)
		{
			remove(position.x(), position.y(), e);
		}

		std::string info() const
		{
			auto result = std::string("qT(") + std::to_string(m_center_x) + std::string(",") + std::to_string(m_center_y) + std::string(")-") + std::to_string(m_range);
			if (m_bucket) result += std::string(":") + std::to_string(m_bucket->size());
			if (nw) result += std::string(" nw=") + nw->info();
			if (ne) result += std::string(" ne=") + ne->info();
			if (sw) result += std::string(" sw=") + sw->info();
			if (se) result += std::string(" se=") + se->info();
			result += ";";
			return result;
		}
	};
}

#endif