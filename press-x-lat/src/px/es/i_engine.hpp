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

			virtual void update_engine() {}
			virtual void fixed_update_engine() {}

		public:
			void add(i_system* system)
			{
				m_systems.push_back(system);
			}
			
			void update()
			{
				for (auto &system : m_systems)
				{
					system->update();
				}
				update_engine();
			}
			void fixed_update()
			{
				for (auto &system : m_systems)
				{
					system->fixed_update();
				}
				fixed_update_engine();
			}
		};
	}
}

#endif