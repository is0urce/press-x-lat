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

		public:
			void layout(rectangle bounds);
			void output(shell::canvas &cnv); // layout to canvas size and draw

			void add(tag name, panel_ptr panel, alignment align);
			void add(panel_ptr panel, alignment align);
			void remove(tag const& name);
			void clear();

			using toggle<true>::active; // overloading
			using toggle<true>::reverse_toggle;
			using toggle<true>::activate;
			using toggle<true>::deactivate;
			void deactivate(tag const& name);
			void activate(tag const& name);
			void activate(tag const& name, bool flag);
			void reverse_toggle(tag const& name);
			bool active(tag const& name) const;

			panel_ptr at(tag const& name) const;
			panel_ptr operator[](tag const& name) const noexcept; // alias for this->at(..), returns nullptr if nothing found
			std::string info() const;

		public:
			stack_panel();
			virtual ~stack_panel();
			stack_panel(stack_panel const&) = delete;

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override;
			virtual bool key_control(shell::key code) override;
			virtual bool hover_control(const point2 &position) override;
			virtual bool click_control(const point2 &position, unsigned int button) override;
			virtual bool scroll_control(int delta) override;

		protected:
			rectangle bounds() const;
			point2 start() const;
			void layout();
			template<typename Op> bool panel_action(Op&& act);

		private:
			struct stacked_panel
			{
				panel_ptr panel;
				alignment align;
				stacked_panel(panel_ptr panel, alignment align) : panel(panel), align(align) {}
			};
			std::map<tag, stacked_panel> m_stack;
			std::list<stacked_panel> m_unnamed;
			rectangle m_bounds;

		public:
			template <typename SubPanel, typename ...Args>
			std::shared_ptr<SubPanel> emplace(alignment align, Args &&...args)
			{
				auto result = std::make_shared<SubPanel>(std::forward<Args>(args)...);
				add(result, align);
				return result;
			}
			template <typename SubPanel, typename ...Args>
			std::shared_ptr<SubPanel> emplace(tag name, alignment align, Args &&...args)
			{
				auto result = std::make_shared<SubPanel>(std::forward<Args>(args)...);
				add(name, result, align);
				return result;
			}
		};
	}
}

#endif