// name: i_component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_I_COMPONENT_HPP
#define PX_ES_I_COMPONENT_HPP

namespace px
{
	namespace es
	{
		class i_component
		{
		private:
			bool m_active;

		public:
			i_component() : m_active(false) {}
			virtual ~i_component() = 0 {}

		protected:
			virtual void activate_component() {}
			virtual void deactivate_component() {}

		public:
			bool active() const
			{
				return m_active;
			}
			bool activate()
			{
				activate_component();
				m_active = true;
			}
			bool deactivate()
			{
				deactivate_component();
				m_active = false;
			}
		};
	}
}

#endif