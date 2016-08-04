// name: i_ability.hpp
// type: c++ header
// desc: interface template class declaration
// auth: is0urce

// abstract ability used on target or on specified position

#ifndef PX_RL_ABILITY_HPP
#define PX_RL_ABILITY_HPP

#include <px/common/point.hpp>

namespace px
{
	namespace rl
	{
		template <typename _U, typename _T>
		class i_ability
		{
		public:
			typedef _U user;
			typedef _T target;

		public:
			virtual ~i_ability() = 0 {}

		protected:
			virtual void use_ability(_U u, _T t) = 0;
			virtual void use_ability(_U u, const point2 &t) = 0;
			virtual bool useable_ability(_U u, _T t) const = 0;
			virtual bool useable_ability(_U u, const point2 &t) const = 0;
			virtual bool targeted_ability() const = 0;

		public:
			bool targeted() const { return targeted_ability(); }
			bool useable(_U u, _T t) const { return useable_ability(u, t); }
			bool useable(_U u, const point2 &t) const { return useable_ability(u, t); }
			void use(_U u, _T t) { use_ability(u, t); }
			void use(_U u, const point2 &t) { use_ability(u, t); }

			bool try_use(_U u, _T t)
			{
				if (useable_ability(u, t))
				{
					use_ability(u, t);
					return true;
				}
				return false;
			}
			bool try_use(_U u, const point2 &t)
			{
				if (useable_ability(u, t))
				{
					use_ability(u, t);
					return true;
				}
				return false;
			}
		};
	}
}

#endif