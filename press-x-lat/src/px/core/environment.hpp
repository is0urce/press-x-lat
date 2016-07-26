// name: environment.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// should be facade for interaction with game and place for script bindings

#ifndef PX_CORE_ENVIRONMENT_HPP
#define PX_CORE_ENVIRONMENT_HPP

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
			ui::stack_panel* m_ui;


			std::shared_ptr<location_component> m_player;
			terrain* m_terrain;
			qtree<location_component*>* m_space;

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
			void turn()
			{
				focus();
				++m_time;
			}
			auto time() -> decltype(m_time)
			{
				return m_time;
			}
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
				if (body_component* body = static_cast<body_component*>(source))
				{
					if (character_component* character = static_cast<character_component*>(*body))
					{
						auto spell = character->get_skill(slot);
						if (spell && spell->targeted())
						{
							spell->use(*character, *body);
							action = true;
						}
					}
				}
				return false;
			}

			void start()
			{
				m_time = 0;
			}
			void end()
			{
				m_time = 0;
			}
		};
	}
}

#endif