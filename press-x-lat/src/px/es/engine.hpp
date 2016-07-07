// name: engine.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_ES_ENGINE_HPP
#define PX_ES_ENGINE_HPP

#include "i_system.hpp"

#include <list>
#include <memory>

namespace px
{
	namespace es
	{
		class engine
		{
		private:
			std::list<i_system*> m_systems;

		public:
			engine() {}
			virtual ~engine() {}

			void add(i_system* system)
			{
				m_systems.push_back(system);
			}
			
		public:
			void update()
			{
				for (auto &system : m_systems)
				{
					system->update();
				}
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