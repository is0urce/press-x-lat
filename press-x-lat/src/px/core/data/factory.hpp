// name: factory.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_DATA_FACTORY_HPP
#define PX_DATA_FACTORY_HPP

#include <px/common/point.hpp>
#include <px/common/color.hpp>

#include <px/core/unit.hpp>
#include <px/core/data/unit_composer.hpp>

#include <memory>
#include <mutex>

namespace px
{
	namespace core
	{
		class rendering_system;
		class location_system;
		class body_system;
		class character_system;
		class behavior_system;

		class image_component;
		class location_component;
		class body_component;
		class character_component;
		class npc_component;
		class resource_component;
		class container_component;

		class factory;
		class product;

		class factory
		{
		public:
			std::unique_ptr<product> produce();
			std::shared_ptr<image_component> make_appearance();
			std::shared_ptr<location_component> make_location();
			std::shared_ptr<body_component> make_body();
			std::shared_ptr<character_component> make_character();
			std::shared_ptr<npc_component> make_npc();

		public:
			factory(rendering_system& rs, location_system &ls, body_system &bs, character_system &cs, behavior_system &ai_sys);
			factory(const factory&) = delete;
			virtual ~factory();

		private:
			rendering_system* m_rs;
			location_system* m_ls;
			body_system* m_bs;
			character_system* m_cs;
			behavior_system* m_behavior_sys;
			std::mutex m_mutex;
		};

		class product : public unit_composer
		{
		public:
			std::shared_ptr<location_component> add_location(point2 position);
			std::shared_ptr<image_component> add_appearance(unsigned int glyph);
			std::shared_ptr<image_component> add_appearance(unsigned int glyph, color tint);
			std::shared_ptr<body_component> add_body();
			std::shared_ptr<body_component> add_body(unsigned int hp, unsigned int mp);
			std::shared_ptr<character_component> add_character();
			std::shared_ptr<npc_component> add_npc_behavior();
			std::shared_ptr<resource_component> add_resource();
			std::shared_ptr<container_component> add_container();
			std::shared_ptr<unit> assemble(persistency status);
		public:
			product(factory &builder);
			virtual ~product();

		private:
			factory* m_factory;
		};
	}
}

#endif