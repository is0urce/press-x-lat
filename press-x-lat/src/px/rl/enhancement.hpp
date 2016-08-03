// name: enhancement.hpp
// type: c++ header
// desc: struct
// auth: is0urce

#ifndef PX_RL_ENHANCEMENT_HPP
#define PX_RL_ENHANCEMENT_HPP

namespace px
{
	namespace rl
	{
		template<typename Effect>
		struct enhancement
		{
		public:
			typedef Effect effect_type;

		public:
			// type and subtype of enhancement
			effect_type main_type;
			unsigned int subtype;

			// power of enhancement
			int value0;
			int value1;
			double magnitude0;
			double magnitude1;

			// is it visible to player
			bool hidden;

		public:
			enhancement() {}

			enhancement(effect_type t)
				: main_type(t) {}

			enhancement(effect_type t, int value)
				: main_type(t)
				, value0(value) {}
			enhancement(effect_type t, double magnitude)
				: main_type(t)
				, magnitude0(magnitude) {}

			enhancement(effect_type t, int first, int second)
				: main_type(t)
				, value0(first), value1(second) {}
			enhancement(effect_type t, double first, double second)
				: main_type(t)
				, magnitude0(first), magnitude1(second) {}

			enhancement(effect_type t, unsigned int sub)
				: main_type(t), subtype(sub) {}
			enhancement(effect_type t, unsigned int sub, int first, int second)
				: main_type(t), subtype(sub)
				, value0(first), value1(second) {}
			enhancement(effect_type t, unsigned int sub, double first, double second)
				: main_type(t), subtype(sub)
				, magnitude0(first), magnitude1(second) {}

			enhancement(effect_type t, unsigned int sub, int first, int second, bool hide)
				: main_type(t), subtype(sub)
				, value0(first), value1(second)
				, hidden(hide) {}
			enhancement(effect_type t, unsigned int sub, double first, double second, bool hide)
				: main_type(t), subtype(sub)
				, magnitude0(first), magnitude1(second)
				, hidden(hide) {}

			enhancement(effect_type t, unsigned int sub, int i0, int i1, double f0, double f1, bool hide)
				: main_type(t), subtype(sub)
				, value0(i0), value1(i1)
				, magnitude0(f0), magnitude1(f1)
				, hidden(hide) {}
		};
	}
}

#endif