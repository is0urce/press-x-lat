// name: status_panel.cpp
// type: c++
// desc: class
// auth: is0urce

#include "status_panel.hpp"

#include <px/core/environment.hpp>
#include <px/core/sys/body_component.hpp>

#include <string>

namespace px
{
	namespace ui
	{
		void status_panel::draw_panel(shell::canvas& cnv) const
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
	}
}

