// name: skill.hpp
// type: c++ header
// desc: class
// auth: is0urce

#ifndef PX_RL_SKILL_HPP
#define PX_RL_SKILL_HPP

#include <px/rl/i_ability.hpp>
#include <px/rl/skill_base.hpp>

namespace px
{
	namespace rl
	{
		template <typename _U, typename _T>
		class skill : public i_ability<_U, _T>, public skill_base
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
			skill(target_fn tf, target_check_fn tfc) :
				m_targeted(true),
				m_target(tf), m_target_check(tfc)
			{
				if (!tf) throw std::logic_error("px::targeted_ability::targeted_ability() - tf is null");
			}
			skill(target_fn tf) :
				m_targeted(true),
				m_target(tf)
			{
				if (!tf) throw std::logic_error("px::targeted_ability::targeted_ability() - tf is null");
			}
			skill(ground_fn gf, ground_check_fn gfc) :
				m_targeted(false),
				m_ground(gf), m_ground_check(gfc)
			{
				if (!gf) throw std::logic_error("px::targeted_ability::targeted_ability() - gf is null");
			}
			skill(ground_fn gf) :
				m_targeted(false),
				m_ground(gf)
			{
				if (!gf) throw std::logic_error("px::targeted_ability::targeted_ability() - gf is null");
			}
			virtual ~skill() {}

		public:
			static skill create_ground(ground_fn gf, ground_check_fn gfc)
			{
				return skill(gf, gfc);
			}
			static skill create_target(target_fn tf, target_check_fn tfc)
			{
				return skill(tf, tfc);
			}

		protected:
			virtual bool targeted_ability() const override
			{
				return m_targeted;
			}
			virtual void use_ability(_U u, _T t) override
			{
				if (!m_target) throw std::logic_error("px::targeted_ability::use_abitity(..) - m_target function is null");

				m_target(u, t);
			}
			virtual void use_ability(_U u, const point2 &t) override
			{
				if (!m_ground) throw std::logic_error("px::targeted_ability::use_abitity(..) - m_ground function is null");

				m_ground(u, t);
			}
			virtual bool useable_ability(_U u, _T t) const override
			{
				return !(is_cooldown() || (m_target_check && !m_target_check(u, t)));
			}
			virtual bool useable_ability(_U u, const point2 &t) const override
			{
				return !(is_cooldown() || (m_ground_check && !m_ground_check(u, t)));
			}
		};
	}
}

#endif