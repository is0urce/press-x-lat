// name: factory.cpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#include "factory.hpp"

#include <px/core/unit.hpp>

#include <px/core/sys/rendering_system.hpp>
#include <px/core/sys/location_system.hpp>
#include <px/core/sys/body_system.hpp>
#include <px/core/sys/character_system.hpp>
#include <px/core/sys/behavior_system.hpp>
#include <px/core/sys/resource_component.hpp>
#include <px/core/sys/container_component.hpp>

#include <memory>
#include <mutex>

namespace px
{
	namespace core
	{
		std::unique_ptr<product> factory::produce()
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			return std::make_unique<product>(*this);
		}
		std::shared_ptr<image_component> factory::make_appearance()
		{
			return m_rs->make();
		}
		std::shared_ptr<location_component> factory::make_location()
		{
			return m_ls->make();
		}
		std::shared_ptr<body_component> factory::make_body()
		{
			return m_bs->make();
		}
		std::shared_ptr<character_component> factory::make_character()
		{
			return m_cs->make();
		}
		std::shared_ptr<npc_component> factory::make_npc()
		{
			return m_behavior_sys->make();
		}

		factory::factory(rendering_system& rs, location_system &ls, body_system &bs, character_system &cs, behavior_system &ai_sys)
			: m_rs(&rs), m_ls(&ls), m_bs(&bs), m_cs(&cs), m_behavior_sys(&ai_sys)
		{
		}
		factory::~factory() {}

		product::product(factory &builder)
			: m_factory(&builder) {}
		product::~product() {}

		std::shared_ptr<location_component> product::add_location(point2 position)
		{
			// create
			auto location = m_factory->make_location();

			// setup
			location->move(position);

			// register & return
			add(location);

			return location;
		}
		std::shared_ptr<image_component> product::add_appearance(unsigned int glyph)
		{
			product::add_appearance(glyph, 0xffffff);
		}
		std::shared_ptr<image_component> product::add_appearance(unsigned int glyph, color tint)
		{
			auto appearance = m_factory->make_appearance();

			appearance->glyph = glyph;
			appearance->tint = tint;

			add(appearance);
			return appearance;
		}
		std::shared_ptr<body_component> product::add_body()
		{
			auto body = m_factory->make_body();

			body->make_blocking();

			add(body);
			return body;
		}
		std::shared_ptr<body_component> product::add_body(unsigned int hp, unsigned int mp)
		{
			auto body = m_factory->make_body();

			body->make_blocking();
			body->set_health(hp);
			body->set_energy(mp);

			add(body);
			return body;
		}
		std::shared_ptr<character_component> product::add_character()
		{
			auto character = m_factory->make_character();

			add(character);
			return character;
		}
		std::shared_ptr<npc_component> product::add_npc_behavior()
		{
			auto npc = m_factory->make_npc();

			add(npc);
			return npc;
		}
		std::shared_ptr<resource_component> product::add_resource()
		{
			auto resource = std::make_shared<resource_component>();
			add(resource);
			return resource;
		}
		std::shared_ptr<container_component> product::add_container()
		{
			auto container = std::make_shared<container_component>();
			add(container);
			return container;
		}
		std::shared_ptr<core::unit> product::assemble(persistency status)
		{
			auto unit = std::make_shared<core::unit>();
			unit->set_persistency(status);
			unit->set_location(location().get());

			unit_composer::assemble(*unit);
			return unit;
		}
	}
}