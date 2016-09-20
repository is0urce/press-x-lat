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
			setup_scripts(env);
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
		void character_system::element_released(character_component &element)
		{
			element.clear_skills(); // clear closures with references
		}
		void character_system::fixed_update_system()
		{
			enumerate([this](auto &character)
			{
				if (character.active())
				{
					character.cooldown(1);
				}
			});
		}

		void character_system::setup_scripts(environment &env)
		{
			// error handling
			m_script.HandleExceptionsWith([](int code, std::string message, std::exception_ptr exc_ptr) {
				throw std::runtime_error("Lua error: code#" + std::to_string(code) + ", message=" + message);
			});

			// 'unit' script object
			m_script["unit"].SetClass<wrap_unit, location_component*>(
				"move", &wrap_unit::move,
				"energy", &wrap_unit::energy,
				"health", &wrap_unit::health,
				"damage", &wrap_unit::damage,
				"restore", &wrap_unit::restore,
				"drain", &wrap_unit::drain,
				"dead", &wrap_unit::dead,
				"weapon_damage", &wrap_unit::weapon_damage);

			// 'point' script object
			m_script["point"].SetClass<point2, int, int>();

			// 'game' script object
			m_script["game"].SetObj(env,
				"distance", &environment::distance_bind,

				"message", &environment::message,
				"hit", &environment::hit);
		}

		character_system::book_type* character_system::book()
		{
			return &m_book;
		}

		void character_system::fill(environment &)
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
			auto ground_action = [this](auto selector) {
				return[selector = selector["action"]](location_component* user, point2 target) {
					try
					{
						selector(wrap_unit(user), target);
					}
					catch (std::exception &exc)
					{
						throw std::runtime_error(std::string("Lua exception, what=") + exc.what());
					}
				};
			};
			auto ground_condition = [this](auto selector) {
				return[selector = selector["condition"]](location_component* user, point2 target) -> bool {
					try
					{
						return selector(wrap_unit(user), target);
					}
					catch (std::exception &exc)
					{
						throw std::runtime_error(std::string("Lua exception, what=") + exc.what());
					}
				};
			};

			json js(std::fstream("data/skills.json", std::fstream::in));
			for (auto s : js["skills"])
			{
				std::string name = s;
				m_script.Load(std::string("script/") + name + ".lua");

				auto selector = m_script[name.c_str()];
				auto &skill = selector["targeted"] ? 
					m_book.add_target(selector["tag"], target_action(selector), target_condition(selector))	:
					m_book.add_ground(selector["tag"], ground_action(selector), ground_condition(selector));

				skill.set_name(selector["name"]);
				skill.set_alias(selector["short"]);
				skill.set_description(selector["description"]);
				skill.set_hostile(selector["hostile"]);
			}
		}
	}
}
