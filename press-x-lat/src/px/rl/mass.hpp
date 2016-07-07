// name: mass.hpp
// type: c++ header
// desc: tile class
// auth: is0urce

// base of shared tile and unit proprieties

#ifndef PX_RL_MASS_HPP
#define PX_RL_MASS_HPP

#include <bitset>

namespace px
{
	namespace rl
	{
		template <unsigned int layers = 1>
		class mass
		{
		public:
			typedef std::bitset<layers> bitset;

			// attributes
		private:
			bool m_transparent;
			std::bitset<layers> m_traversable;

			// ctor & dtor
		public:
			mass() {}
			mass(bool transparent) : m_transparent(transparent) {}
			mass(bool is_transparent, bool is_traversable) : m_transparent(is_transparent) { traversable(is_traversable); }
			mass(bool is_transparent, bitset traversable) : m_transparent(is_transparent), m_traversable(traversable) {}

		public:

			// transparency accessors

			bool transparent() const { return m_transparent; }

			void make_transparent(bool transparency) { m_transparent = transparency; }
			void make_transparent() { m_transparent = true; }
			void make_opaque() { m_transparent = false; }

			// traversability accessors

			bool traversable() const { return m_traversable.test(0); }
			bool traversable(unsigned int layer) const { return m_traversable.test(layer); }
			bool traversable(bitset l) const { return (m_traversable & l).any(); }
			template <typename _E>
			bool traversable(_E enum_layer) const { return traversable((unsigned int)enum_layer); }

			void make_traversable(unsigned int layer, bool val)	{ m_traversable.set(layer, val); }
			void make_traversable(unsigned int layer) { m_traversable.set(layer); }
			void make_traversable(bool val)	{ m_traversable.set(0, val); }
			void make_traversable() { m_traversable.set(0); }
			void make_traversable(bitset layers) { m_traversable |= layers; }
			void make_blocking(unsigned int layer) { m_traversable.reset(layer); }
			void make_blockingb(bitset layers) { m_traversable &= layers; }
			void make_traversable(bitset l, bool val)
			{
				if (val)
				{
					m_traversable |= l;
				}
				else
				{
					m_traversable &= ~l;
				}
			}
			template <typename _E>
			void make_traversable(_E enum_layer) { m_traversable.set((unsigned int)enum_layer); }
			template <typename _E>
			void make_traversable(_E enum_layer, bool val) { traversable((unsigned int)enum_layer, val); }
			template <typename _E>
			void make_blocking(_E enum_layer) { m_traversable.reset((unsigned int)enum_layer); }


			// utility
			void make_wall() { traversable(false); transparent(false); }
			void make_ground() { traversable(true); transparent(true); }
		};
	}
}

#endif