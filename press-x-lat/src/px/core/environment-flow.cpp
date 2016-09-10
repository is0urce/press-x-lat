// name: environment-flow.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "environment.hpp"

#include <px/core/terrain.hpp>

#include <px/core/gen/world.hpp>
#include <px/core/data/factory.hpp>

#include <px/core/unit.hpp>
#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>
#include <px/core/sys/character_component.hpp>

#include <px/core/terrain_director.hpp>

namespace px
{
	namespace core
	{
		void environment::start()
		{
			end();

			m_time = 0;
			m_running = true;

			m_world.resize(settings::world_width, settings::world_height);
			m_world.generate(m_settings.seed());

			spawn_player({ 1, 1 });

			// ui
			tie_map();
			m_ui->deactivate("title");
			m_ui->activate("ingame");

			// make chest
			auto chest = m_factory->produce();
			chest->add_appearance('$', { 1, 1, 1 });
			chest->add_location({ 2, 2 });
			chest->add_body();
			chest->add_container();
			m_terrain.add(chest->assemble(persistency::serialized));
		}
		void environment::end()
		{
			if (!m_running) return;

			m_running = false;

			untie_map();

			m_world.clear();
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

			m_terrain.enumerate([](auto &unit) {
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
		void environment::spawn_player(point2 location)
		{
			// player props
			auto weapon = std::make_shared<body_component::item_type>();
			weapon->add({ rl::effect::weapon_damage, 0x00, 50 });
			weapon->set_name("Copper Sword");
			weapon->set_tag("copper_sword");

			// make player
			auto task = m_factory->produce();
			auto img = task->add_appearance('@', { 1, 1, 1 });
			auto pawn = task->add_location(location);
			auto body = task->add_body(100, 100);
			auto character = task->add_character();
			// setup
			body->join_faction(1);
			body->equip_weapon(weapon);
			character->add_skill("melee");
			character->set_tag("player");
			// add
			auto player = task->assemble(persistency::permanent);
			m_terrain.add(player);

			impersonate(pawn);
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
				m_terrain.focus(m_player->current());
			}
		}
	}
}