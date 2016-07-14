// name: engine.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_I_ENGINE_HPP
#define PX_ES_I_ENGINE_HPP

#include "i_system.hpp"

#include <list>
#include <memory>

namespace px
{
	namespace es
	{
		class i_engine
		{
		private:
			std::list<i_system*> m_systems;

		public:
			i_engine() {}
			virtual ~i_engine() {}

			virtual void pre_update_engine() {}
			virtual void post_update_engine() {}
			virtual bool fixed() { return false; }

		public:
			void add(i_system* system)
			{
				m_systems.push_back(system);
			}
			void remove(i_system* system)
			{
				m_systems.remove(system);
			}
			void clear()
			{
				m_systems.clear();
			}
			
			void update()
			{
				if (fixed())
				{
					fixed_update();
				}

				pre_update_engine();
				for (auto &system : m_systems)
				{
					system->pre_update();
				}
				for (auto &system : m_systems)
				{
					system->update();
				}
				for (auto &system : m_systems)
				{
					system->post_update();
				}
				post_update_engine();
			}
			void fixed_update()
			{
				for (auto &system : m_systems)
				{
					system->fixed_update();
				}
			}
		};
	}
}

#endif