// name: environment-act.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "environment.hpp"

#include <px/core/terrain.hpp>
#include <px/core/ui/map_panel.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>
#include <px/core/sys/character_component.hpp>
#include <px/core/sys/i_useable_component.hpp>

namespace px
{
	namespace core
	{
		namespace
		{
			static const unsigned int max_use_distance = 1;
		}
		bool environment::maneuver(location_component& location, point2 target)
		{
			bool action = false;
			auto layers = rl::traverse::floor;
			if (m_terrain.traversable(target, layers))
			{
				location_component* versus = blocking(target, rl::traverse::floor);

				if (versus)
				{
					action = cast(location, 0, *versus);
				}
				else
				{
					location.move(target);
					action = true;
				}
			}

			return action;
		}

		bool environment::cast(location_component& source, unsigned int slot, location_component& target_location)
		{
			bool action = false;
			if (body_component* user_body = source)
			{
				if (character_component* character = *user_body)
				{
					if (auto spell = character->skill(slot))
					{
						if (spell->targeted())
						{
							if (target_location)
							{
								action = spell->try_use(&source, &target_location);
							}
						}
					}
				}
			}
			return action;
		}

		bool environment::cast(location_component& source, unsigned int slot, point2 target)
		{
			bool action = false;
			if (body_component* user_body = source)
			{
				if (character_component* character = *user_body)
				{
					if (auto spell = character->skill(slot))
					{
						if (spell->targeted())
						{
							// select target from location
							location_component* target_pawn = nullptr;
							m_space->find(target.x(), target.y(), [&](int x, int y, location_component* component)
							{
								if (body_component* b = *component) // should have body
								{
									target_pawn = component;
								}
							});

							if (target_pawn)
							{
								action = spell->try_use(&source, target_pawn);
							}
						}
						else
						{
							action = spell->try_use(&source, target);
						}
					}
				}
			}
			return action;
		}

		bool environment::activate(location_component& source, point2 position)
		{
			bool done = false;
			if (distance(source.current(), position) <= max_use_distance)
			{
				m_space->find(position.x(), position.y(), [&](int x, int y, location_component* component)
				{
					if (body_component* body = *component)
					{
						if (i_useable_component* useable = *body)
						{
							done = useable->try_use(&source, *this);
						}
					}
				});
			}
			return done;
		}
	}
}