// name: character_system.cpp
// type: c++ header
// desc: class methods implementation
// auth: is0urce

#include "character_system.hpp"

#include <px/core/environment.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>

#include <px/core/wrap_unit.hpp>

#include <json.hpp>

#include <fstream>

#ifdef WIN64
#pragma comment(lib, "lib/x64/lua53.lib")
#else
#pragma comment(lib, "lib/x32/lua53.lib")
#endif

using json = nlohmann::json;

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
			auto target_action = [this](auto selector) {
				return [selector = selector["action"]](location_component* user, location_component* target) {
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
			auto target_condition = [this](auto selector) {
				return [selector = selector["condition"]](location_component* user, location_component* target) -> bool {
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

			std::fstream fs;
			fs.open("data/skills.json", std::fstream::in);
			json js(fs);

			for (auto skill : js["skills"])
			{
				std::string name = skill;
				m_script.Load(std::string("script/") + name + ".lua");
				auto selector = m_script[name.c_str()];
				if (selector["targeted"])
				{
					auto& skill = m_book.add_target(selector["tag"], target_action(selector), target_condition(selector));

					skill.set_name(selector["name"]);
					skill.set_description(selector["description"]);
					skill.set_hostile(selector["hostile"]);
				}
			}
		}
	}
}
