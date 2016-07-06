// name: cell_automata.h
// type: c++ header
// desc: template class for cellular automation
// auth: is0urce

// template for cellular automata based on matrix2
// _C - cell / element type

#ifndef PX_FN_CELL_AUTOMATA_HPP
#define PX_FN_CELL_AUTOMATA_HPP

#include <px/common/matrix.hpp>

#include <functional>
#include <memory>

// _A acc type
// _F fold functor
// _S select functor

namespace px
{
	namespace fn
	{
		template<typename _C, unsigned int _W, unsigned int _H>
		class cell_automata
		{
		public:
			typedef matrix2<_C, _W, _H> map;
		private:
			std::unique_ptr<map> m_map;
		public:
			cell_automata() : m_map(std::make_unique<map>()) {}
			cell_automata(const _C &initial) : m_map(std::make_unique<map>(initial)) {}
			template <typename _Op>
			cell_automata(_Op fn) : m_map(std::make_unique<map>(fn)) {}
			~cell_automata() {}

		private:
			// clip out-of-range access to bounds
			const _C& sample(const point2 &position) const
			{
				return m_map->at(position.clamped({ 0, 0 }, { _W - 1, _H - 1 }));
			}

		public:
			// run generator for several generations
			template<typename _A, typename _F, typename _S>
			void mutate(_F fold, _A start, _S select, unsigned int generations)
			{
				for (unsigned int i = 0; i < generations; ++i)
				{
					next_generation(fold, start, select);
				}
			}

			// run generator once
			template<typename _A, typename _F, typename _S>
			void next_generation(_F fold, _A start, _S select)
			{
				m_map = std::make_unique<map>
					([&](unsigned int i, unsigned int j)
				{
					point2 pos(i, j);
					_A acc = start;
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