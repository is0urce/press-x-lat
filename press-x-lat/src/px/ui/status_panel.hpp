// name: status_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_STATUS_PANEL_HPP
#define PX_UI_STATUS_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/core/environment.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		class status_panel : public stack_panel
		{
		private:
			core::environment* m_environment;

		public:
			status_panel(core::environment& e) : m_environment(&e) {}
			virtual ~status_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				auto pen = bounds().start();

				auto player = m_environment->player();
				if (player)
				{
					cnv.write(pen, to_string(player->current()));
					pen.move_axis<1>(1);
					cnv.write(pen, "[PLAYER]");
					pen.move_axis<1>(1);

					if (auto body = static_cast<core::body_component*>(*player))
					{
						if (auto hp = body->health())
						{
							cnv.write(pen, std::string("HP ") + std::to_string(hp->current()) + "/" + std::to_string(hp->maximum()));
							pen.move_axis<1>(1);
						}
						if (auto mp = body->energy())
						{
							cnv.write(pen, std::string("MP ") + std::to_string(mp->current()) + "/" + std::to_string(mp->maximum()));
							pen.move_axis<1>(1);
						}
					}
				}
			}
		};
	}
}

#endif