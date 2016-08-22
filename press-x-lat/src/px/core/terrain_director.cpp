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

#include <px/core/unit_record.hpp>
#include <px/core/image.hpp>
#include <px/core/data/factory.hpp>
#include <px/core/gen/landmark.hpp>

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

		void terrain_director::generate_cell(point2 const& world_position, map_type &terrain, unit_list &units)
		{
			auto &world_cell = m_world->map()->select(world_position, m_outer);

			bool mobiles = world_cell.generated;
			world_cell.generated = true;

			generate_cell(world_position, terrain, units, !mobiles);

			if (auto &landmark = world_cell.landmark)
			{
				landmark->generate(world_cell.seed, terrain, units, !mobiles);
			}
		}
		void terrain_director::generate_cell(point2 const& world_position, map_type &terrain, unit_list &units, bool static_mobiles)
		{
			unsigned int seed = m_seed + world_position.x() * 51 + world_position.y() * terrain.width() * terrain.height();

			std::array<unsigned int, 624> seed_data;
			std::iota(std::begin(seed_data), std::end(seed_data), seed);
			std::seed_seq state(std::begin(seed_data), std::end(seed_data));
			rng_type generator(state);
			generator.discard(rng_type::state_size);

			fn::perlin<perlin_width, perlin_height> noise(generator);

			double mx = static_cast<double>(perlin_width - 1) / terrain.width();
			double my = static_cast<double>(perlin_height - 1) / terrain.height();

			terrain.enumerate([&](auto const& location, auto& t)
			{
				auto magnitude = noise.sample(mx * location.x(), my * location.y(), perlin_samples);

				auto &img = t.appearance();
				if (magnitude > 0)
				{
					img.glyph = '.';
					img.tint = { 1, 1, 1 };
					img.bg = { 0, 0.25, 0 };
					t.make_traversable();
				}
				else
				{
					img.glyph = '^';
					img.tint = { 0.5, 0.5, 0.5 };
					img.bg = { 0, 0, 0 };
					t.make_traversable();
				}

				//if (location.x() == 0) terrain[location].appearance().glyph = '|';
				//if (location.y() == 0) terrain[location].appearance().glyph = '-';
				//if (location.empty()) terrain[location].appearance().glyph = '+';
			});

			//if (static_mobiles)
			//{
			//	for (point2 spawn : room_center)
			//	{
			//		auto task = m_factory->produce();

			//		auto sprite = task->add_appearance('f', { 1, 0, 0 });
			//		auto pawn = task->add_location(spawn);
			//		auto body = task->add_body(100, 100);
			//		auto character = task->add_character();
			//		auto ai = task->add_npc_behavior();

			//		auto weapon = std::make_shared<body_component::item_type>();
			//		weapon->add({ rl::effect::weapon_damage, 0x00, 1 });

			//		body->equip_weapon(weapon);
			//		body->join_faction(1);
			//		character->add_skill("melee");
			//		character->set_tag("mob");

			//		units.emplace_back(task->assemble(persistency::serialized), task->location());
			//	}
			//}
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