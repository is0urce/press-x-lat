// name: character_system.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "character_system.hpp"

#include <px/core/environment.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

#include <px/core/wrap_unit.hpp>

#ifdef WIN64
#pragma comment(lib, "lib/x64/lua53.lib")
#else
#pragma comment(lib, "lib/x32/lua53.lib")
#endif

namespace px
{
	namespace core
	{
		character_system::character_system(environment &env) : m_lua{ true }
		{
			m_lua.HandleExceptionsWith([](int code, std::string message, std::exception_ptr exc_ptr) {
				throw std::runtime_error("Lua error: code =" + std::to_string(code) + ", message=" + message);
			});

			m_lua["unit"].SetClass<wrap_unit, location_component*>(
				"energy", &wrap_unit::energy,
				"health", &wrap_unit::health,
				"damage", &wrap_unit::damage,
				"dead", &wrap_unit::dead,
				"weapon_damage", &wrap_unit::weapon_damage);

			fill(env);
		}
		character_system::~character_system()
		{
			m_spellbook.clear(); // clear closures with references first
		}

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
			auto action = [this](std::string name) {
				return [selector = m_lua[name.c_str()]["action"]](location_component* user, location_component* target) {
					try
					{
						selector(wrap_unit(user), wrap_unit(target));
					}
					catch (std::exception &exc)
					{
						throw std::runtime_error(std::string("Lua exception, what=") + exc.what());
					}
				};
			};
			auto condition = [this](std::string name) {
				return [selector = m_lua[name.c_str()]["condition"]](location_component* user, location_component* target) {
					try
					{
						return static_cast<bool>(selector(wrap_unit(user), wrap_unit(target)));
					}
					catch (std::exception &exc)
					{
						throw std::runtime_error(std::string("Lua exception, what=") + exc.what());
					}
				};
			};

			m_lua.Load("bash.lua");
			m_spellbook.add_target("melee", action("bash"), condition("bash"));
		}
	}
}
