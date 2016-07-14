// name: environment.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// should be facade for interaction with game and place for script bindings

#ifndef PX_CORE_ENVIRONMENT_HPP
#define PX_CORE_ENVIRONMENT_HPP

#include <px/ui/stack_panel.hpp>
#include <px/core/location_component.hpp>
#include <px/rl/traverse.hpp>
#include "terrain.hpp"

#include <memory>

namespace px
{
	namespace core
	{
		class environment
		{
		private:
			unsigned int m_time;
			ui::stack_panel m_ui;

			std::shared_ptr<location_component> m_player;
			terrain* m_terrain;
			qtree<location_component*>* m_space;

		public:
			environment(terrain& terra, qtree<location_component*> &space)
				: m_terrain(&terra)
				, m_space(&space)
				, m_time(0)
			{
			}
			virtual ~environment()
			{
			}

		public:
			void turn()
			{
				++m_time;
			}
			auto time() -> decltype(m_time)
			{
				return m_time;
			}
			const ui::panel& ui() const
			{
				return m_ui;
			}

			std::shared_ptr<location_component> player()
			{
				return m_player;
			}
			void impersonate(std::shared_ptr<location_component> unit)
			{
				if (!unit) throw std::runtime_error("px::core::environment::make_player(unit) unit is null");

				m_player = unit;
			}
			bool maneuver(location_component& location, point2 target)
			{
				bool result = false;
				auto layers = rl::traverse::floor;
				if (m_terrain->traversable(target, layers))
				{
					location_component* vs = nullptr;
					m_space->find(target.x(), target.y(), 0, [&](int x, int y, location_component* lc)
					{
						bool cont = true;
						if (!lc->traversable(layers))
						{
							vs = lc;
							cont = false;
						}
						return cont;
					});
					if (vs)
					{
					}
					else
					{
						location.move(target);
						turn();
						result = true;
					}
				}
				return result;
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