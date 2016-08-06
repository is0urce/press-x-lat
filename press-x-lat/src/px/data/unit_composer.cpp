// name: unit_composer.cpp
// type: c++
// desc: class
// auth: is0urce

#include "unit_composer.hpp"

#include <px/core/unit.hpp>
#include <px/core/sys/image_component.hpp>
#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>
#include <px/core/sys/character_component.hpp>
#include <px/core/sys/npc_component.hpp>

#include <stdexcept>

namespace px
{
	namespace data
	{
		unit_composer::unit_composer() : m_done(false)
		{
			clear();
		}
		unit_composer::~unit_composer() {}

		void unit_composer::clear()
		{
			m_location.reset();
			m_appearance.reset();
			m_body.reset();
			m_character.reset();
		}

		void unit_composer::end()
		{
			clear();
		}
		void unit_composer::add(location_ptr l)
		{
			if (m_location) throw std::runtime_error("px::unit_composer::add(location) - location component already exists");

			m_location = l;
		}
		void unit_composer::add(sprite_ptr sprite)
		{
			if (m_appearance) throw std::runtime_error("px::unit_composer::add(sprite_ptr) - image component already exists");

			m_appearance = sprite;
		}
		void unit_composer::add(body_ptr b)
		{
			if (m_body) throw std::runtime_error("px::unit_composer::add(body_ptr) - body component already exists");

			m_body = b;
		}
		void unit_composer::add(char_ptr c)
		{
			if (m_character) throw std::runtime_error("px::unit_composer::add(character) - component already exists");

			m_character = c;
		}
		void unit_composer::add(npc_ptr npc)
		{
			if (m_npc) throw std::runtime_error("px::unit_composer::add(npc_ptr) - component already exists");

			m_npc = npc;
		}
		void unit_composer::add(resource_ptr resource)
		{
			if (m_resource) throw std::runtime_error("px::unit_composer::add(resource_ptr) - component already exists");

			m_resource = resource;
		}
		void unit_composer::add(container_ptr container)
		{
			if (m_npc) throw std::runtime_error("px::unit_composer::add(container_ptr) - component already exists");

			m_container = container;
		}
		std::shared_ptr<core::unit> unit_composer::assemble()
		{
			if (m_done) throw std::runtime_error("px::unit_composer::assemble - unit already created");
			m_done = true;

			// add components
			if (m_body)
			{
				m_unit->add(m_body);

				m_body->link(m_character);
			}
			if (m_appearance)
			{
				m_unit->add(m_appearance);
				m_appearance->link(m_location);
			}
			if (m_location)
			{
				m_unit->add(m_location);
				m_location->link(m_body);
			}
			if (m_character)
			{
				m_unit->add(m_character);
			}
			if (m_npc)
			{
				m_unit->add(m_npc);
				m_npc->link(m_location);
			}
			if (m_resource)
			{
				m_unit->add(m_resource);
				if (m_body)	m_body->link(m_resource);
			}
			if (m_container)
			{
				m_unit->add(m_container);
				m_container->link(m_body);
				if (m_body) m_body->link(m_container);
			}

			return m_unit;
		}
	}
}