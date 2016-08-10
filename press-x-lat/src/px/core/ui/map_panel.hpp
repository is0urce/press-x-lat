// name: map_panel.hpp
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_UI_MAP_PANEL_HPP
#define PX_CORE_UI_MAP_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board_panel.hpp>
#include <px/ui/static_text_panel.hpp>

#include <px/core/world.hpp>

#include <px/shell/key.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class map_panel : public ui::stack_panel
		{
		public:
			void bind(world &w)
			{
				m_world = &w;
			}
			void tear()
			{
				m_world = nullptr;
			}

		public:
			map_panel()
			{
				emplace<ui::board_panel>({ { 0, 0 },{ 0, 0 },{ 0, 1 },{ 1, 0 } }, color(0.3, 0.3, 0.3));
				emplace<ui::board_panel>({ { 0, 0 },{ 0, 1 },{ 0, -1 },{ 1, 1 } }, color(0.8, 0.8, 0.8));
				emplace<ui::static_text_panel>({ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } }, "[MAP]", color(1, 1, 1));
			}
			virtual ~map_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				if (m_world)
				{

				}
			}
			virtual bool key_control(shell::key code) override
			{
				bool result = stack_panel::key_control(code);

				if (!result)
				{
					bool result = code == shell::key::command_cancel || code == shell::key::move_none;
					if (result)
					{
						deactivate();
					}
				}
				return result;
			}
			virtual bool click_control(const point2 &position, unsigned int button) override
			{
				bool result = stack_panel::click_control(position, button);
				if (!result && !bounds().contains(position))
				{
					deactivate();
					result = true;
				}
				return result;
			}

		private:
			world* m_world;
		};
	}
}

#endif