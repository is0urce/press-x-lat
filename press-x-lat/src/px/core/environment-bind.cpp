// name: environment-bind.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "environment.hpp"

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

#include <px/core/wrap_unit.hpp>

#include <px/rl/effect.hpp>
#include <px/rl/damage_type.hpp>

namespace px
{
	namespace core
	{
		// damage, damage_type, is_hit, is_critical
		std::tuple<int, unsigned int, bool, bool> environment::hit(wrap_unit &attacker, wrap_unit &target)
		{
			int damage = 0;
			unsigned int damage_type = static_cast<unsigned int>(rl::damage_type::not_valid);
			bool hit = false;
			bool critical = false;

			body_component* attacker_body = attacker.body();
			body_component* target_body = target.body();

			if (attacker_body)
			{
				auto dmg = attacker_body->accumulate<rl::effect::weapon_damage>();
				damage = dmg.value0;
				damage_type = static_cast<unsigned int>(dmg.subtype);
			}

			if (target_body)
			{
				hit = true;
			}

			return std::tuple<int, unsigned int, bool, bool>{ damage, damage_type, hit, critical };
		}
	}
}