// name: environment.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// should be facade for interaction with game and place for script bindings

#ifndef PX_CORE_ENVIRONMENT_HPP
#define PX_CORE_ENVIRONMENT_HPP

#include <px/ui/stack_panel.hpp>

#include <px/core/world_settings.hpp>
#include <px/core/gen/world.hpp>
#include <px/core/terrain_director.hpp>
#include <px/core/terrain.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

#include <px/rl/faction_relation.hpp>
#include <px/rl/traverse.hpp>

#include <px/common/fps_counter.hpp>
#include <px/common/qtree.hpp>
#include <px/common/toggle.hpp>

#include <memory>
#include <tuple>

namespace px
{
	namespace core
	{
		//class location_component;
		//class body_component;

		class factory;

		class inventory_panel;
		class container_panel;
		class anvil_panel;
		class map_panel;

		class wrap_unit;

		class environment : public toggle<true>
		{
		public:
			typedef unsigned int time_type;
			typedef qtree<location_component*> space_type;
			typedef std::shared_ptr<location_component> player_type;

		public:
			// game flow

			void start();
			void end();
			bool running() const;
			time_type time() const;
			void turn();

			// props querry

			terrain const* scene() const;
			terrain* scene();
			int distance(point2 const& a, point2 const& b) const;
			int reputation(body_component& a, body_component& b) const;
			int aspect(world_aspect aspect) const;

			// player

			player_type player();
			const player_type player() const;
			void spawn_player(point2 location);
			void impersonate(player_type unit);

			// actions

			bool maneuver(location_component& location, point2 target);
			bool cast(location_component& source, unsigned int slot, location_component& target_location);
			bool cast(location_component& source, unsigned int slot, point2 target);
			bool activate(location_component& source, point2 position);
			bool equip(body_component& body, body_component::item_ptr item);
			bool unequip(body_component& body, body_component::item_ptr item);

			// space and actors querry

			bool traversable(point2 position, rl::traverse layer) const;
			location_component* blocking(point2 position, rl::traverse layer) const;
			std::list<location_component*> nearest(point2 postion, unsigned int radius);

			// interface relation

			void compose_ui();
			void target(point2 location);
			point2 targeted() const;
			void open_workshop(std::shared_ptr<body_component> user);
			void open_container(std::shared_ptr<body_component> user, std::shared_ptr<body_component> container);
			void assign_inventory(std::shared_ptr<body_component> body);

			// script bindings (implementation in "environment-bind.cpp")

			// damage, damage_type, is_hit, is_critical
			std::tuple<int, unsigned int, bool, bool> hit(wrap_unit &attacker, wrap_unit &target);
			int distance_wrap(point2 const& from, point2 const& to);
			int reputation_wrap(wrap_unit &unit, wrap_unit &target);
			void message(wrap_unit &target, std::string text);
			wrap_unit first_in_area(point2 const& location, int radius, bool require_fov);
			wrap_unit next();

		public:
			virtual ~environment();
			environment(ui::stack_panel &ui, factory &builder, space_type &space);
			environment(environment const&) = delete;
			environment& operator=(environment const&) = delete;

		private:
			void focus();
			void tie_map();
			void untie_map();

		private:
			space_type* m_space;
			factory* m_factory;
			world_settings m_settings;
			world m_world;
			terrain_director m_terrain_director;
			terrain m_terrain;
			faction_relation m_factions;

			// flow
			bool m_running;
			time_type m_time;
			player_type m_player;

			// user interface
			ui::stack_panel* m_ui;
			point2 m_target;
			fps_counter m_fps;
			inventory_panel* m_inventory;
			anvil_panel* m_craft;
			container_panel* m_container;
			map_panel* m_map;

			// binding helpers
			std::list<location_component*> m_query;
			std::list<location_component*>::iterator m_cursor;
		};
	}
}

#endif