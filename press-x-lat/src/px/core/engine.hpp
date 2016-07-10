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

namespace px
{
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

			unit m_unit;

		public:
			engine(shell::opengl* gl)
				: control_dispatcher(m_adapter), m_adapter(m_game)
				, m_rs(gl)
			{
				add(&m_rs);
				add(&m_ls);

				auto sprite = m_rs.make_sprite('@');
				auto l = m_ls.make_location({ 3, 3 });
				sprite->link(l);

				m_unit.add(l);
				m_unit.add(sprite);
				m_unit.activate();
			}
			virtual ~engine() {}
		};
	}
}

#endif