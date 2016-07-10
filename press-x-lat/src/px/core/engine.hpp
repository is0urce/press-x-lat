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

#include <px/es/component_collection.hpp>
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

			es::component_collection m_obj;

		public:
			engine(shell::opengl* gl)
				: control_dispatcher(&m_adapter), m_adapter(&m_game)
				, m_rs(gl)
			{
				add(&m_rs);
				add(&m_ls);

				auto sprite = m_rs.make_sprite('@');
				auto l = m_ls.make_location({ 3, 3 });
				sprite->link(l);

				m_obj.add(l);
				m_obj.add(sprite);
				m_obj.activate();
			}
			virtual ~engine() {}
		};
	}
}

#endif