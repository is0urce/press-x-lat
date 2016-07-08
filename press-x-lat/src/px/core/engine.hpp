// name: engine.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_ENGINE_HPP
#define PX_CORE_ENGINE_HPP

#include <px/es/i_engine.hpp>
#include <px/shell/control.hpp>
#include <px/shell/control_dispatcher.hpp>
#include "input_adapter.hpp"
#include "game.hpp"

namespace px
{
	namespace core
	{
		class engine
			: public es::i_engine
			, public shell::control_dispatcher
		{
		private:
			input_adapter m_adapter;
			game m_game;

		public:
			engine() : control_dispatcher(&m_adapter), m_adapter(&m_game) {}
			virtual ~engine() {}
		};
	}
}

#endif