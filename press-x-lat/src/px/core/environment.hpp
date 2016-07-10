// name: environment.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// should be facade for interaction with game and place for script bindings

#ifndef PX_CORE_ENVIRONMENT_HPP
#define PX_CORE_ENVIRONMENT_HPP

#include <px/ui/stack_panel.hpp>

namespace px
{
	namespace core
	{
		class environment
		{
		private:
			ui::stack_panel m_ui;
		public:
			void turn();
			const ui::panel& ui() const
			{
				return m_ui;
			}
		};
	}
}

#endif