// name: unit_composer.cpp
// type: c++
// desc: class
// auth: is0urce

#include "unit_composer.hpp"

#include <px/core/sys/image_component.hpp>
#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>
#include <px/core/sys/character_component.hpp>
#include <px/core/sys/npc_component.hpp>
#include <px/core/sys/resource_component.hpp>
#include <px/core/sys/container_component.hpp>

#include <stdexcept>

namespace px
{
	namespace core
	{
		unit_composer::unit_composer() : m_done(false)
		{
			clear();
		}
		unit_composer::~unit_composer() {}

		void unit_composer::clear()
		{
			m_location = nullptr;
			m_appearance = nullptr;
			m_body = nullptr;
			m_character = nullptr;
			m_npc = nullptr;
			m_resource = nullptr;
			m_container = nullptr;
		}
		void unit_composer::add(location_ptr l)
		{
			if (!l) throw std::runtime_error("px::unit_composer::add(..) - component is null");
			if (m_location) throw std::runtime_error("px::unit_composer::add(location) - location component already exists");

			m_location = l;
		}
		unit_composer::location_ptr unit_composer::location()
		{
			return m_location;
		}
		void unit_composer::add(sprite_ptr sprite)
		{
			if (!sprite) throw std::runtime_error("px::unit_composer::add(..) - component is null");
			if (m_appearance) throw std::runtime_error("px::unit_composer::add(sprite_ptr) - image component already exists");

			m_appearance = sprite;
		}
		void unit_composer::add(body_ptr b)
		{
			if (!b) throw std::runtime_error("px::unit_composer::add(..) - component is null");
			if (m_body) throw std::runtime_error("px::unit_composer::add(body_ptr) - body component already exists");

			m_body = b;
		}
		void unit_composer::add(char_ptr c)
		{
			if (!c) throw std::runtime_error("px::unit_composer::add(..) - component is null");
			if (m_character) throw std::runtime_error("px::unit_composer::add(character) - component already exists");

			m_character = c;
		}
		void unit_composer::add(npc_ptr npc)
		{
			if (!npc) throw std::runtime_error("px::unit_composer::add(..) - component is null");
			if (m_npc) throw std::runtime_error("px::unit_composer::add(npc_ptr) - component already exists");

			m_npc = npc;
		}
		void unit_composer::add(resource_ptr resource)
		{
			if (!resource) throw std::runtime_error("px::unit_composer::add(..) - component is null");
			if (m_resource) throw std::runtime_error("px::unit_composer::add(resource_ptr) - component already exists");

			m_resource = resource;
		}
		void unit_composer::add(container_ptr container)
		{
			if (!container) throw std::runtime_error("px::unit_composer::add(..) - component is null");
			if (m_npc) throw std::runtime_error("px::unit_composer::add(container_ptr) - component already exists");

			m_container = container;
		}
		void unit_composer::assemble(es::component_collection& container)
		{
			if (m_body)
			{
				container.add(m_body);
				m_body->link(m_character);
			}
			if (m_appearance)
			{
				container.add(m_appearance);
				m_appearance->link(m_location);
			}
			if (m_location)
			{
				container.add(m_location);
				m_location->link(m_body);
			}
			if (m_character)
			{
				container.add(m_character);
			}
			if (m_npc)
			{
				container.add(m_npc);
				m_npc->link(m_location);
				if (m_body) m_body->link(m_npc);
			}
			if (m_resource)
			{
				container.add(m_resource);
				if (m_body)	m_body->link(m_resource);
			}
			if (m_container)
			{
				container.add(m_container);
				m_container->link(m_body);
				if (m_body) m_body->link(m_container);
			}

			clear();
		}
	}
}