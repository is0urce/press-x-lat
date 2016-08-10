// name: world.cpp
// type: c++ header
// desc: class methods
// auth: is0urce

#include "world.hpp"

#include <px/common/matrix.hpp>
#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/fn/perlin.hpp>

#include <px/core/image.hpp>
#include <px/core/data/factory.hpp>

#include <px/core/world_generator.hpp>

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
			typedef std::mt19937 rng;
		}

		void world::generate(unsigned int seed)
		{
			m_seed = seed;

			m_map.resize(world_width, world_height);
			world_generator generator(m_map, seed);
		}

		void world::generate_cell(const point2 &cell, local_map_type& terrain, std::list<unit_ptr>& units)
		{
			world_cell &c = m_map.select(cell, m_outer);
			bool mobiles = c.generated;
			c.generated = true;

			generate_cell(cell, terrain, !mobiles, units);
		}
		void world::generate_cell(const point2 &cell, local_map_type& terrain, bool static_mobiles, std::list<unit_ptr>& units)
		{
			unsigned int seed = m_seed + cell.x() * 51 + cell.y() * cell_width * cell_height;

			std::array<unsigned int, 624> seed_data;
			std::iota(std::begin(seed_data), std::end(seed_data), seed);
			std::seed_seq state(std::begin(seed_data), std::end(seed_data));

			rng generator(state);
			generator.discard(rng::state_size);
			fn::perlin<perlin_width, perlin_height> noise([generator, distribution = std::uniform_real_distribution<double>()]() mutable {return distribution(generator); });

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

				if (i == 0) terrain[{i, j}].appearance().glyph = '|';
				if (j == 0) terrain[{i, j}].appearance().glyph = '-';
				if (i == 0 && j == 0) terrain[{i, j}].appearance().glyph = '+';

			});

			if (static_mobiles)
			{
				auto task = m_factory->produce();

				auto sprite = task->add_appearance('g', { 1, 0, 0 });
				auto pawn = task->add_location(cell * point2(cell_width, cell_height) + point2(3, 3));
				auto body = task->add_body(100, 100);
				auto character = task->add_character();
				auto ai = task->add_npc_behavior();

				auto weapon = std::make_shared<body_component::item_type>();
				weapon->add({ rl::effect::weapon_damage, 0, 1 });
				auto loot = std::make_shared<body_component::item_type>();
				loot->add({ rl::effect::ore_power, 0x00, 50 });
				loot->set_name("Ore");
				loot->set_tag("common_ore");

				body->equip_weapon(weapon);
				body->join_faction(0);
				body->add(loot);
				character->add_skill("melee");

				units.push_back(task->assemble(persistency::serialized));
			}
		}
		void world::store(unit_ptr unit)
		{

		}

		world::world(factory &factory) : m_factory(&factory)
		{
			m_outer.generated = true; // no additional generation for out-of-borders cells
		}
		world::~world()
		{
		}
	}
}