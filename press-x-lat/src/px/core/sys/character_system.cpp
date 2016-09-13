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
		character_system::character_system(environment &env)
			: m_script{ true }
		{
			m_script.HandleExceptionsWith([](int code, std::string message, std::exception_ptr exc_ptr) {
				throw std::runtime_error("Lua error: code#" + std::to_string(code) + ", message=" + message);
			});

			m_script["unit"].SetClass<wrap_unit, location_component*>(
				"energy", &wrap_unit::energy,
				"health", &wrap_unit::health,
				"damage", &wrap_unit::damage,
				"drain", &wrap_unit::drain,
				"dead", &wrap_unit::dead,
				"weapon_damage", &wrap_unit::weapon_damage);

			m_script["point"].SetClass<point2, int, int>();

			fill(env);
		}
		character_system::~character_system()
		{
			m_book.clear(); // clear closures with references first
		}

		void character_system::element_allocated(character_component &element)
		{
			element.deactivate();
			element.provide(&m_book);
		}

		character_system::skillbook_type* character_system::skill_book()
		{
			return &m_book;
		}

		void character_system::fill(environment &env)
		{
			auto action = [this](const char *c_name) {
				return [selector = m_script[c_name]["action"]](location_component* user, location_component* target) {
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
			auto condition = [this](const char *c_name) {
				return [selector = m_script[c_name]["condition"]](location_component* user, location_component* target) -> bool {
					try
					{
						return selector(wrap_unit(user), wrap_unit(target));
					}
					catch (std::exception &exc)
					{
						throw std::runtime_error(std::string("Lua exception, what=") + exc.what());
					}
				};
			};

			m_script.Load("bash.lua");
			m_script.Load("skills.lua");

			const char * name = "bash";
			auto skills = m_script["skills"];

			unsigned int i = 0;
			bool exist = true;
			do
			{
				++i;
				auto skill = m_script["x"];// skills["one"];
				exist = static_cast<bool>(i <= 1);

				std::string name = skill;
				auto c_name = name.c_str();


				if (exist)
				{
					if (m_script[c_name]["targeted"])
					{
						auto& skill = m_book.add_target(m_script[c_name]["tag"], action(c_name), condition(c_name));

						skill.set_name(m_script[c_name]["name"]);
						skill.set_description(m_script[c_name]["description"]);
						skill.set_hostile(m_script[c_name]["hostile"]);
					}
				}
			}
			while (exist);

			if (m_script[name]["targeted"])
			{
				auto& skill = m_book.add_target("melee", action(name), condition(name));

				skill.set_name(m_script[name]["name"]);
				skill.set_description(m_script[name]["description"]);
				skill.set_hostile(m_script[name]["hostile"]);
			}
		}
	}
}
