// name: rural_mapper.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "rural_mapper.hpp"

#include <px/core/unit_record.hpp>
#include <px/core/data/factory.hpp>

#include <px/core/sys/body_component.hpp>
#include <px/core/sys/character_component.hpp>

namespace px
{
	namespace core
	{
		rural_mapper::rural_mapper(factory &unit_builder) : m_factory(&unit_builder)
		{
			map_type::element wall({ false }, { '#',{ 1.0, 1.0, 1.0 },{ 0.0, 0.0, 0.0 } });

			add_rule(fn::build_tile::soil, { { true, true }, { '"', { 0.0, 1.0, 0.0 }, { 0.25, 0.25, 0.0 } } });
			add_rule(fn::build_tile::wall, wall);
			add_rule(fn::build_tile::wall_inside, wall);
			add_rule(fn::build_tile::wall_outside, wall);
			add_rule(fn::build_tile::floor, { { true, true }, { '.',{ 1.0, 1.0, 1.0 },{ 0.0, 0.0, 0.0 } } });
			add_rule(fn::build_tile::door_ark, { { true, true }, { '-',{ 1.0, 1.0, 1.0 },{ 0.0, 0.0, 0.0 } } });
			add_rule(fn::build_tile::gravel, { { true, true },{ '.',{ 0.0, 1.0, 0.0 },{ 0.25, 0.25, 0.0 } } });
			add_rule(fn::build_tile::water, { { true },{ '.',{ 0.0, 1.0, 0.0 },{ 0.25, 0.25, 0.0 } } });
			//add_rule(fn::build_tile::doodad_tombstone, { { true },{ '+',{ 0.0, 1.0, 0.0 },{ 0.25, 0.25, 0.0 } } });
		}
		rural_mapper::~rural_mapper() {}

		void rural_mapper::apply_mapping(fn::build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const
		{
			map_mapper::apply_mapping(build, terrain, units, generate_placeables);

			build.tiles.enumerate([&](auto const& location, auto const& blueprint_tile) {
				auto &tile = terrain[location];
				auto &img = tile.appearance();

				switch (blueprint_tile.tile)
				{
				case fn::build_tile::doodad_tombstone:
					tile.make_blocking();
					tile.make_transparent();
					img.glyph = '+'; // cross
					img.tint = 0x999999;
					break;
				case fn::build_tile::no_change:
					break;
				default:
					break;
				}
			});

			for (auto const& placeable : build.placeables)
			{
				switch (placeable.placeable)
				{
				case fn::build_placeable::mobile:
					{
					// items
					auto weapon = std::make_shared<body_component::item_type>();
					weapon->add({ rl::effect::weapon_damage, 0x00, 1 });
					weapon->set_name("Copper Sword");
					weapon->set_tag("copper_sword");

					// composition
					auto task = m_factory->produce();
					auto img = task->add_appearance('f', 0xccccff);
					auto pawn = task->add_location(placeable.location);
					auto body = task->add_body(100, 100);
					auto character = task->add_character();
					auto npc = task->add_npc_behavior();

					// setup
					body->join_faction(1);
					body->equip_weapon(weapon);
					character->add_skill("sk_melee");

					// add
					units.emplace_back(task->assemble(persistency::serialized), task->location());
					}
					break;
				case fn::build_placeable::animal_domestic:
				{
					// items
					auto weapon = std::make_shared<body_component::item_type>();
					weapon->add({ rl::effect::weapon_damage, 0x00, 1 });
					weapon->set_name("Copper Sword");
					weapon->set_tag("copper_sword");

					// composition
					auto task = m_factory->produce();
					auto img = task->add_appearance('m', { 1, 1, 0 });
					auto pawn = task->add_location(placeable.location);
					auto body = task->add_body(100, 100);
					auto character = task->add_character();
					auto npc = task->add_npc_behavior();

					// setup
					body->join_faction(1);
					body->equip_weapon(weapon);
					character->add_skill("sk_melee");

					// add
					units.emplace_back(task->assemble(persistency::serialized), task->location());
				}
				break;
				case fn::build_placeable::furniture_bed:
				case fn::build_placeable::furniture_chair:
				case fn::build_placeable::furniture_shelf:
				case fn::build_placeable::furniture:
					{
						// composition
						auto task = m_factory->produce();
						auto img = task->add_appearance('h', { 0.5, 0.5, 0.5 });
						auto pawn = task->add_location(placeable.location);
						auto body = task->add_body();

						// setup
						body->join_faction(0);

						// add
						units.emplace_back(task->assemble(persistency::serialized), task->location());
					}
					break;
				case fn::build_placeable::furniture_table:
					{
						// composition
						auto task = m_factory->produce();
						auto img = task->add_appearance('t', { 0.5, 0.5, 0.5 });
						auto pawn = task->add_location(placeable.location);
						auto body = task->add_body();

						// setup
						body->join_faction(0);

						// add
						units.emplace_back(task->assemble(persistency::serialized), task->location());
					}
					break;
				case fn::build_placeable::furniture_barrel:
					{
						// composition
						auto task = m_factory->produce();
						auto img = task->add_appearance('%', { 0.5, 0.5, 0.5 });
						auto pawn = task->add_location(placeable.location);
						auto body = task->add_body();

						// setup
						body->join_faction(0);

						// add
						units.emplace_back(task->assemble(persistency::serialized), task->location());
					}
					break;
				default:
					break;
				}
			}
		}
	}
}