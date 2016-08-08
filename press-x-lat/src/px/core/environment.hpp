// name: environment.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// should be facade for interaction with game and place for script bindings

#ifndef PX_CORE_ENVIRONMENT_HPP
#define PX_CORE_ENVIRONMENT_HPP

#include <px/rl/faction_relation.hpp>
#include <px/rl/traverse.hpp>
#include <px/core/terrain.hpp>

#include <px/ui/stack_panel.hpp>

#include <px/common/fps_counter.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class inventory_panel;
		class container_panel;
		class anvil_panel;

		class environment
		{
		public:
			static const unsigned int max_use_distance = 1;

		public:
			// interface relation

			void setup_ui();
			void target(point2 location);
			point2 targeted() const;
			void open_workshop(std::weak_ptr<body_component> user);
			void open_container(std::weak_ptr<body_component> user, std::weak_ptr<body_component> container);
			void assign_inventory(std::weak_ptr<body_component> body);

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

				m_terrain->enumerate([](auto unit) {
					if (location_component* pawn = unit->location())
					{
						if (body_component* body = *pawn)
						{
							if (auto hp = body->health())
							{
								if (hp->empty() && body->empty() && unit->get_persistency() != persistency::destroying)
								{
									unit->destroy(5);
								}
							}
						}
					}
				});
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
					assign_inventory(unit->linked());
				}
			}

		public:
			environment(ui::stack_panel &ui, terrain &terra, qtree<location_component*> &space)
				: m_ui(&ui)
				, m_terrain(&terra)
				, m_space(&space)
				, m_time(0)
			{
				setup_ui();
			}
			virtual ~environment() {}
			environment(const environment&) = delete;

		private:
			void focus()
			{
				if (m_player)
				{
					m_terrain->focus(m_player->current());
				}
			}

		private:
			unsigned int m_time;
			std::shared_ptr<location_component> m_player;
			terrain* m_terrain;
			qtree<location_component*>* m_space;
			faction_relation m_factions;

			// user interface
			ui::stack_panel* m_ui;
			point2 m_target;
			fps_counter m_fps;
			std::shared_ptr<inventory_panel> m_inventory;
			std::shared_ptr<anvil_panel> m_craft;
			std::shared_ptr<container_panel> m_container;
		};
	}
}

#endif