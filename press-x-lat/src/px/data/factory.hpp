// name: factory.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_DATA_FACTORY_HPP
#define PX_DATA_FACTORY_HPP

#include <px/core/unit.hpp>
#include <px/core/rendering_system.hpp>
#include <px/core/location_system.hpp>
#include <px/core/body_system.hpp>
#include <px/core/character_system.hpp>

#include "unit_composer.hpp"

#include <memory>

namespace px
{
	namespace data
	{
		class factory;
		class task;

		class factory
		{
		private:
			core::rendering_system* m_rs;
			core::location_system* m_ls;
			core::body_system* m_bs;
			core::character_system* m_cs;

		public:
			factory(core::rendering_system& rs, core::location_system &ls, core::body_system &bs, core::character_system &cs)
				: m_rs(&rs), m_ls(&ls), m_bs(&bs), m_cs(&cs)
			{
			}
			virtual ~factory() {}

		public:
			std::unique_ptr<task> produce()
			{
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
		};

		class task : public unit_composer
		{
		private:
			factory* m_factory;

		public:
			task(factory &builder) : m_factory(&builder)
			{
			}
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
			auto add_body()
			{
				auto body = m_factory->make_body();

				body->make_blocking();

				add(body);
				return body;
			}
			auto add_character()
			{
				auto character = m_factory->make_character();

				add(character);
				return character;
			}
		};
	}
}

#endif