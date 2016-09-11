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
#include <px/rl/buff.hpp>

// enums
#include <px/rl/traverse.hpp>
#include <px/rl/effect.hpp>

#include <functional>
#include <memory>
#include <list>

namespace px
{
	namespace core
	{
		class i_useable_component;
		class character_component;
		class environment;

		class body_component
			: public es::i_component
			, public es::component_link<character_component>
			, public es::component_link<i_useable_component>
			, public rl::body
			, public rl::mass<static_cast<size_t>(rl::traverse::max_value) + 1>
			, public rl::inventory<rl::effect>
		{
		public:
			typedef std::function<void(environment&, body_component&)> buff_event;
			typedef rl::buff<rl::effect, buff_event> buff_type;

		public:
			using es::component_link<character_component>::link;
			using es::component_link<i_useable_component>::link;
			using rl::inventory<rl::effect>::add;

			bool dead() const
			{
				auto hp = health();
				return hp && hp->empty();
			}

			void add(buff_type buff, environment& env)
			{
				m_buffs.push_back(buff);
				buff.apply(env, *this);
			}
			void tick(environment& env)
			{
				for (auto it = m_buffs.begin(), last = m_buffs.end(); it != last; )
				{
					it->tick(env, *this);
					if (it->expired())
					{
						it->expire(env, *this);
						it = m_buffs.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
			template <typename T, typename BinaryOperation>
			T accumulate(T start, BinaryOperation fold) const
			{
				for (auto &buff : m_buffs)
				{
					start = buff.accumulate(start, fold);
				}
				return start;
			}

			template <effect_type TValue>
			rl::enhancement<rl::effect> accumulate() const
			{
				rl::enhancement<rl::effect> start{};

				// affect buffs
				for (auto &buff : m_buffs)
				{
					start += buff.accumulate<TValue>();
				}

				// item buffs
				if (m_hands)
				{
					start += m_hands->accumulate<TValue>();
				}

				return start;
			}

			// equipment

			item_ptr weapon()
			{
				return m_hands;
			}
			void equip_weapon(item_ptr i)
			{
				if (m_hands) throw std::runtime_error("px::core::body_component::equip_weapon(..) weapon already presents");

				m_hands = i;
			}
			void clear_equipment()
			{
				m_hands = nullptr;
			}

		public:
			virtual ~body_component() {}

		private:
			item_ptr m_hands;
			std::list<buff_type> m_buffs;
		};
	}
}

#endif