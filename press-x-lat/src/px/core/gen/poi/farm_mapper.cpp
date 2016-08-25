// name: farm_mapper.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "farm_mapper.hpp"

#include <px/core/gen/build_result.hpp>

#include <px/core/unit_record.hpp>
#include <px/core/data/factory.hpp>

#include <px/core/sys/body_component.hpp>
#include <px/core/sys/character_component.hpp>

namespace px
{
	namespace core
	{
		farm_mapper::farm_mapper(factory &unit_builder) : m_factory(&unit_builder) {}
		farm_mapper::~farm_mapper() {}

		void farm_mapper::apply_mapping(build_result &build, map_type &terrain, units_list &units, bool generate_placeables) const
		{
			build.tiles.enumerate([&](auto const& location, auto const& blueprint_tile) {
				auto &tile = terrain[location];
				auto &img = tile.appearance();

				switch (blueprint_tile)
				{
				case build_tile::wall:
				case build_tile::wall_inside:
				case build_tile::wall_outside:
					tile.make_blocking();
					tile.make_opaque();
					img.glyph = 0x2317; // '#'
					img.tint = { 1, 1, 1 };
					img.bg = { 0, 0, 0 };
					break;
				case build_tile::floor:
					tile.make_traversable();
					tile.make_transparent();
					img.glyph = '.';
					img.tint = { 1, 1, 1 };
					img.bg = { 0, 0, 0 };
					break;
				case build_tile::door_ark:
					tile.make_traversable();
					tile.make_transparent();
					img.glyph = '-';
					img.tint = { 1, 1, 1 };
					img.bg = { 0, 0, 0 };
					break;
				case build_tile::soil:
					tile.make_traversable();
					tile.make_transparent();
					img.glyph = '"';
					img.tint = { 0, 1, 0 };
					img.bg = { 0.25, 0.25, 0.0 };
					break;
				case build_tile::water:
					tile.make_blocking();
					tile.make_transparent();
					img.glyph = '~';
					img.tint = { 0, 0, 1 };
					img.bg = { 0.0, 0.0, 0.25 };
					break;
				case build_tile::no_change:
					break;
				default:
					break;
				}
			});

			for (auto const& placeable : build.placeables)
			{
				switch (placeable.placeable)
				{
				case build_placeable::mobile:
					{
					// items
					auto weapon = std::make_shared<body_component::item_type>();
					weapon->add({ rl::effect::weapon_damage, 0x00, 1 });
					weapon->set_name("Copper Sword");
					weapon->set_tag("copper_sword");

					// composition
					auto task = m_factory->produce();
					auto img = task->add_appearance('f', { 1, 1, 0 });
					auto pawn = task->add_location(placeable.location);
					auto body = task->add_body(100, 100);
					auto character = task->add_character();
					auto npc = task->add_npc_behavior();

					// setup
					body->join_faction(1);
					body->equip_weapon(weapon);
					character->add_skill("melee");
					character->set_tag("farm_dweller");

					// add
					units.emplace_back(task->assemble(persistency::serialized), task->location());
					}
					break;
				case build_placeable::furniture:
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
				case build_placeable::table:
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
				default:
					break;
				}

			}
		}
	}
}