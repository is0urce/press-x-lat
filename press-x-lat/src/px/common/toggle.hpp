// name: toggle.hpp
// type: c++ header
// desc: template

// okay, stop reinventing this over and over
// almost as bad as iheriting from bool

#ifndef PX_COMMON_TOGGLE_HPP
#define PX_COMMON_TOGGLE_HPP

namespace px
{
	template <bool _Initial>
	class toggle
	{
	private:
		bool m_switch;

	public:
		toggle() : m_switch(_Initial)
		{
		}

		bool active() const
		{
			return m_switch;
		}

		void activate()
		{
			m_switch = true;
		}
		void deactivate()
		{
			m_switch = false;
		}
		void reverse_toggle()
		{
			m_switch = !m_switch;
		}
		void reset()
		{
			m_switch = _Initial;
		}
	};
}

#endif