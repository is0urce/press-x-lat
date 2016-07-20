// name: map_stream.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_RL_MAP_STREAM_HPP
#define PX_RL_MAP_STREAM_HPP

#include <px/common/matrix.hpp>

#include "tile.hpp"

#include <thread>
#include <list>
#include <chrono>

namespace px
{
	namespace rl
	{
		template <typename _TP, typename _Unit, unsigned int _W, unsigned int _H>
		class map_stream
		{
		public:
			typedef tile<_TP> tile;
			typedef matrix2<tile, _W, _H> map;
			typedef std::list<_Unit> unit_list;

		private:
			map m_map;
			unit_list m_units;
			volatile bool m_loaded;
			bool m_pending;

		public:
			map_stream() : m_loaded(true), m_pending(false)
			{
			}
			virtual ~map_stream()
			{
				wait();
			}
			map_stream(const map_stream&) = delete;


		public:
			void wait() const
			{
				while (!m_loaded)
				{
					std::this_thread::yield();
				}
			}
			bool loaded() const
			{
				return m_loaded;
			}

			// is there units for merge
			bool pending() const
			{
				return m_pending;
			}

			template <typename _Op>
			void load(_Op fn)
			{
				wait();
				m_loaded = false;
				m_pending = true;
				fn(m_map, m_units);
				m_loaded = true;
			}
			template <typename _Op>
			void load_stream(_Op fn)
			{
				wait();
				std::thread stream([fn, this]()
				{
					load(fn);
				});
				stream.detach();
			}
			void merge(unit_list &grand)
			{
				wait();
				if (!m_pending) throw std::runtime_error("px::rl::map_stream::merge() - no pending");

				m_pending = false;
				grand.merge(m_units);
			}

			const map* operator->() const
			{
				return &m_map;
			}
		};
	}
}

#endif