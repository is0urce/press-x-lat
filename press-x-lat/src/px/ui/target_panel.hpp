// name: target_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_TARGET_PANEL_HPP
#define PX_UI_TARGET_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/core/environment.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		class target_panel : public stack_panel
		{
		private:
			core::environment* m_environment;

		public:
			target_panel(core::environment& e) : m_environment(&e) {}
			virtual ~target_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				auto pen = bounds().start();
				auto target = m_environment->targeted();

				cnv.write(pen, to_string(target));
				pen.move_axis<1>(1);

				if (!m_environment->traversable(target, rl::traverse::floor))
				{
					cnv.write(pen, "[BLOCKING]");
					pen.move_axis<1>(1);
				}

				auto blocking = m_environment->blocking(target, rl::traverse::floor);
				if (blocking)
				{
					if (auto body = static_cast<core::body_component*>(*blocking))
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