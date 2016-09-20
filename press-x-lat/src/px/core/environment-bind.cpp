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
			rl::damage_type damage_type = rl::damage_type::not_valid;
			bool hit = false;
			bool critical = false;

			body_component* attacker_body = attacker.body();
			body_component* target_body = target.body();

			if (attacker_body)
			{
				auto effect = attacker_body->accumulate<rl::effect::weapon_damage>();
				damage = effect.value0;
				damage_type = static_cast<rl::damage_type>(effect.subtype);
			}

			if (target_body)
			{
				hit = true;
			}

			if (critical)
			{
				damage *= 2;
			}

			return std::tuple<int, unsigned int, bool, bool>{ damage, static_cast<unsigned int>(damage_type), hit, critical };
		}

		int environment::distance_bind(wrap_unit &from, wrap_unit &to)
		{
			auto a = from.location();
			auto b = to.location();
			return a && b ? distance(a->current(), b->current()) : -1;
		}

		void environment::message(wrap_unit &unit, std::string text)
		{
		}
	}
}