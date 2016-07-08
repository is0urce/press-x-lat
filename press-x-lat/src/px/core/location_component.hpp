// name: image_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_LOCATION_COMPONENT_HPP
#define PX_CORE_LOCATION_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/common/qtree.hpp>
#include "location.hpp"

namespace px
{
	namespace core
	{
		class location_component : public es::i_component, protected location
		{
		//public:
		//	typedef qtree<location_component*> space_t;
		private:
			qtree<location_component*>* m_space;
		public:
			location_component() : m_space(nullptr) {}
			virtual ~location_component() {}

		protected:
			virtual void activate_component() override
			{
				if (m_space)
				{
					m_space->add(position, this);
				}
			}
			virtual void deactivate_component() override
			{
				if (m_space)
				{
					m_space->remove(position, this);
				}
			}

		public:
			// lame mnemonics > good names
			void incarnate(qtree<location_component*>* space)
			{
				if (m_space != space)
				{
					if (active())
					{
						if (m_space)
						{
							m_space->remove(position, this);
						}
						if (space)
						{
							space->add(position, this);
						}
					}
					m_space = space;
				}
			}
			const point2& current() const
			{
				return position;
			}
			void move(point2 destination)
			{
				if (active() && m_space)
				{
					m_space->move(position, this, destination);
				}
				position = destination;
			}
		};
	}
}

#endif