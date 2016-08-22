// name: environment.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "environment.hpp"

#include <px/core/terrain.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

namespace px
{
	namespace core
	{
		environment::environment(ui::stack_panel &ui, factory &builder, space_type &space)
			: m_ui(&ui), m_factory(&builder), m_space(&space)
			, m_terrain_director(m_world, builder)
			, m_terrain(m_terrain_director)
			, m_running(false)
		{
			compose_ui();
		}
		environment::~environment()
		{
		}

		terrain const* environment::scene() const
		{
			return &m_terrain;
		}
		terrain* environment::scene()
		{
			return &m_terrain;
		}

		int environment::distance(point2 a, point2 b) const
		{
			return a.king_distance(b);
		}
		int environment::reputation(body_component& a, body_component& b)
		{
			return m_factions.reputation(a.faction(), b.faction());
		}

		// space and actors querry

		bool environment::traversable(point2 position, rl::traverse layer) const
		{
			return m_terrain.traversable(position, layer) && !blocking(position, layer);
		}
		location_component* environment::blocking(point2 position, rl::traverse layer) const
		{
			location_component* blocking = nullptr;
			m_space->find(position.x(), position.y(), [&](int x, int y, location_component* component)
			{
				body_component* body = *component;
				if (body && !body->traversable(layer))
				{
					blocking = component;
				}
			});
			return blocking;
		}
		std::list<location_component*> environment::nearest(point2 postion, unsigned int radius)
		{
			std::list<location_component*> list;
			m_space->find(postion.x(), postion.y(), radius, [&](int x, int y, location_component* component)
			{
				list.push_back(component);
			});
			return list;
		}

	}
}