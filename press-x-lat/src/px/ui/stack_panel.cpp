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
		stack_panel::stack_panel() {}
		stack_panel::~stack_panel() {}

		template<typename Op>
		bool stack_panel::panel_action(Op&& act)
		{
			for (auto &p : m_stack)
			{
				if (p.second.panel && p.second.panel->active() && std::forward<Op>(act)(p.second)) return true;
			}
			for (auto &p : m_unnamed)
			{
				if (p.panel && p.panel->active() && std::forward<Op>(act)(p)) return true;
			}
			return false;
		}

		void stack_panel::draw_panel(shell::canvas& cnv) const
		{
			for (auto &p : m_stack)
			{
				if (p.second.panel && p.second.panel->active())
				{
					p.second.panel->draw(cnv);
				}
			}
			for (auto &p : m_unnamed)
			{
				if (p.panel && p.panel->active())
				{
					p.panel->draw(cnv);
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

		void stack_panel::add(tag name, panel_ptr panel, alignment align)
		{
			if (!panel) throw std::runtime_error("px::ui::stack_panel::add(tag, panel, align) - panel is null, tag = " + name);
			if ((*this)[name]) throw std::runtime_error("px::ui::stack_panel::add(tag, panel, align) - panel name already occupied, tag = " + name);

			panel->layout(m_bounds);
			m_stack.emplace(name, stacked_panel(panel, align));
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

		stack_panel::panel_ptr stack_panel::at(const tag &name) const
		{
			auto find = m_stack.find(name);
			if (find == m_stack.end()) throw std::logic_error("px::ui::stack_panel::at(panel_id) panel not found, name = " + name);
			return find->second.panel;
		}
		stack_panel::panel_ptr stack_panel::operator[](const tag &name) const noexcept
		{
			auto find = m_stack.find(name);
			return find == m_stack.end() ? nullptr : find->second.panel;
		}

		void stack_panel::deactivate(tag const& name)
		{
			if (auto p = (*this)[name])
			{
				p->deactivate();
			}
		}

		void stack_panel::activate(tag const& name)
		{
			if (auto p = (*this)[name])
			{
				p->activate();
			}
		}
		void stack_panel::activate(tag const& name, bool flag)
		{
			if (auto p = (*this)[name])
			{
				p->set_toggle(flag);
			}
		}
		void stack_panel::reverse_toggle(tag const& name)
		{
			if (auto p = (*this)[name])
			{
				p->reverse_toggle();
			}
		}
		bool stack_panel::active(tag const& name) const
		{
			auto p = (*this)[name];
			return p ? p->active() : false;
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