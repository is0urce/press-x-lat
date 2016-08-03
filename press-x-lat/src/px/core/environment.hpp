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

namespace px
{
	namespace core
	{
		class environment
		{
		private:
			unsigned int m_time;
			point2 m_target;
			ui::stack_panel* m_ui;

			std::shared_ptr<location_component> m_player;
			terrain* m_terrain;
			qtree<location_component*>* m_space;
			faction_relation m_factions;

		public:
			environment(ui::stack_panel &ui, terrain &terra, qtree<location_component*> &space)
				: m_ui(&ui)
				, m_terrain(&terra)
				, m_space(&space)
				, m_time(0)
			{
			}
			virtual ~environment()
			{
			}

		private:
			void focus()
			{
				if (m_player)
				{
					m_terrain->focus(m_player->current());
				}
			}

		public:
			bool traversable(point2 position, rl::traverse layer) const
			{
				return m_terrain->traversable(position, layer) && !blocking(position, layer);
			}
			location_component* blocking(point2 position, rl::traverse layer) const
			{
				location_component* blocking = nullptr;
				m_space->find(position.x(), position.y(), [&](int x, int y, location_component* component)
				{
					bool search = true;
					body_component* body = static_cast<body_component*>(*component);
					if (body && !body->traversable(layer))
					{
						blocking = component;
						search = false;
					}
					return search;
				});
				return blocking;
			}
			template<typename CallbackOperator>
			void nearest(point2 postion, unsigned int radius, CallbackOperator& fn)
			{
				m_space->find(postion.x(), postion.y(), radius, [&](int x, int y, location_component* component)
				{
					fn(x, y, component);
					return true;
				});
			}
			auto nearest(point2 postion, unsigned int radius)
			{
				std::list<location_component*> list;
				m_space->find(postion.x(), postion.y(), radius, [&](int x, int y, location_component* component)
				{
					list.push_back(component);
					return true;
				});
				return list;
			}

			bool maneuver(location_component& location, point2 target)
			{
				bool action = false;
				auto layers = rl::traverse::floor;
				if (m_terrain->traversable(target, layers))
				{
					location_component* versus = blocking(target, rl::traverse::floor);

					if (versus)
					{
						action = true;
					}
					else
					{
						location.move(target);
						action = true;
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
						auto spell = character->skill(slot);
						if (spell && spell->targeted())
						{
							// select target from location
							location_component* target_location = nullptr;
							m_space->find(target.x(), target.y(), [&](int x, int y, auto* component)
							{
								bool search = true;
								if (body_component* b = *component) // should have body
								{
									target_location = component;
									search = false;
								}
								return search;
							});
							
							if (spell->useable(&source, target_location))
							{
								spell->use(&source, target_location);
								action = true;
							}
						}
					}
				}
				return action;
			}

			// props querry

			auto distance(point2 a, point2 b) const
			{
				return a.king_distance(b);
			}
			auto reputation(body_component& a, body_component& b)
			{
				return m_factions.reputation(a.faction(), b.faction());
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
		};
	}
}

#endif