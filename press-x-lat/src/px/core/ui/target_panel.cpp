// name: target_panel.cpp
// type: c++
// desc: class functions implementation
// auth: is0urce

#include "target_panel.hpp"

#include <px/common/string_ext.hpp>
#include <px/core/environment.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

#include <string>

namespace px
{
	namespace core
	{
		target_panel::target_panel(core::environment& e) : m_environment(&e)
		{
		}
		target_panel::~target_panel()
		{
		}

		void target_panel::draw_stacked(shell::canvas& cnv) const
		{
			auto pen = start();
			auto target = m_environment->targeted();

			cnv.write(pen, to_string(target));
			pen.move_axis<1>(1);

			if (!m_environment->traversable(target, rl::traverse::floor))
			{
				cnv.write(pen, "[BLOCKING]");
				pen.move_axis<1>(1);
			}

			for (auto* pawn : m_environment->nearest(target, 0))
			{
				if (pawn)
				{
					if (body_component* body = *pawn)
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
}