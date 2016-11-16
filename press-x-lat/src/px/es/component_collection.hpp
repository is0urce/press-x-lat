// name: component_collection.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_COMPONENT_COLLECTION_HPP
#define PX_ES_COMPONENT_COLLECTION_HPP

#include "i_component.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace px
{
	namespace es
	{
		class component_collection
		{
		public:
			typedef std::shared_ptr<i_component> component_ptr;
		private:
			std::vector<component_ptr> m_components;

		public:
			void add(component_ptr c)
			{
				m_components.push_back(c);
			}
			// remove specified component (O=n)
			void remove(component_ptr c)
			{
				m_components.erase(std::remove(std::begin(m_components), std::end(m_components), c));
			}
			// remove all components
			void clear()
			{
				m_components.clear();
			}

			template <typename Operator>
			void enumerate(Operator fn)
			{
				for (auto & component : m_components)
				{
					std::forward<Operator>(fn)(component);
				}
			}

			// querry component by type
			template<typename Cast>
			std::shared_ptr<Cast> component() const
			{
				std::shared_ptr<Cast> cast;
				for (auto & component : m_components.end())
				{
					cast = std::dynamic_pointer_cast<Cast>(component);
					if (cast) break;
				}
				if (!cast) throw std::runtime_error("px::es::component_collection::component<c>() - we're not serving subclasses here");
				return cast;
			}

			// remove component by type
			template<typename C>
			void remove()
			{
				for (auto it = m_components.begin(), last = m_components.end(); it != last; ++it)
				{
					auto cast = std::dynamic_pointer_cast<C>(*it);
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
				for (auto & component : m_components)
				{
					component->activate();
				}
			}
			void deactivate()
			{
				for (auto & component : m_components)
				{
					component->deactivate();
				}
			}
		};
	}
}

#endif