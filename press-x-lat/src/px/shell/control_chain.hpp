// name: control_chain.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// send input one control, if it fails, to another

#ifndef PX_CONTROL_CHAIN_HPP
#define PX_CONTROL_CHAIN_HPP

#include "control.hpp"

#include <functional>

namespace px
{
	namespace shell
	{
		class control_chain : public control
		{
		public:
			typedef std::function<point2(point2)> map_fn;
		private:
			control* m_first;
			control* m_second;
			map_fn m_map1;
			map_fn m_map2;
		public:
			control_chain(control* first, control* second, map_fn map1, map_fn map2)
				: m_first(first), m_second(second), m_map1(map1), m_map2(map2)
			{
				if (!first) throw std::runtime_error("px::shell::control_chain::ctor() - first is null");
				if (!second) throw std::runtime_error("px::shell::control_chain::ctor() - second is null");
				if (!map1) throw std::runtime_error("px::shell::control_chain::ctor() - map1 is null");
				if (!map2) throw std::runtime_error("px::shell::control_chain::ctor() - map2 is null");
			}
			virtual ~control_chain() {}

		protected:
			virtual bool key_control(key_t code) override
			{
				return m_first->key(code) || m_second->key(code);
			}
			virtual bool hover_control(const point2 &position) override
			{
				return m_first->hover(m_map1(position)) || m_second->hover(m_map2(position));
			}
			virtual bool click_control(const point2 &position, unsigned int button) override
			{
				return m_first->click(m_map1(position)) || m_second->click(m_map2(position));
			}
			virtual bool scroll_control(int delta) override
			{
				return m_first->scroll(delta) || m_second->scroll(delta);
			}
		};
	}
}

#endif