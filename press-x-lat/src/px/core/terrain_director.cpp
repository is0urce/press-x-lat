// name: terrain_director.cpp
// type: c++ header
// desc: class methods
// auth: is0urce

#include "terrain_director.hpp"

#include <px/common/matrix.hpp>
#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/fn/perlin.hpp>
#include <px/fn/bsp.hpp>

#include <px/core/image.hpp>
#include <px/core/data/factory.hpp>

#include <memory>
#include <list>
#include <random>
#include <numeric>

namespace px
{
	namespace core
	{
		namespace
		{
			static const unsigned int perlin_width = 7;
			static const unsigned int perlin_height = perlin_width;
			static const unsigned int perlin_samples = 9;
			typedef std::mt19937 rng_type;
		}

		void terrain_director::generate(unsigned int seed)
		{
			m_seed = seed;

			m_world->resize(settings::world_width, settings::world_height);
			m_world->generate(seed);
		}

		void terrain_director::generate_cell(const point2 &cell, map_type& terrain, std::list<unit_ptr>& units)
		{
			auto &c = m_world->map()->select(cell, m_outer);
			bool mobiles = c.generated;
			c.generated = true;

			generate_cell(cell, terrain, !mobiles, units);
		}
		void terrain_director::generate_cell(const point2 &cell, map_type& terrain, bool static_mobiles, std::list<unit_ptr>& units)
		{
			unsigned int seed = m_seed + cell.x() * 51 + cell.y() * cell_width * cell_height;

			std::array<unsigned int, 624> seed_data;
			std::iota(std::begin(seed_data), std::end(seed_data), seed);
			std::seed_seq state(std::begin(seed_data), std::end(seed_data));
			rng_type generator(state);
			generator.discard(rng_type::state_size);

			fn::perlin<perlin_width, perlin_height> noise(generator);

			double mx = static_cast<double>(perlin_width - 1) / cell_width;
			double my = static_cast<double>(perlin_height - 1) / cell_height;

			terrain.enumerate([&](int i, int j, auto& t)
			{
				auto magnitude = noise.sample(mx * i, my * j, perlin_samples);

				auto &img = t.appearance();
				if (magnitude > 0)
				{
					img.glyph = '.';
					img.tint = { 0, 1, 0 };
					t.make_traversable();
				}
				else
				{
					img.glyph = '#';
					img.tint = { 0.5, 0.5, 0.5 };
					t.make_traversable();
				}

				//if (i == 0) terrain[{i, j}].appearance().glyph = '|';
				//if (j == 0) terrain[{i, j}].appearance().glyph = '-';
				//if (i == 0 && j == 0) terrain[{i, j}].appearance().glyph = '+';

			});

			std::list<point2> room_center;

			fn::bsp<rng_type> buildings(generator, { { 0, 0 }, point2(cell_width, cell_height) }, 12);
			buildings.enumerate([&](const auto &building) {
				fn::bsp<rng_type> rooms(generator, building.bounds.deflated(1), 4);

				rooms.enumerate([&](const auto &room) {

					rectangle(room.bounds.start(), room.bounds.range() + point2(1, 1)).enumerate_bounds([&](int i, int j)	{
						auto& img = terrain[point2(i, j)].appearance();
						
						img.glyph = '#';
						img.tint = color::white();
					});
					room_center.push_back(room.bounds.start() + room.bounds.range() / 2);
				});
			});

			if (static_mobiles)
			{
				point2 spawn(3, 3);
				for (point2 spawn : room_center)
				{
					auto task = m_factory->produce();

					auto sprite = task->add_appearance('f', { 1, 0.0, 0 });
					auto pawn = task->add_location((cell * point2(cell_width, cell_height)) + spawn);
					auto body = task->add_body(100, 100);
					auto character = task->add_character();
					auto ai = task->add_npc_behavior();

					auto weapon = std::make_shared<body_component::item_type>();
					weapon->add({ rl::effect::weapon_damage, 0, 0 });

					body->equip_weapon(weapon);
					body->join_faction(0);
					character->add_skill("melee");
					character->set_tag("mob");

					units.push_back(task->assemble(persistency::serialized));
				}
			}
		}

		terrain_director::terrain_director(world &w, factory &factory)
			: m_world(&w), m_factory(&factory)
		{
			m_outer.generated = true; // no additional generation for out-of-borders cells
		}
		terrain_director::~terrain_director()
		{
		}

		const world::map_type* terrain_director::map() const
		{
			return m_world->map();
		}
		world::map_type* terrain_director::map()
		{
			return m_world->map();
		}
	}
}