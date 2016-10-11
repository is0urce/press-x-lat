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
#include <px/rl/effect.hpp>
#include <px/rl/equipment_slot.hpp>
#include <px/rl/traverse.hpp>

#include <functional>
#include <memory>
#include <list>
#include <vector>

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
				for (auto const& buff : m_buffs)
				{
					start += buff.accumulate<TValue>();
				}

				// item buffs
				for (auto const& item : m_equipment)
				{
					if (item)
					{
						start += item->accumulate<TValue>();
					}
				}

				return start;
			}

			// equipment

			void equip(item_ptr item, rl::equipment_slot slot)
			{
				m_equipment[static_cast<size_t>(slot)] = item;
			}
			void unequip(rl::equipment_slot slot)
			{
				m_equipment[static_cast<size_t>(slot)].reset();
			}
			void clear_equipment()
			{
				for (auto& item : m_equipment)
				{
					item.reset();
				}
			}
			void equip(item_ptr item)
			{
				auto pair = item->find<rl::effect::equipment>();
				if (std::get<0>(pair))
				{
					equip(item, static_cast<rl::equipment_slot>(std::get<1>(pair).subtype));
				}
			}
			item_ptr weapon()
			{
				return m_equipment[static_cast<size_t>(rl::equipment_slot::hand_main)];
			}
			void equip_weapon(item_ptr item)
			{
				equip(item, rl::equipment_slot::hand_main);
			}


		public:
			body_component() : m_equipment(static_cast<size_t>(rl::equipment_slot::max_value) + 1)
			{}
			virtual ~body_component() {}

		private:
			std::vector<item_ptr> m_equipment;
			std::list<buff_type> m_buffs;
		};
	}
}

#endif