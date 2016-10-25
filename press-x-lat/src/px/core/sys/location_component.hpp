// name: image_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_LOCATION_COMPONENT_HPP
#define PX_CORE_LOCATION_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/es/component_link.hpp>
#include <px/common/qtree.hpp>
#include <px/core/location.hpp>

namespace px
{
	namespace core
	{
		class body_component;
		class location_component
			: public es::i_component
			, public es::component_link<body_component>
			, protected location
		{
		public:
			typedef qtree<location_component*> space_type;

		public:
			void incarnate(space_type* space); // add to space
			const point2& current() const;
			void move(point2 destination);
			space_type *const space() const;

		public:
			location_component();
			virtual ~location_component();

		protected:
			virtual void activate_component() override;
			virtual void deactivate_component() override;

		private:
			space_type* m_space;
		};
	}
}

#endif