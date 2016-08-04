// name: unit_composer.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_DATA_UNIT_COMPOSER_HPP
#define PX_DATA_UNIT_COMPOSER_HPP

#include <px/core/unit.hpp>
#include <px/core/sys/image_component.hpp>
#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>
#include <px/core/sys/character_component.hpp>
#include <px/core/sys/npc_component.hpp>
#include <px/core/sys/resource_component.hpp>

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
			typedef std::shared_ptr<core::npc_component> npc_ptr;
			typedef std::shared_ptr<core::resource_component> resource_ptr;

		private:
			std::shared_ptr<core::unit> m_unit;

			location_ptr m_location;
			sprite_ptr  m_appearance;
			body_ptr m_body;
			char_ptr m_character;
			npc_ptr m_npc;
			resource_ptr m_resource;

			bool m_done;

		public:
			unit_composer();
			~unit_composer();
			unit_composer(const unit_composer&) = delete;

		private:
			void clear();

		public:
			template <core::persistency _P>
			void begin()
			{
				m_unit = std::make_shared<core::unit>();
				m_unit->set_persistency(_P);
			}
			void begin(core::persistency p)
			{
				m_unit = std::make_shared<core::unit>();
				m_unit->set_persistency(p);
			}
			void end();
			void add(location_ptr l);
			void add(sprite_ptr sprite);
			void add(body_ptr b);
			void add(char_ptr c);
			void add(npc_ptr npc);
			void add(resource_ptr resource);
			std::shared_ptr<core::unit> assemble();
		};
	}
}

#endif