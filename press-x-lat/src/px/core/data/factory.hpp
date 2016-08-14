// name: factory.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_DATA_FACTORY_HPP
#define PX_DATA_FACTORY_HPP

#include <px/core/unit.hpp>

#include <px/core/sys/rendering_system.hpp>
#include <px/core/sys/location_system.hpp>
#include <px/core/sys/body_system.hpp>
#include <px/core/sys/character_system.hpp>
#include <px/core/sys/behavior_system.hpp>
#include <px/core/sys/resource_component.hpp>
#include <px/core/sys/container_component.hpp>

#include "unit_composer.hpp"

#include <memory>
#include <mutex>

namespace px
{
	namespace core
	{
		class factory;
		class task;

		class factory
		{
		public:
			std::unique_ptr<task> produce()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				return std::make_unique<task>(*this);
			}
			auto make_appearance()
			{
				return m_rs->make();
			}
			auto make_location()
			{
				return m_ls->make();
			}
			auto make_body()
			{
				return m_bs->make();
			}
			auto make_character()
			{
				return m_cs->make();
			}
			auto make_npc()
			{
				return m_behavior_sys->make();
			}

		public:
			factory(rendering_system& rs, location_system &ls, body_system &bs, character_system &cs, behavior_system &ai_sys)
				: m_rs(&rs), m_ls(&ls), m_bs(&bs), m_cs(&cs), m_behavior_sys(&ai_sys)
			{
			}
			factory(const factory&) = delete;
			virtual ~factory() {}

		private:
			rendering_system* m_rs;
			location_system* m_ls;
			body_system* m_bs;
			character_system* m_cs;
			behavior_system* m_behavior_sys;
			std::mutex m_mutex;
		};

		class task : public unit_composer
		{
		private:
			factory* m_factory;

		public:
			task(factory &builder)
				: m_factory(&builder) {}
			~task() {}

		public:
			auto add_location(point2 position)
			{
				// create
				auto location = m_factory->make_location();

				// setup
				location->move(position);

				// register & return
				add(location);

				return location;
			}
			auto add_appearance(unsigned int glyph)
			{
				auto appearance = m_factory->make_appearance();

				appearance->glyph = glyph;

				add(appearance);
				return appearance;
			}
			auto add_appearance(unsigned int glyph, color tint)
			{
				auto appearance = m_factory->make_appearance();

				appearance->glyph = glyph;
				appearance->tint = tint;

				add(appearance);
				return appearance;
			}
			auto add_body()
			{
				auto body = m_factory->make_body();

				body->make_blocking();

				add(body);
				return body;
			}
			auto add_body(unsigned int hp, unsigned int mp)
			{
				auto body = m_factory->make_body();

				body->make_blocking();
				body->set_health(hp);
				body->set_energy(mp);

				add(body);
				return body;
			}
			auto add_character()
			{
				auto character = m_factory->make_character();

				add(character);
				return character;
			}
			auto add_npc_behavior()
			{
				auto npc = m_factory->make_npc();

				add(npc);
				return npc;
			}
			auto add_resource()
			{
				auto resource = std::make_shared<resource_component>();
				add(resource);
				return resource;
			}
			auto add_container()
			{
				auto container = std::make_shared<container_component>();
				add(container);
				return container;
			}
			std::shared_ptr<core::unit> assemble(persistency status)
			{
				auto unit = std::make_shared<core::unit>();
				unit->set_persistency(status);
				unit->set_location(location().get());

				unit_composer::assemble(*unit);
				return unit;
			}
		};
	}
}

#endif