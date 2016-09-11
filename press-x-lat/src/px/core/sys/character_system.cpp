// name: character_system.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "character_system.hpp"

#include <px/core/environment.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

namespace px
{
	namespace core
	{
		character_system::character_system(environment &env)
		{
			fill(env);
		}
		character_system::~character_system() {}

		void character_system::element_allocated(character_component &element)
		{
			element.deactivate();
			element.provide(&m_spellbook);
		}

		character_system::skillbook_type* character_system::skill_book()
		{
			return &m_spellbook;
		}

		void character_system::fill(environment &env)
		{
			m_spellbook.add_target("melee", [](location_component* user, location_component* target) {
				if (target && user)
				{
					body_component* target_body = *target;
					body_component* user_body = *user;
					if (target_body && user_body)
					{
						if (auto hp = target_body->health())
						{
							auto dmg = user_body->accumulate<rl::effect::weapon_damage>();
							hp->damage(dmg.value0);
						}
					}
				}
			}, [&](location_component* user, location_component* target) {
				if (!user || !target) return false;

				body_component* body = *user;
				body_component* target_body = *target;

				return body && target_body && body != target_body
					&& target_body->health() && !target_body->health()->empty() // should have positive hit points
					&& env.reputation(*body, *target_body) < 0 // friend-or-foe
					&& env.distance(user->current(), target->current()) == 1; // 1 tile melee distance
			});
		}
	}
}
