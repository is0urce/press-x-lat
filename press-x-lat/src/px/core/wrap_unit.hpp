// name: lua_unit.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_LUA_WRAP_UNIT_HPP
#define PX_CORE_LUA_WRAP_UNIT_HPP

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

namespace px
{
	namespace core
	{
		class wrap_unit final
		{
		public:
			void damage(int dmg)
			{
				update();
				if (m_body)
				{
					if (auto hp = m_body->health())
					{
						hp->damage(dmg);
					}
				}
			}
			void restore(int val)
			{
				update();
				if (m_body)
				{
					if (auto hp = m_body->health())
					{
						hp->restore(val);
					}
				}
			}
			void drain(int mana)
			{
				update();
				if (m_body)
				{
					if (auto mp = m_body->energy())
					{
						mp->damage(mana);
					}
				}
			}
			void replenish(int mana)
			{
				update();
				if (m_body)
				{
					if (auto mp = m_body->energy())
					{
						mp->restore(mana);
					}
				}
			}
			bool dead()
			{
				update();
				return m_body ? m_body->dead() : false;
			}
			int energy()
			{
				update();
				int energy = 0;
				if (m_body)
				{
					auto mp = m_body->energy();
					energy += mp ? mp->current() : 0;
				}
				return energy;
			}
			int energy_max()
			{
				update();
				int energy = 0;
				if (m_body)
				{
					auto mp = m_body->energy();
					energy += mp ? mp->maximum() : 0;
				}
				return energy;
			}
			int health()
			{
				update();
				int health = 0;
				if (m_body)
				{
					auto hp = m_body->health();
					health += hp ? hp->current() : 0;
				}
				return health;
			}
			int health_max()
			{
				update();
				int health = 0;
				if (m_body)
				{
					auto hp = m_body->health();
					health = hp ? hp->maximum() : 0;
				}
				return health;
			}
			void move(point2 destination)
			{
				if (m_location)
				{
					m_location->move(destination);
				}
			}

			bool valid() const
			{
				return m_location != nullptr;
			}
			point2 position() const
			{
				return m_location ? m_location->current() : point2(0, 0);
			}

			body_component* body()
			{
				update();
				return m_body;
			}
			location_component* location()
			{
				return m_location;
			}

		public:
			~wrap_unit() {}
			wrap_unit(location_component* pawn)
				: m_location(pawn), m_body(nullptr)
			{
			}
			wrap_unit(wrap_unit&& other) : wrap_unit(nullptr)
			{
				std::swap(m_location, other.m_location);
				std::swap(m_body, other.m_body);
			}
			wrap_unit(wrap_unit const&) = default;
			wrap_unit& operator=(wrap_unit const&) = default;

		private:
			void update()
			{
				if (!m_body && m_location)
				{
					m_body = *m_location;
				}
			}

		private:
			location_component* m_location;
			body_component *m_body;
		};
	}
}

#endif