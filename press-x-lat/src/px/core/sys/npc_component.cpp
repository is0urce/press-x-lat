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

			bool is_enemy(body_component &source_body, location_component &target_location, environment &env)
			{
				if (body_component* target_body = target_location)
				{
					auto hp = target_body->health();
					character_component* target_character = *target_body;
					return target_character && !hp->empty() && env.reputation(source_body, *target_body) < 0; // an living enemy character
				}
				return false;
			}
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
										if (skill->try_use(location, neighbour_location))
										{
											done = true;
											break;
										}
									}
								}
								else
								{
									done = skill->try_use(location, location->current());;
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
								if (neighbour_location && is_enemy(*body, *neighbour_location, env))
								{
									auto range = env.distance(location->current(), neighbour_location->current());
									if (distance > range)
									{
										distance = range;
										target_location = neighbour_location;
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