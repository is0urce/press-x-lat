// name: environment.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// should be facade for interaction with game and place for script bindings

#ifndef PX_CORE_ENVIRONMENT_HPP
#define PX_CORE_ENVIRONMENT_HPP

#include <px/ui/stack_panel.hpp>
#include <px/core/location_component.hpp>
#include "scene.hpp"

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
			std::unique_ptr<scene> m_scene;

		public:
			environment() : m_time(0)
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
				location.move(target);
				turn();
				return true;
			}

			void start()
			{
				m_time = 0;
				m_scene = std::make_unique<scene>();
			}
			void end()
			{
				m_time = 0;
				m_scene.reset();
			}
		};
	}
}

#endif