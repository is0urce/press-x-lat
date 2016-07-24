// name: enhancement.hpp
// type: c++ header
// desc: struct
// auth: is0urce

#ifndef PX_RL_ENHANCEMENT_COLLECTION_HPP
#define PX_RL_ENHANCEMENT_COLLECTION_HPP

#include <px/rl/enhancement.hpp>

namespace px
{
	namespace rl
	{
		template <typename _E>
		class enhancement_collection
		{
		public:
			typedef _E effect_type;
			typedef enhancement<effect_type> enhancement;

		private:
			std::vector<enhancement> m_effects;

		public:

			// manage

			void add(enhancement e)
			{
				m_effects.push_back(e);
			}
			template <typename ...Args> void emplace(Args &&... args)
			{
				m_effects.emplace_back(std::forward<Args>(args)...);
			}
			bool remove(effect_type e)
			{
				bool done = false;
				for (auto it = m_effects.begin(), last = m_effects.end(); it != last; ++it)
				{
					if (it->main_type == e)
					{
						m_effects.erase(it);
						done = true;
						break;
					}
				}
				return done;
			}
			template <typename _Op> bool remove(_Op predicate)
			{
				for (auto it = m_effects.begin(), last = m_effects.end(); it != last; )
				{
					if (predicate(*it))
					{
						it = m_effects.erase(it);
						done = true;
					}
					else
					{
						++it;
					}
				}
				return done;
			}
			void clear()
			{
				m_effects.clear();
			}

			// access

			template <typename _Op> bool enumerate(_Op fn) const
			{
				bool cont = true;
				for (auto it = m_effects.cbegin(), last = m_effects.cend(); cont && it != last; ++it)
				{
					cont = fn(*it);
				}
				return cont;
			}
		};
	}
}
#endif