// name: stack_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_STACK_PANEL_HPP
#define PX_UI_STACK_PANEL_HPP

#include <px/ui/alignment.hpp>
#include "panel.hpp"

#include <memory>
#include <map>
#include <list>

namespace px
{
	namespace ui
	{
		class stack_panel : public panel
		{
		public:
			typedef std::string tag;
			typedef std::shared_ptr<stack_panel> panel_ptr;
		private:
			struct stacked_panel
			{
				panel_ptr panel;
				alignment align;
				stacked_panel(panel_ptr panel, alignment align)
					: panel(panel), align(align)
				{
				}
			};
		private:
			std::map<tag, stacked_panel> m_stack;
			std::list<stacked_panel> m_unnamed;
			rectangle m_bounds;

		public:
			stack_panel() {}
			virtual ~stack_panel() {}

		private:
			template<typename Op>
			bool panel_action(Op&& act)
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

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
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
			virtual bool key_control(shell::key code) override;
			virtual bool hover_control(const point2 &position) override;
			virtual bool click_control(const point2 &position, unsigned int button) override;
			virtual bool scroll_control(int delta) override;

		protected:
			rectangle bounds() const;
			void layout();

		public:
			void layout(rectangle bounds);
			void output(shell::canvas& cnv); // layout to canvas size and draw

			void add(tag name, panel_ptr panel, alignment align);
			void add(panel_ptr panel, alignment align);
			void remove(const tag &name);
			void clear();

			panel_ptr at(const tag &name);
			void disable(const tag &name);
			void enable(const tag &name);
			void toggle(const tag &name);
			bool enabled(const tag &name) const;
			std::string info() const;
		};
	}
}

#endif