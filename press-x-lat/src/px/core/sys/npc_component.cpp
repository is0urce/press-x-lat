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
		namespace
		{
			unsigned int sight_radius = 10;
			unsigned int max_steps = 200; // max steps in pathfinding
		}
		npc_component::npc_component() : m_alert(false) {}
		npc_component::~npc_component() {}

		void npc_component::resolve_action(environment &env)
		{
			if (location_component* location = *this)
			{
				if (body_component* self_body = *location)
				{
					auto hp = self_body->health();
					if (hp && !hp->empty())
					{
						// select nearest enemy in sight radius
						location_component* target = nullptr;
						int distance = sight_radius + 1; // start with something out of range
						env.nearest(location->current(), sight_radius, [&](int x, int y, location_component* enum_location)
						{
							if (enum_location && enum_location != location) // don't count self
							{
								body_component* target_body = *enum_location;
								if (target_body && env.reputation(*self_body, *target_body) < 0) // an enemy
								{
									auto range = env.distance(location->current(), { x, y });
									if (distance > range)
									{
										distance = range;
										target = enum_location;
									}
								}
							}
						});

						if (target)
						{
							auto path = astar::find(location->current(), target->current(), [&env](const point2 &check_pos) { return env.traversable(check_pos, rl::traverse::floor); }, max_steps);
							if (!path.empty())
							{
								env.maneuver(*location, path.front());
							}
						}
					}
				}
			}
		}
	}
}