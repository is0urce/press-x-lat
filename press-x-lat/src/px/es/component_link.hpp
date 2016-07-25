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

namespace px
{
	namespace es
	{
		template<typename _L>
		class component_link
		{
		private:
			std::weak_ptr<_L> m_link;

		public:
			component_link() {}
			_L* linked() const
			{
				auto shared = m_link.lock();
				return shared ? shared.get() : nullptr;
			}
			void link(std::weak_ptr<_L> weak)
			{
				m_link = weak;
			}
			explicit operator _L*() const
			{
				return linked();
			}
		};
	}
}

#endif