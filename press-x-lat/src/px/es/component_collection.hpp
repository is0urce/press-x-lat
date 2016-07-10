// name: component_collection.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_COMPONENT_COLLECTION_HPP
#define PX_ES_COMPONENT_COLLECTION_HPP

#include "i_component.hpp"

#include <list>
#include <memory>

namespace px
{
	namespace es
	{
		class component_collection
		{
		public:
			typedef std::shared_ptr<i_component> component_ptr;
		private:
			std::list<component_ptr> m_components;

		public:
			void add(component_ptr c)
			{
				m_components.push_back(c);
			}
			// remove specified component (O=n)
			void remove(component_ptr c)
			{
				m_components.remove(c);
			}
			// remove all components
			void clear()
			{
				m_components.clear();
			}

			template <typename _Operator>
			void enumerate(_Operator fn)
			{
				for (auto it = m_components.begin(), end = m_components.end(); it != end; ++it)
				{
					if (*it);
				}
			}

			// querry component by type
			template<typename _Cast>
			std::shared_ptr<_Cast> component() const
			{
				std::shared_ptr<_Cast> cast;
				for (auto it = m_components.begin(), end = m_components.end(); it != end; ++it)
				{
					cast = std::dynamic_pointer_cast<_Cast>(*i);
					if (cast) break;
				}
				if (it == end) throw std::runtime_error("px::es::component_collection::component<c>() - we're not serving subclasses here");
				return cast;
			}

			// remove component by type
			template<typename _C>
			void remove()
			{
				for (auto it = m_components.begin(), last = m_components.end(); it != last; ++it)
				{
					auto cast = std::dynamic_pointer_cast<_C>(*it);
					if (cast)
					{
						m_components.erase(it);
						break;
					}
				}
				if (it == end) throw std::runtime_error("px::es::component_collection::remove<c>() - component not detected");
				return false;
			}

			void activate()
			{
				for (auto it = m_components.begin(), end = m_components.end(); it != end; ++it)
				{
					(*it)->activate();
				}
			}
			void deactivate()
			{
				for (auto it = m_components.begin(), end = m_components.end(); it != end; ++it)
				{
					(*it)->deactivate();
				}
			}
		};
	}
}

#endif