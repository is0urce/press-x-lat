// name: unit_composer.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_DATA_UNIT_COMPOSER_HPP
#define PX_DATA_UNIT_COMPOSER_HPP

#include <px/es/component_collection.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class image_component;
		class location_component;
		class body_component;
		class character_component;
		class npc_component;
		class resource_component;
		class container_component;

		class unit_composer
		{
		public:
			typedef std::shared_ptr<location_component> location_ptr;
			typedef std::shared_ptr<image_component>  sprite_ptr;
			typedef std::shared_ptr<body_component> body_ptr;
			typedef std::shared_ptr<character_component> char_ptr;
			typedef std::shared_ptr<npc_component> npc_ptr;
			typedef std::shared_ptr<resource_component> resource_ptr;
			typedef std::shared_ptr<container_component> container_ptr;

		public:
			void add(location_ptr l);
			void add(sprite_ptr sprite);
			void add(body_ptr b);
			void add(char_ptr c);
			void add(npc_ptr npc);
			void add(resource_ptr resource);
			void add(container_ptr container);
			location_ptr location();
			void assemble(es::component_collection& container);

		public:
			unit_composer();
			virtual ~unit_composer();
			unit_composer(const unit_composer&) = delete;

		private:
			void clear();

		private:
			location_ptr m_location;
			sprite_ptr  m_appearance;
			body_ptr m_body;
			char_ptr m_character;
			npc_ptr m_npc;
			resource_ptr m_resource;
			container_ptr m_container;
			bool m_done;
		};
	}
}

#endif