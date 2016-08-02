// name: link.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

// component-to-component link
// smart pointer for auto destruction
// expose to pointer for easy casting for multiple inherited links

// sfinae resolution in case of multiple inheritance

#ifndef PX_ES_COMPONENT_LINK_HPP
#define PX_ES_COMPONENT_LINK_HPP

#include <memory>
#include <type_traits>

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

			template <typename _C, typename = std::enable_if<std::is_same<_C, _L>::value>::type>
			void link(std::weak_ptr<_C> weak)
			{
				m_link = weak;
			}
			template <typename _C, typename = std::enable_if<std::is_same<_C, _L>::value>::type>
			void link(std::shared_ptr<_C> weak)
			{
				m_link = weak;
			}
			template <typename _C, typename = std::enable_if<std::is_same<_C, _L>::value>::type>
			_C* linked() const
			{
				std::shared_ptr<_L> shared = m_link.lock();
				return shared ? shared.get() : nullptr;
			}
			template <typename _C, typename = std::enable_if<std::is_same<_C, _L>::value>::type>
			operator _C*() const
			{
				return linked<_C>();
			}
		};
	}
}

#endif