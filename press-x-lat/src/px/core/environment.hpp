// name: environment.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// should be facade for interaction with game and place for script bindings

#ifndef PX_CORE_ENVIRONMENT_HPP
#define PX_CORE_ENVIRONMENT_HPP

#include <px/common/fps_counter.hpp>

#include <px/rl/faction_relation.hpp>
#include <px/rl/traverse.hpp>
#include <px/core/terrain.hpp>

#include <px/common/fps_counter.hpp>
#include <px/ui/stack_panel.hpp>

#include <px/core/ui/status_panel.hpp>
#include <px/core/ui/target_panel.hpp>
#include <px/core/ui/inventory_panel.hpp>
#include <px/core/ui/anvil_panel.hpp>

#include <px/ui/performance_panel.hpp>

namespace px
{
	namespace core
	{
		class environment
		{
		public:
			static const unsigned int max_use_distance = 1;
		private:
			unsigned int m_time;

			std::shared_ptr<location_component> m_player;
			terrain* m_terrain;
			qtree<location_component*>* m_space;
			faction_relation m_factions;

			// user interface

			fps_counter m_fps;
			point2 m_target;
			ui::stack_panel* m_ui;
			std::shared_ptr<inventory_panel> m_inventory;
			std::shared_ptr<anvil_panel> m_craft;

		public:
			environment(ui::stack_panel &ui, terrain &terra, qtree<location_component*> &space)
				: m_ui(&ui)
				, m_terrain(&terra)
				, m_space(&space)
				, m_time(0)
			{
				setup_ui();
			}
			virtual ~environment()
			{
			}

		private:
			void setup_ui()
			{
				m_inventory = std::make_shared<inventory_panel>();
				m_craft = std::make_shared<anvil_panel>();

				m_ui->emplace<ui::performance_panel>("performance", { { 0.0, 0.0 },{ 1,0 },{ -2, 1 },{ 1.0, 0.0 } }, m_fps);
				m_ui->emplace<status_panel>("status", { { 0.0, 1.0 },{ 1, -12 },{ -2, 1 },{ 1.0, 0.0 } }, *this);
				m_ui->emplace<target_panel>("target", { { 1.0, 1.0 },{ -12, -12 },{ -2, 1 },{ 1.0, 0.0 } }, *this);

				m_ui->add("inventory", m_inventory, { { 0.30, 0.1 },{ 0, 0 },{ 0, 0 },{ 0.4, 0.8 } });
				m_ui->add("craft", m_craft, { { 0.25, 0.25 },{ 0, 0 },{ 0, 0 },{ 0.25, 0.25 } });

				m_inventory->deactivate();
				m_craft->deactivate();
			}
			void focus()
			{
				if (m_player)
				{
					m_terrain->focus(m_player->current());
				}
			}

		public:

			// actions

			bool maneuver(location_component& location, point2 target)
			{
				bool action = false;
				auto layers = rl::traverse::floor;
				if (m_terrain->traversable(target, layers))
				{
					location_component* versus = blocking(target, rl::traverse::floor);

					if (versus)
					{
						action = cast(location, 0, *versus);
					}
					else
					{
						location.move(target);
						action = true;
					}
				}

				return action;
			}
			bool cast(location_component& source, unsigned int slot, location_component& target_location)
			{
				bool action = false;
				if (body_component* user_body = source)
				{
					if (character_component* character = *user_body)
					{
						if (auto spell = character->skill(slot))
						{
							if (spell->targeted())
							{
								if (target_location)
								{
									action = spell->try_use(&source, &target_location);
								}
							}
						}
					}
				}
				return action;
			}
			bool cast(location_component& source, unsigned int slot, point2 target)
			{
				bool action = false;
				if (body_component* user_body = source)
				{
					if (character_component* character = *user_body)
					{
						if (auto spell = character->skill(slot))
						{
							if (spell->targeted())
							{
								// select target from location
								location_component* target_pawn = nullptr;
								m_space->find(target.x(), target.y(), [&](int x, int y, location_component* component)
								{
									if (body_component* b = *component) // should have body
									{
										target_pawn = component;
									}
								});

								if (target_pawn)
								{
									action = spell->try_use(&source, target_pawn);
								}
							}
							else
							{
								action = spell->try_use(&source, target);
							}
						}
					}
				}
				return action;
			}
			bool activate(location_component& source, point2 position)
			{
				bool done = false;
				if (distance(source.current(), position) <= max_use_distance)
				{
					m_space->find(position.x(), position.y(), [&](int x, int y, location_component* component)
					{
						if (body_component* body = *component)
						{
							if (i_useable_component* useable = *body)
							{
								done = useable->try_use(&source, *this);
							}
						}
					});
				}
				return done;
			}

			// props querry

			auto distance(point2 a, point2 b) const -> decltype(a.king_distance(b))
			{
				return a.king_distance(b);
			}
			auto reputation(body_component& a, body_component& b)
			{
				return m_factions.reputation(a.faction(), b.faction());
			}

			// space and actors querry

			bool traversable(point2 position, rl::traverse layer) const
			{
				return m_terrain->traversable(position, layer) && !blocking(position, layer);
			}
			location_component* blocking(point2 position, rl::traverse layer) const
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
			template<typename CallbackOperator>
			void nearest(point2 postion, unsigned int radius, CallbackOperator&& enum_fn)
			{
				m_space->find(postion.x(), postion.y(), radius, enum_fn);
			}
			auto nearest(point2 postion, unsigned int radius)
			{
				std::list<location_component*> list;
				m_space->find(postion.x(), postion.y(), radius, [&](int x, int y, location_component* component)
				{
					list.push_back(component);
				});
				return list;
			}

			// game flow

			void start()
			{
				m_time = 0;
			}
			void end()
			{
			}
			auto time()
			{
				return m_time;
			}
			void turn()
			{
				focus();
				++m_time;
			}

			// player

			auto player()
			{
				return m_player;
			}
			const auto player() const
			{
				return m_player;
			}
			void impersonate(std::shared_ptr<location_component> unit)
			{
				m_player = unit;
				focus();
				if (unit)
				{
					m_inventory->show(unit->linked());
				}
			}

			// interface relation

			void target(point2 location)
			{
				m_target = location;
			}
			point2 targeted() const
			{
				return m_target;
			}

			void open_workshop(std::weak_ptr<body_component> body)
			{
				m_craft->show(body);

				m_inventory->deactivate();
				m_craft->activate();
			}
		};
	}
}

#endif