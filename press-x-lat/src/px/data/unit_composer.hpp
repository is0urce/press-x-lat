// name: unit_composer.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_DATA_UNIT_COMPOSER_HPP
#define PX_DATA_UNIT_COMPOSER_HPP

#include <px/core/unit.hpp>
#include <px/core/image_component.hpp>
#include <px/core/location_component.hpp>
#include <px/core/body_component.hpp>
#include <px/core/character_component.hpp>

#include <memory>

namespace px
{
	namespace data
	{
		class unit_composer
		{
		public:
			typedef std::shared_ptr<core::location_component> location_ptr;
			typedef std::shared_ptr<core::image_component>  sprite_ptr;
			typedef std::shared_ptr<core::body_component> body_ptr;
			typedef std::shared_ptr<core::character_component> char_ptr;

		private:
			std::shared_ptr<core::unit> m_unit;

			location_ptr m_location;
			sprite_ptr  m_appearance;
			body_ptr m_body;
			char_ptr m_character;

			bool m_done;

		public:
			unit_composer() : m_done(false)
			{
				begin();
			}
			~unit_composer() {}

		public:
			void begin()
			{
				m_location.reset();
				m_appearance.reset();
				m_body.reset();
				m_character.reset();

				m_unit = std::make_shared<core::unit>();

				m_done = false;
			}
			void add(location_ptr l)
			{
				if (m_location) throw std::runtime_error("px::unit_composer::add_location(..) - location component already exists");

				m_location = l;
			}
			void add(sprite_ptr sprite)
			{
				if (m_appearance) throw std::runtime_error("px::unit_composer::add_appearance(sprite) - image component already exists");

				m_appearance = sprite;
			}
			void add(body_ptr b)
			{
				if (m_body) throw std::runtime_error("px::unit_composer::add_body(..) - body component already exists");

				m_body = b;
			}
			void add(char_ptr c)
			{
				if (m_character) throw std::runtime_error("px::unit_composer::add_character(..) - component already exists");

				m_character = c;
			}
			std::shared_ptr<core::unit> assemble()
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

				return m_unit;
			}
		};
	}
}

#endif