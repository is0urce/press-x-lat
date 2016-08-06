// name: stack_panel.cpp
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#include "stack_panel.hpp"

#include <sstream>
#include <stdexcept>

namespace px
{
	namespace ui
	{
		void stack_panel::draw_panel(shell::canvas& cnv) const
		{
			for (auto &p : m_unnamed)
			{
				if (p.panel && p.panel->active())
				{
					p.panel->draw(cnv);
				}
			}
			for (auto &p : m_stack)
			{
				if (p.second.panel && p.second.panel->active())
				{
					p.second.panel->draw(cnv);
				}
			}
		}

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
			if (!panel) throw std::runtime_error("px::ui::stack_panel::add(tag, panel, align) - panel is null, tag = " + name_tag);

			remove(name_tag);
			panel->layout(m_bounds);
			m_stack.emplace(name_tag, stacked_panel(panel, align));
		}
		void stack_panel::add(panel_ptr panel, alignment align)
		{
			if (!panel) throw std::runtime_error("px::ui::stack_panel::add(panel, align) - panel is null");

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
		stack_panel::panel_ptr stack_panel::operator[](const tag &name)
		{
			return at(name);
		}

		void stack_panel::disable(const tag &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				panel &p = *find->second.panel;
				p.deactivate();
			}
#ifdef _DEBUG
			else throw std::runtime_error("stack_panel::disable tag=" + name_tag);
#endif // _DEBUG
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
		point2 stack_panel::start() const
		{
			return m_bounds.start();
		}

		void stack_panel::output(shell::canvas&c)
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