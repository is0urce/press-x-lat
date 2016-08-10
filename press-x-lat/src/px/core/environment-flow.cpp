// name: environment-flow.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "environment.hpp"

#include <px/core/ui/map_panel.hpp>

namespace px
{
	namespace core
	{
		void environment::start(terrain &tiles, space_type &space, world &map)
		{
			end();

			m_terrain = &tiles;
			m_space = &space;
			m_world = &map;
			m_time = 0;
			m_running = true;

			m_map->bind(map);

			m_world->generate(0);
		}
		void environment::end()
		{
			if (!m_running) return;

			m_running = false;
			m_terrain = nullptr;
			m_space = nullptr;

			m_map->tear();
		}
		bool environment::running() const
		{
			return m_running;
		}
		environment::time_type environment::time() const
		{
			return m_time;
		}
		void environment::turn()
		{
			focus();
			++m_time;

			m_terrain->enumerate([](auto unit) {
				if (location_component* pawn = unit->location())
				{
					if (body_component* body = *pawn)
					{
						if (body->dead() && body->empty() && unit->get_persistency() != persistency::destroying)
						{
							unit->destroy(5);
						}
					}
				}
			});
		}

		// player
		environment::player_type environment::player()
		{
			return m_player;
		}
		const environment::player_type environment::player() const
		{
			return m_player;
		}
		void environment::impersonate(environment::player_type unit)
		{
			m_player = unit;
			focus();
			if (unit)
			{
				assign_inventory(unit->linked());
			}
		}
		void environment::focus()
		{
			if (m_player)
			{
				m_terrain->focus(m_player->current());
			}
		}
	}
}