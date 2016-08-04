// name: body_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_BODY_COMPONENT_HPP
#define PX_CORE_BODY_COMPONENT_HPP

#include <px/es/i_component.hpp>
#include <px/es/component_link.hpp>

// meat
#include <px/rl/body.hpp>
#include <px/rl/mass.hpp>
#include <px/rl/inventory.hpp>

// enums
#include <px/rl/traverse.hpp>
#include <px/rl/effect.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class character_component;
		class body_component
			: public es::i_component
			, public es::component_link<character_component>
			, public rl::body
			, public rl::mass<static_cast<size_t>(rl::traverse::max_value)>
			, public rl::inventory<rl::effect>
		{
		private:
			item_ptr m_hands;

		public:
			virtual ~body_component() {}

		public:
			item_ptr weapon()
			{
				return m_hands;
			}
			void equip_weapon(item_ptr i)
			{
				if (m_hands) throw std::runtime_error("px::core::body_component::equip_weapon(..) weapon already presents");

				m_hands = i;
			}
		};
	}
}

#endif