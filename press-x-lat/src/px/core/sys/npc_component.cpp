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
				if (body_component* body = *location)
				{
					auto hp = body->health();
					if (hp && !hp->empty())
					{
						character_component* character = *body;
						auto nearest = env.nearest(location->current(), sight_radius);

						// select best skill for use
						bool done = false;
						if (character)
						{
							for (size_t slot = 0, len = character->skils_total(); slot < len && !done; ++slot)
							{
								auto skill = character->skill(slot);
								if (skill->targeted())
								{
									for (auto neighbour_location : nearest)
									{
										if (skill->useable(location, neighbour_location))
										{
											skill->use(location, neighbour_location);
											done = true;
											break;
										}
									}
								}
								else
								{
									if (skill->useable(location, location->current()))
									{
										skill->use(location, location->current());
									}
									done = true;
								}
							}
						}

						// approach enemy if no skills ready
						if (!done)
						{
							// select nearest enemy in sight radius
							location_component* target_location = nullptr;
							int distance = sight_radius + 1; // start with something out of range
							for (auto neighbour_location : nearest)
							{
								if (body_component* target_body = *neighbour_location)
								{
									character_component* target_character = *target_body;
									if (target_character && env.reputation(*body, *target_body) < 0) // an enemy character
									{
										auto range = env.distance(location->current(), neighbour_location->current());
										if (distance > range)
										{
											distance = range;
											target_location = neighbour_location;
										}
									}
								}
							}

							// target found
							if (target_location)
							{
								auto path = astar::find(location->current(), target_location->current(), [&env](const point2 &check_pos) { return env.traversable(check_pos, rl::traverse::floor); }, max_steps);
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
}