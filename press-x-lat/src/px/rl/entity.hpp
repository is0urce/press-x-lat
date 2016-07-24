// name: entity.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_ENTITY_HPP
#define PX_RL_ENTITY_HPP

// something with generic characteristics as name

namespace px
{
	namespace rl
	{
		class entity
		{
		private:
			std::string m_name;
			std::string m_description;

		public:
			std::string name() const
			{
				return m_name;
			}
			void set_name(std::string name)
			{
				m_name = name;
			}
		};
	}
}

#endif