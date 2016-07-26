// name: stack_panel.cpp
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#include "stack_panel.hpp"

#include <sstream>

namespace px
{
	namespace ui
	{
		using namespace shell;

		bool stack_panel::key_control(shell::key code)
		{
			return panel_action([&](stacked_panel& p) { return p.panel->key(code); });
		}

		bool stack_panel::hover_control(const point2 &position)
		{
			return panel_action([&](stacked_panel& p) { return p.panel->hover(position); });
		}
		bool stack_panel::click_control(const point2 &position, unsigned int button)
		{
			return panel_action([&](stacked_panel& p) { return p.panel->click(position, button); });
		}
		bool stack_panel::scroll_control(int delta)
		{
			return panel_action([&](stacked_panel& p) { return p.panel->scroll(delta); });
		}

		void stack_panel::add(tag name_tag, panel_ptr panel, alignment align)
		{
			remove(name_tag);
			panel->layout(m_bounds);
			m_stack.emplace(name_tag, stacked_panel(panel, align));
		}
		void stack_panel::add(panel_ptr panel, alignment align)
		{
			panel->layout(m_bounds);
			m_unnamed.emplace_back(panel, align);
		}
		void stack_panel::remove(const tag &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				m_stack.erase(find);
			}
		}
		void stack_panel::clear()
		{
			m_stack.clear();
			m_unnamed.clear();
		}

		stack_panel::panel_ptr stack_panel::at(const tag &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find == m_stack.end()) throw std::logic_error("px::ui::stack_panel::at(panel_id) panel not found");
			return find->second.panel;
		}

		void stack_panel::disable(const tag &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				panel &p = *find->second.panel;
				p.deactivate();
			}
		}

		void stack_panel::enable(const tag &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				panel &p = *find->second.panel;
				p.activate();
			}
		}
		void stack_panel::toggle(const tag &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				panel &p = *find->second.panel;
				p.reverse_toggle();
			}
		}
		bool stack_panel::enabled(const tag &name_tag) const
		{
			auto find = m_stack.find(name_tag);
			return find == m_stack.end() ? false : find->second.panel->active();
		}
		void stack_panel::layout(rectangle bounds)
		{
			m_bounds = bounds;
			layout();
		}
		void stack_panel::layout()
		{
			panel_action([&](stacked_panel& p) { p.panel->m_bounds = p.align.calculate(m_bounds); p.panel->layout(); return false; });
		}
		rectangle stack_panel::bounds() const
		{
			return m_bounds;
		}

		void stack_panel::output(canvas&c)
		{
			layout({ { 0, 0 }, c.range() });
			draw(c);
		}

		std::string stack_panel::info() const
		{
			std::stringstream ss("");
			ss << "childs=" << m_stack.size() << " ";
			ss << "bounds=[start=[" << m_bounds.start().x() << "," << m_bounds.start().y() << "], range=[" << m_bounds.range().x() << ", " << m_bounds.range().y() << "]]";
			return ss.str();
		}
	}
}