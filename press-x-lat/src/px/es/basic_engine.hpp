// name: basic_engine.hpp
// type: c++ header
// desc: base class
// auth: is0urce

#pragma once

// base class for engine in component / enetity system composition
// engine composes various system in one logical pipeline and executes it's updates on timer on frame() calls

#include "basic_system.hpp"
#include "quant.hpp"

#include <algorithm>
#include <chrono>
#include <vector>

namespace px
{
	namespace es
	{
		class basic_engine
		{
		public:
			typedef quant::resolution time_type;

		public:
			void add(basic_system* system) noexcept
			{
				m_systems.push_back(system);
			}
			void remove(basic_system* system) noexcept
			{
				m_systems.erase(std::remove(std::begin(m_systems), std::end(m_systems), system));
			}
			void clear() noexcept
			{
				m_systems.clear();
			}

			time_type fixed() const noexcept
			{
				return m_quant.fixed.delta;
			}
			void fixed(time_type span) noexcept
			{
				m_quant.fixed.delta = span;
			}
			time_type dilation() const noexcept
			{
				return m_quant.ratio;
			}
			void dilation(time_type ratio) noexcept
			{
				m_real_elapsed = m_quant.real.elapsed;
				m_simulation_elapsed = m_quant.simulation.elapsed;

				m_quant.real.elapsed = 0;
				m_quant.simulation.elapsed = 0;
				m_start = m_last;

				m_quant.ratio = ratio;
			}
			time_type simulation_time() const noexcept
			{
				return m_simulation_elapsed;
			}
			void simulation_time(time_type elapsed) noexcept
			{
				m_simulation_elapsed = elapsed - m_quant.simulation.elapsed;
			}
			time_type session_time() const noexcept
			{
				return m_real_elapsed;
			}
			void session_time(time_type elapsed) noexcept
			{
				m_real_elapsed = elapsed - m_quant.real.elapsed;
			}
			quant const& time() const noexcept
			{
				return m_quant;
			}

			// set starting time to current session time
			void start(time_type simulation_elapsed, time_type session_elapsed)	noexcept
			{
				m_simulation_elapsed = simulation_elapsed;
				m_real_elapsed = session_elapsed;

				m_start = std::chrono::high_resolution_clock::now();
				m_last = m_start;
				m_last_fixed = m_start;
			}

			void frame()
			{
				// update time quant
				auto now = std::chrono::high_resolution_clock::now();
				auto delta = std::chrono::duration<time_type>(now - m_last).count();
				auto elapsed = std::chrono::duration<time_type>(now - m_start).count();
				auto fixed_delta = std::chrono::duration<time_type>(now - m_last_fixed).count();
				m_last = now;

				m_quant.real.elapsed = m_real_elapsed + elapsed;
				m_quant.real.delta = delta;

				m_quant.simulation.elapsed = m_simulation_elapsed + elapsed * m_quant.ratio;
				m_quant.simulation.delta = delta * m_quant.ratio;

				m_quant.fixed.elapsed = m_quant.simulation.elapsed;

				// run systems (fixed first)
				for (int i = 1; fixed_delta - i * m_quant.fixed.delta >= 0; ++i)
				{
					for (auto & system : m_systems)
					{
						system->pre_update(m_quant);
					}
					m_last_fixed = now; // don't care to optimize this
				}

				// run realtime systems respecting pre/post order
				for (auto & system : m_systems)
				{
					system->pre_update(m_quant);
				}
				for (auto & system : m_systems)
				{
					system->update(m_quant);
				}
				for (auto & system : m_systems)
				{
					system->post_update(m_quant);
				}
			}

		public:
			basic_engine() noexcept
			{
				m_quant.fixed.delta = 0.050; // 20 fps
				m_quant.ratio = 1;
				start(0, 0);
			}

		private:
			std::vector<basic_system*> m_systems;
			quant m_quant;
			time_type m_real_elapsed;
			time_type m_simulation_elapsed;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_last;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_last_fixed;
		};
	}
}