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
#include <px/rl/shadowcasting.hpp>

namespace px
{
	namespace core
	{
		// returns tuple : damage, damage_type, is_hit, is_critical
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

			// critical hits do double damage
			if (critical)
			{
				damage *= 2;
			}

			return std::tuple<int, unsigned int, bool, bool>{ damage, static_cast<unsigned int>(damage_type), hit, critical };
		}

		int environment::distance_wrap(point2 const& from, point2 const& to)
		{
			return distance(from, to);
		}

		int environment::reputation_wrap(wrap_unit &unit, wrap_unit &target)
		{
			body_component* body = unit.body();
			body_component* target_body = target.body();
			return (body && target_body) ? reputation(*body, *target_body) : 0;
		}

		void environment::message(wrap_unit &unit, std::string text)
		{
		}

		wrap_unit environment::first_in_area(point2 const& location, int radius, bool require_fov)
		{
			m_query.clear();

			m_space->find(location.x(), location.y(), radius, [&](int, int, location_component* component)
			{
				m_query.push_back(component);
			});

			if (require_fov)
			{
				rl::shadowcasting fov(radius, location, [this](auto const& position) { return m_terrain.transparent(position); });
				m_query.remove_if([&](auto position) { return !fov.in_fov(position->current()); });
			}

			m_cursor = std::begin(m_query);
			return next();
		}
		wrap_unit environment::next()
		{
			wrap_unit current(nullptr);

			if (m_cursor != m_query.end())
			{
				current = wrap_unit(*m_cursor);
				++m_cursor;
			}

			return current;
		}
	}
}