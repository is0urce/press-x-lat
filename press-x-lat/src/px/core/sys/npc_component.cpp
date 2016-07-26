// name: npc_component.cpp
// type: c++
// desc: class
// auth: is0urce

#include "npc_component.hpp"

#include <px/rl/astar.hpp>

#include <px/core/environment.hpp>
#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

namespace px
{
	namespace core
	{
		npc_component::npc_component() : m_alert(false) {}
		npc_component::~npc_component() {}

		void npc_component::resolve_action(environment &e)
		{
			if (auto* location = static_cast<location_component*>(*this))
			{
				if (auto* body = static_cast<body_component*>(*location))
				{
					auto hp = body->health();
					if (hp && !hp->empty())
					{
						if (auto target = e.player())
						{
							auto path = astar::find(location->current(), target->current(), [&e](const point2 &p) { return e.traversable(p, rl::traverse::floor); }, 500);
							if (!path.empty())
							{
								e.maneuver(*location, path.front());
							}
						}
					}
				}
			}
		}
	}
}