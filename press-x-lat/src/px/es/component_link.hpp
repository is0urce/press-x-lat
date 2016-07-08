// name: link.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

// component-to-component link
// smart pointer for auto desptruction
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
		public:
			typedef std::weak_ptr<_L> link_t;
			typedef _L* expose_t;
		private:
			link_t m_link;
		public:
			component_link() {}
			expose_t link() const
			{
				auto shared = m_link.lock();
				return shared ? shared.get() : nullptr;
			}
			void link(link_t link)
			{
				m_link = link;
			}
			explicit operator expose_t() const
			{
				return link();
			}
		};
	}
}

#endif