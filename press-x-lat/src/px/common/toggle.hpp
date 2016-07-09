// name: toggle.hpp
// type: c++ header
// desc: template

// okay, stop reinventing this over and over

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
		void reset()
		{
			m_switch = _Initial;
		}
	};
}

#endif