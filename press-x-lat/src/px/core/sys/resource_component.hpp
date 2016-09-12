// name: resource_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_RESOURCE_COMPONENT_HPP
#define PX_CORE_RESOURCE_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>
#include <px/core/sys/i_useable_component.hpp>

namespace px
{
	namespace core
	{
		class resource_component
			: public es::i_component
			, public i_useable_component
		{
		public:
			typedef body_component::item_ptr item_ptr;

		public:
			void deposit(item_ptr resource)
			{
				m_item = resource;
			}
			bool empty() const
			{
				return !m_item;
			}
			item_ptr collect()
			{
				auto result = std::move(m_item);
				return result;
			}

		public:
			resource_component() {}
			virtual ~resource_component() {}

		protected:
			virtual bool useable_component(location_component*, environment&) const override
			{
				return !empty();
			}
			virtual void use_component(location_component* user_pos, environment&) override
			{
				if (body_component* user_body = *user_pos)
				{
					user_body->add(collect());
				}
			}

		private:
			body_component::item_ptr m_item;
		};
	}
}

#endif