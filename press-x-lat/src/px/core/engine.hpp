// name: engine.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_ENGINE_HPP
#define PX_CORE_ENGINE_HPP

#include <px/common/toggle.hpp>
#include <px/es/i_engine.hpp>
#include <px/shell/control.hpp>
#include <px/shell/control_dispatcher.hpp>
#include <px/shell/opengl.h>

#include "unit.hpp"
#include "rendering_system.hpp"
#include "location_system.hpp"
#include "input_adapter.hpp"
#include "game.hpp"

#include <px/data/factory.hpp>

namespace px
{
	namespace
	{
		// should be power of 2, size not matters while it grows
		const unsigned int space_start_width = 64;
	}
	namespace core
	{
		class engine
			: public es::i_engine
			, public shell::control_dispatcher
			, public toggle<true>
		{
		private:
			rendering_system m_rs;
			location_system m_ls;
			input_adapter m_adapter;
			game m_game;
			qtree<location_component*> m_space;

			data::factory m_factory;

			std::shared_ptr<unit> m_unit;

		public:
			engine(shell::opengl* gl)
				: control_dispatcher(m_adapter), m_adapter(m_game)
				, m_space(space_start_width)
				, m_ls(m_space)
				, m_rs(gl)
				, m_factory(m_rs, m_ls)
			{
				add(&m_rs);
				add(&m_ls);

				auto task = m_factory.produce();
				task->add_appearance('@');
				auto l = task->add_location({ 5, 5 });
				m_unit = task->assemble();

				m_unit->activate();

				m_game.impersonate(l);
			}
			virtual ~engine() {}
		};
	}
}

#endif