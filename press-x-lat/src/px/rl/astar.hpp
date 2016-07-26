// name: astar.h
// type: c++ header
// desc: pathfinding
// auth: is0urce

#ifndef PX_A_STAR_HPP
#define PX_A_STAR_HPP

#include <px/common/point.hpp>

#include <memory>
#include <list>
#include <set>
#include <tuple>
#include <functional>

namespace px
{
	class astar
	{
	public:
		class coord;
		typedef std::shared_ptr<coord> coord_ptr;

		class coord : public point2
		{
		public:
			double g_score;
			double f_score;
			coord_ptr came_from;

		public:
			coord(const point2 &p)
				: point2(p)
				, g_score(0)
				, f_score(0)
			{
			}
			coord(int x, int y)
				: point2(x, y)
				, g_score(0)
				, f_score(0)
			{
			}
			coord(point2 p, double heuristics)
				: point2(p)
				, g_score(0)
				, f_score(heuristics)
			{
			}
			coord(int x, int y, double heuristics)
				: point2(x, y)
				, g_score(0)
				, f_score(heuristics)
			{
			}

			void update(double gscore, double fscore, coord_ptr step)
			{
				g_score = gscore;
				f_score = fscore;
				came_from = step;
			}
		};

	private:
		class score_less
		{
		public:
			bool operator() (const coord_ptr a, const coord_ptr b) const
			{
				return (a->f_score) < (b->f_score);
			}
		};
		class lexicographical_less
		{
		public:
			bool operator() (const coord_ptr lhs, const coord_ptr rhs) const
			{
				return (lhs->x() < rhs->x()) || ((lhs->x() == rhs->x()) && (lhs->y() < rhs->y()));
			}
		};
		static double heuristic(const coord &a, const coord &b)
		{
			return a.distance(b);
		}
		static double distance(const coord &a, const coord &b)
		{
			return a.king_distance(b);
		}
		static void neighbours(const coord &c, std::list<coord_ptr> &list)
		{
			auto result = std::make_unique<std::list<coord_ptr>>();

			list.push_back(std::make_shared<coord>(c.x() + 1, c.y()));
			list.push_back(std::make_shared<coord>(c.x()    , c.y() + 1));
			list.push_back(std::make_shared<coord>(c.x() - 1, c.y()));
			list.push_back(std::make_shared<coord>(c.x()    , c.y() - 1));
			// diagonals
			list.push_back(std::make_shared<coord>(c.x() + 1, c.y() + 1));
			list.push_back(std::make_shared<coord>(c.x() + 1, c.y() - 1));
			list.push_back(std::make_shared<coord>(c.x() - 1, c.y() + 1));
			list.push_back(std::make_shared<coord>(c.x() - 1, c.y() - 1));
		}

		// reconstruct path and return it
		static void construct_path(coord_ptr last_step, std::list<point2> &path)
		{
			coord_ptr step = last_step;
			while (step)
			{
				if (!step->came_from) break; // stop there and not include starting point as part of path, so first step is list::front()
				path.emplace_front(step->x(), step->y());
				step = step->came_from;
			}
		}

	public:
		template <unsigned int _S, typename _TFn>
		static std::list<point2> find(int sx, int sy, int dx, int dy, _TFn traversable)
		{
			return find(point2(sx, sy), point2(dx, dy), traversable);
		}
		template <unsigned int _S, typename _TFn>
		static std::list<point2> find(point2 start, point2 finish, _TFn traversable)
		{
			unsigned int steps = _S;

			std::list<point2> path;
			std::multiset<coord_ptr, score_less> open;
			std::set<coord_ptr, lexicographical_less> closed;

			open.insert(std::make_shared<coord>(start, heuristic(start, finish)));
			while (steps > 0 && !open.empty())
			{
				auto top = open.begin();
				coord_ptr current = *top;

				if (*current == finish) // terminal
				{
					construct_path(current, path);
					return path;
				}

				open.erase(*top);
				closed.insert(current);

				std::list<coord_ptr> next;
				neighbours(*current, next);
				for (coord_ptr neighbor : next)
				{
					if (!traversable(*neighbor) && (*neighbor != finish)) continue;
					if (closed.find(neighbor) != closed.end()) continue;

					auto tentative = current->g_score + distance(*current, *neighbor);
					neighbor->update(tentative, tentative + heuristic(*neighbor, finish), current);

					bool find = false;
					for (auto it = open.begin(), last = open.end(); it != last; ++it)
					{
						if (**it == *neighbor)
						{
							find = true;
							if (neighbor->g_score < (*it)->g_score)
							{
								open.insert(open.erase(it), neighbor); // update with lesser key score
							}
							break;
						}
					}
					if (!find)
					{
						open.insert(neighbor);
					}
				}

				--steps;
			}
			// nothing found

			// construct path to best found vertice in border set if present (i.e. maybe there is a path & we're just reach step limit)
			//auto xi = open.begin();
			//if (xi != open.end())
			//{
			//	if (heuristic(**xi, finish) < heuristic(start, finish))
			//	{
			//		construct_path(*xi, path);
			//	}
			//}

			// not found
			return path;
		}
	};
}

#endif