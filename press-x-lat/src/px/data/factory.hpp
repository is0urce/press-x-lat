// name: factory.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_DATA_FACTORY_HPP
#define PX_DATA_FACTORY_HPP

#include <px/core/unit.hpp>
#include <px/core/rendering_system.hpp>
#include <px/core/location_system.hpp>

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

		public:
			factory(core::rendering_system& rs, core::location_system &ls) : m_rs(&rs), m_ls(&ls) {}
			~factory() {}

		public:
			std::unique_ptr<task> produce()
			{
				return std::make_unique<task>(*this);
			}
			auto make_appearance() -> decltype(m_rs->make())
			{
				return m_rs->make();
			}
			auto make_location() -> decltype(m_ls->make())
			{
				return m_ls->make();
			}
		};

		class task
		{
		private:
			factory* m_factory;
			std::shared_ptr<core::location_component> m_location;
			std::shared_ptr<core::image_component> m_appearance;
			std::shared_ptr<core::unit> m_unit;
			bool m_done;
		public:
			task(factory& f) : m_factory(&f), m_unit(std::make_shared<core::unit>()), m_done(false) {}
			~task() {}

		public:
			auto add_location(point2 position) -> decltype(m_location)
			{
				if (m_location) throw std::runtime_error("px::core::factory::task - location component already exists");
				m_location = m_factory->make_location();

				m_location->move(position);

				return m_location;
			}
			auto add_appearance(unsigned int glyph) -> decltype(m_appearance)
			{
				if (m_appearance) throw std::runtime_error("px::core::factory::task - image component already exists");
				m_appearance = m_factory->make_appearance();

				m_appearance->glyph = glyph;

				return m_appearance;
			}
			std::shared_ptr<core::unit> assemble()
			{
				if (m_done) throw std::runtime_error("px::core::factory::task::assemble - unit already created");
				m_done = true;

				// add components
				m_unit->add(m_location);
				m_unit->add(m_appearance);

				// setup links
				if (m_appearance)
				{
					if (m_location)
					{
						m_appearance->link(m_location);
					}
				}

				return m_unit;
			}
		};
	}
}

#endif