// name: character_system.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "character_system.hpp"

#include <px/core/environment.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

#include <selene.h>

#ifdef WIN64
#pragma comment(lib, "lib/x64/lua53.lib")
#else
#pragma comment(lib, "lib/x32/lua53.lib")
#endif

sel::State state{ true };

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
			state.HandleExceptionsWith([](int code, std::string message, std::exception_ptr exc_ptr) {
				throw std::runtime_error("Lua error: code =" + std::to_string(code) + ", message=" + message);
			});
			state.Load("bash.lua");
			state["do_damage"] = [&](body_component* body, int dmg) {
				if (body)
				{
					auto hp = body->health();
					if (hp)
					{
						hp->damage(dmg);
					}
				}
			};
			state["weapon_damage"] = [&](body_component* body) ->int {
				return body ? body->accumulate<rl::effect::weapon_damage>().value0 : 0;
			};
			state["game"].SetObj(env, "damage", &environment::damage);

			m_spellbook.add_target("melee", [](location_component* user, location_component* target) {
				try
				{
					state["action"](static_cast<body_component*>(*user), static_cast<body_component*>(*target));
				}
				catch (std::exception &exc)
				{
					throw std::runtime_error(std::string("Lua exception, what=") + exc.what());
				}
			}, [&](location_component* user, location_component* target) {
				try
				{
					auto condition = state["condition"](static_cast<body_component*>(*user), static_cast<body_component*>(*target));
					return static_cast<bool>(condition);
				}
				catch (std::exception &exc)
				{
					throw std::runtime_error(std::string("Lua exception, what=") + exc.what());
				}

				//return body && target_body && body != target_body
				//	&& target_body->health() && !target_body->health()->empty() // should have positive hit points
				//	&& env.reputation(*body, *target_body) < 0 // friend-or-foe
				//	&& env.distance(user->current(), target->current()) == 1; // 1 tile melee distance
			});
		}
	}
}
