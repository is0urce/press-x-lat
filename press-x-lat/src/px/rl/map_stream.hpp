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
			std::thread m_thread;

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
			void wait()
			{
				if (m_thread.joinable())
				{
					m_thread.join();
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
				if (m_pending) throw std::runtime_error("px::rl::map_stream::load() - pending");

				m_pending = true;
				fn(m_map, m_units);
			}
			template <typename _Op>
			void load_stream(_Op fn)
			{
				if (m_pending) throw std::runtime_error("px::rl::map_stream::load_stream() - pending");

				m_loaded = false;
				m_pending = true;
				m_thread = std::thread([fn, this]()
				{
					fn(m_map, m_units);
					m_loaded = true;
				});
			}
			void merge(unit_list &grand)
			{
				if (!m_pending) throw std::runtime_error("px::rl::map_stream::merge() - no pending");

				wait();

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