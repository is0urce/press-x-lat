// name: world.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_WORLD_H
#define PX_CORE_WORLD_H

#include <px/core/gen/world_cell.hpp>

#include <px/common/matrix.hpp>
#include <px/rl/tile.hpp>
#include <px/rl/traverse.hpp>
#include <px/rl/map_stream.hpp>
#include <px/fn/perlin.hpp>

#include <px/core/image.hpp>

#include <px/core/data/factory.hpp>

#include <memory>
#include <list>
#include <random>
#include <cstdint>
#include <algorithm>

namespace px
{
	namespace core
	{
		class world
		{
		public:
			static const unsigned int world_width = 10;
			static const unsigned int world_height = world_width;
			static const unsigned int cell_width = 100;
			static const unsigned int cell_height = cell_width;
			static const unsigned int perlin_width = 6;
			static const unsigned int perlin_height = perlin_width;
			static const unsigned int perlin_samples = 1;

		public:
			typedef std::shared_ptr<unit> unit_ptr;
			typedef rl::tile<image> tile_type;
			typedef matrix2<tile_type, cell_width, cell_height> local_map_type;
			typedef matrix2<world_cell, world_width, world_height> world_map_type;
			typedef std::mersenne_twister_engine<std::uint_fast32_t, 32, 624, 397, 31,
				0x9908b0df, 11,
				0xffffffff, 7,
				0x9d2c5680, 15,
				0xefc60000, 18, 1812433253> rng;

		public:
			void generate(unsigned int seed)
			{
			}

			void arrange(const point2 &cell, local_map_type& terrain, std::list<unit_ptr>& units)
			{
				world_cell &c = m_map.select(cell, m_outer);
				bool mobiles = c.generated;
				c.generated = true;

				generate(cell, terrain, !mobiles, units);
			}
			void generate(const point2 &cell, local_map_type& terrain, bool static_mobiles, std::list<unit_ptr>& units)
			{
				rng generator(m_seed + cell.x() + cell.y() * cell_width * cell_height);
				std::uniform_real_distribution<double> distribution;
				fn::perlin<perlin_width, perlin_height> noise([&]() {return distribution(generator); });

				double mx = static_cast<double>(perlin_width) / cell_width;
				double my = static_cast<double>(perlin_height) / cell_height;

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
#ifdef _DEBUG
				const char* digits = "0123456789ABCDEF";
				terrain[{1, 1}].appearance().glyph = digits[cell.x() - 10 * size_t((std::floor)(cell.x() / 10.0))];
				terrain[{2, 1}].appearance().glyph = ':';
				terrain[{3, 1}].appearance().glyph = digits[cell.y() - 10 * size_t((std::floor)(cell.y() / 10.0))];
#endif

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
			void store(unit_ptr unit)
			{

			}

		public:
			world(factory &factory) : m_factory(&factory), m_seed(0)
			{
				m_outer.generated = true; // no additional generation for out-of-borders cells

				generate(m_seed);
			}
			virtual ~world()
			{
			}
			world(const world&) = delete;

		private:
			unsigned int m_seed;
			world_map_type m_created;
			factory* m_factory;
			world_map_type m_map;
			world_cell m_outer;
		};
	}
}

#endif