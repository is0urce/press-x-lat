// name: enhancement_collection.hpp
// type: c++ header
// desc: struct
// auth: is0urce

// collection of enhancements - single item or buff

#ifndef PX_RL_ENHANCEMENT_COLLECTION_HPP
#define PX_RL_ENHANCEMENT_COLLECTION_HPP

#include <px/rl/enhancement.hpp>

#include <vector>

namespace px
{
	namespace rl
	{
		template <typename Effect>
		class enhancement_collection
		{
		public:
			typedef Effect effect_type;
			typedef enhancement<effect_type> enhancement_type;

		private:
			std::vector<enhancement_type> m_effects;

		public:

			// manage

			void clear()
			{
				m_effects.clear();
			}
			void add(enhancement_type e)
			{
				m_effects.push_back(e);
			}
			template <typename ...Args> void emplace(Args &&... args)
			{
				m_effects.emplace_back(std::forward<Args>(args)...);
			}
			bool remove_first(effect_type e)
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
			template <typename Predicate>
			bool remove_first(Predicate&& predicate)
			{
				bool done = false;
				for (auto it = m_effects.begin(), last = m_effects.end(); it != last; ++it)
				{
					if (std::forward<Predicate>(predicate)(*it))
					{
						it = m_effects.erase(it);
						done = true;
						break;
					}
				}
				return done;
			}

			// access

			template <typename CallbackOperator>
			bool enumerate(CallbackOperator&& fn) const
			{
				bool cont = true;
				for (auto it = m_effects.cbegin(), last = m_effects.cend(); cont && it != last; ++it)
				{
					cont = std::forward<CallbackOperator>(fn)(*it);
				}
				return cont;
			}
		};
	}
}
#endif