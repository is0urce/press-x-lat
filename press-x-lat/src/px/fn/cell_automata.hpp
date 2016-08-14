// name: cell_automata.hpp
// type: c++ header
// desc: template class for cellular automation
// auth: is0urce

// template for cellular automata based on matrix2
// C - cell / element type

#ifndef PX_FN_CELL_AUTOMATA_HPP
#define PX_FN_CELL_AUTOMATA_HPP

#include <px/common/matrix.hpp>

#include <memory>

// A acc type
// F fold functor
// S select functor

namespace px
{
	namespace fn
	{
		template<typename _C, size_t W, size_t H>
		class cell_automata
		{
		public:
			typedef matrix2<_C, W, H> map;
		private:
			std::unique_ptr<map> m_map;
		public:
			cell_automata() : m_map(std::make_unique<map>()) {}
			cell_automata(const _C &initial) : m_map(std::make_unique<map>(initial)) {}
			template <typename Op>
			cell_automata(Op fn) : m_map(std::make_unique<map>(fn)) {}

		private:
			// clip out-of-range access to bounds
			const _C& sample(const point2 &position) const
			{
				return m_map->at(position.clamped({ 0, 0 }, { W - 1, H - 1 }));
			}

		public:
			// run generator for several generations
			template<typename A, typename F, typename S>
			void mutate(F fold, A start, S select, unsigned int generations)
			{
				for (unsigned int i = 0; i < generations; ++i)
				{
					next_generation(fold, start, select);
				}
			}

			// run generator once
			template<typename A, typename F, typename S>
			void next_generation(F fold, A start, S select)
			{
				// generate and swap
				m_map = std::make_unique<map>([&](unsigned int i, unsigned int j)
				{
					point2 pos(i, j);
					A acc = start;
					acc = fold(acc, sample(pos));
					acc = fold(acc, sample(pos.moved({ 0, 1 })));
					acc = fold(acc, sample(pos.moved({ 0, -1 })));
					acc = fold(acc, sample(pos.moved({ -1, 0 })));
					acc = fold(acc, sample(pos.moved({ 1, 0 })));
					acc = fold(acc, sample(pos.moved({ 1, 1 })));
					acc = fold(acc, sample(pos.moved({ -1, -1 })));
					acc = fold(acc, sample(pos.moved({ 1, -1 })));
					acc = fold(acc, sample(pos.moved({ -1, 1 })));
					return select(acc);
				});
			}

			// array access
			const map* operator->()
			{
				return m_map.get();
			}
		};
	}
}

#endif