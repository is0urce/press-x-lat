// name: status_panel.cpp
// type: c++
// desc: class functions
// auth: is0urce

#include "status_panel.hpp"

#include <px/core/environment.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

#include <string>

namespace px
{
	namespace core
	{
		status_panel::status_panel(core::environment& e) : m_environment(&e)
		{
		}
		status_panel::~status_panel()
		{
		}

		void status_panel::draw_panel(shell::canvas& cnv) const
		{
			stack_panel::draw_panel(cnv);

			auto pen = start();

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
	}
}