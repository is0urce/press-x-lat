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

		class skill_base
		{
		public:
			typedef unsigned int timespan;
			typedef unsigned int range_component;
			typedef std::pair<range_component, range_component> range;
		private:
			timespan m_cooldown;
			timespan m_timer;

		public:
			skill_base() : m_cooldown(0), m_timer(0) {}
			virtual ~skill_base() {}

		public:
			void set_cooldown_time(timespan time)
			{
				m_cooldown = time;
			}
			void set_cooldown_remaining(timespan time)
			{
				m_timer = time;
			}
			timespan cooldown_time() const
			{
				return m_cooldown;
			}
			timespan cooldown_remaining() const
			{
				return m_timer;
			}
			bool is_cooldown() const
			{
				return m_timer > 0;
			}
			void cooldown(timespan time)
			{
				m_timer = (m_timer < time) ? 0 : m_timer - time;
			}
			void reset_cooldown()
			{
				m_timer = 0;
			}
			void start_cooldown()
			{
				m_timer = m_cooldown;
			}
		};

		template <typename _U, typename _T>
		class skill : public ability<_U, _T>, public skill_base
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