// name: link.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

// component-to-component link
// smart pointer for auto destruction
// expose to pointer for easy casting for multiple inherited links

#ifndef PX_ES_COMPONENT_LINK_HPP
#define PX_ES_COMPONENT_LINK_HPP

#include <memory>
#include <type_traits>

namespace px
{
	namespace es
	{
		template<typename L>
		class component_link
		{
		private:
			std::weak_ptr<L> m_link;

		public:
			component_link() {}

			void link(std::shared_ptr<L> smart_ptr)
			{
				m_link = smart_ptr;
			}
			auto linked() const
			{
				return m_link;
			}
			operator L*() const
			{
				auto shared = m_link.lock();
				return shared ? shared.get() : nullptr;
			}
		};
	}
}

#endif