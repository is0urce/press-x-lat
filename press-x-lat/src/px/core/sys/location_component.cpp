// name: image_component.cpp
// type: c++
// desc: class methods implementation
// auth: is0urce

#include "location_component.hpp"

#include <px/common/qtree.hpp>
#include <px/core/location.hpp>

namespace px
{
	namespace core
	{
		location_component::location_component()
			: m_space(nullptr)
		{
		}
		location_component::~location_component() {}

		void location_component::activate_component()
		{
			if (m_space)
			{
				m_space->add(position, this);
			}
		}
		void location_component::deactivate_component()
		{
			if (m_space)
			{
				m_space->remove(position, this);
			}
		}

		void location_component::incarnate(space_type * space)
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

		const point2& location_component::current() const
		{
			return position;
		}
		void location_component::move(point2 destination)
		{
			if (active() && m_space)
			{
				m_space->move(position, this, destination);
			}
			position = destination;
		}
		location_component::space_type *const location_component::space() const
		{
			return m_space;
		}
	}
}