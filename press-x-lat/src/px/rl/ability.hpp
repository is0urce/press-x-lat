// name: ability.hpp
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
		class ability
		{
		public:
			typedef _U user;
			typedef _T target;

		public:
			virtual ~ability() = 0 {}

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
		};

		template <typename _U, typename _T>
		class delegate_ability : public ability<_U, _T>
		{
		public:
			typedef std::function<void(_U, target)> target_fn;
			typedef std::function<bool(_U, target)> target_check_fn;
			typedef std::function<void(_U, const point2&)> ground_fn;
			typedef std::function<bool(_U, const point2&)> ground_check_fn;

		private:
			bool m_targeted;
			target_fn m_target;
			target_check_fn m_target_check;
			ground_fn m_ground;
			ground_check_fn m_ground_check;

		public:
			delegate_ability(target_fn tf, target_check_fn tfc) :
				m_targeted(true),
				m_target(tf), m_target_check(tfc)
			{
				if (!tf) throw std::logic_error("px::targeted_ability::targeted_ability() - tf is null");
				if (!tfc) throw std::logic_error("px::targeted_ability::targeted_ability() - tfc is null");
			}
			delegate_ability(ground_fn gf, ground_check_fn gfc) :
				m_targeted(false),
				m_ground(gf), m_ground_check(gfc)
			{
				if (!gf) throw std::logic_error("px::targeted_ability::targeted_ability() - gf is null");
				if (!gfc) throw std::logic_error("px::targeted_ability::targeted_ability() - gfc is null");
			}
			virtual ~delegate_ability() {}

		public:
			static delegate_ability create_ground(ground_fn gf, ground_check_fn gfc)
			{
				return delegate_ability(gf, gfc);
			}
			static delegate_ability create_target(target_fn tf, target_check_fn tfc)
			{
				return delegate_ability(tf, tfc);
			}

		protected:
			virtual bool targeted_ability() const override
			{
				return m_targeted;
			}
			virtual void use_ability(_U u, _T t) override
			{
				if (!m_target) throw std::logic_error("px::targeted_ability::use_abitity(..) - m_target is null");

				m_target(u, t);
			}
			virtual void use_ability(_U u, const point2 &t) override
			{
				if (!m_ground) throw std::logic_error("px::targeted_ability::use_abitity(..) - m_ground is null");

				m_ground(u, t);
			}
			virtual bool useable_ability(_U u, _T t) const override
			{
				if (!m_target_check) throw std::logic_error("px::targeted_ability::useable_ability(..) - m_target_check is null");

				return m_target_check(u, t);
			}
			virtual bool useable_ability(_U u, const point2 &t) const override
			{
				if (!m_ground_check) throw std::logic_error("px::targeted_ability::useable_ability(..) - m_ground_check is null");

				return m_ground_check(u, t);
			}
		};
	}
}

#endif