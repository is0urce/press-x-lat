// name: body.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_BODY_HPP
#define PX_RL_BODY_HPP

#include <px/rl/bar.hpp>

#include <memory>

namespace px
{
	namespace rl
	{
		class body
		{
		public:
			typedef bar<int> bar;
			typedef std::unique_ptr<bar> bar_ptr;
		private:
			bar_ptr m_hp;
			bar_ptr m_ep;

		public:
			void set_health(bar::value_t amount)
			{
				m_hp = std::make_unique<bar>(amount);
			}
			void set_energy(bar::value_t amount)
			{
				m_hp = std::make_unique<bar>(amount);
			}
			void remove_health()
			{
				m_hp.release();
			}
			void remove_energy()
			{
				m_hp.release();
			}
			const bar* health() const
			{
				return m_hp.get();
			}
			bar* health()
			{
				return m_hp.get();
			}
			const bar* energy() const
			{
				return m_ep.get();
			}
			bar* energy()
			{
				return m_ep.get();
			}
		};
	}
}

#endif