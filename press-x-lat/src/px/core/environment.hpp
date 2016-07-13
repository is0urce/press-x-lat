// name: environment.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// should be facade for interaction with game and place for script bindings

#ifndef PX_CORE_ENVIRONMENT_HPP
#define PX_CORE_ENVIRONMENT_HPP

#include <px/ui/stack_panel.hpp>
#include <px/core/location_component.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class environment
		{
		public:
			typedef std::shared_ptr<unit> unit_ptr;
		private:
			ui::stack_panel m_ui;
			std::shared_ptr<location_component> m_player;
		public:
			void turn();
			const ui::panel& ui() const
			{
				return m_ui;
			}

			std::shared_ptr<location_component> player()
			{
				return m_player;
			}
			void make_player(std::shared_ptr<location_component> unit)
			{
				if (!unit) throw std::runtime_error("px::core::environment::make_player(unit) unit is null");

				m_player = unit;
			}
			bool maneuver(location_component& location, point2 target)
			{
				location.move(target);
				return true;
			}
		};
	}
}

#endif