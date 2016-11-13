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

			double fixed() const noexcept
			{
				return m_quant.fixed.delta;
			}
			void fixed(double span) noexcept
			{
				m_quant.fixed.delta = span;
			}
			double dilation() const noexcept
			{
				return m_ratio;
			}
			void dilation(double ratio) noexcept
			{
				m_real_elapsed = m_quant.real.elapsed;
				m_simulation_elapsed = m_quant.simulation.elapsed;

				m_quant.real.elapsed = 0;
				m_quant.simulation.elapsed = 0;
				m_start = m_last;

				m_ratio = ratio;
			}
			double simulation_time() const noexcept
			{
				return m_simulation_elapsed;
			}
			void simulation_time(double elapsed) noexcept
			{
				m_simulation_elapsed = elapsed - m_quant.simulation.elapsed;
			}
			double session_time() const noexcept
			{
				return m_real_elapsed;
			}
			void session_time(double elapsed) noexcept
			{
				m_real_elapsed = elapsed - m_quant.real.elapsed;
			}
			quant const& time() const noexcept
			{
				return m_quant;
			}

			void frame()
			{
				// update time quant
				auto now = std::chrono::high_resolution_clock::now();
				auto delta = std::chrono::duration<quant::resolution>(now - m_last).count();
				auto elapsed = std::chrono::duration<quant::resolution>(now - m_start).count();
				auto fixed_delta = std::chrono::duration<quant::resolution>(now - m_last_fixed).count();
				m_last = now;

				m_quant.real.elapsed = m_real_elapsed + elapsed;
				m_quant.real.delta = delta;

				m_quant.simulation.elapsed = m_simulation_elapsed + elapsed * m_ratio;
				m_quant.simulation.delta = delta * m_ratio;

				m_quant.fixed.elapsed = m_quant.simulation.elapsed;

				// run systems (fixed first)
				if (fixed_delta >= m_fixed)
				{
					for (int i = 1; fixed_delta - i * m_fixed >= 0; ++i)
					{
						for (auto & system : m_systems)
						{
							system->pre_update(m_quant);
						}
					}
					m_last_fixed = m_last;
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
			basic_engine()
				: m_quant{ {},{},{}, 1.0 }
				, m_ratio(1)
				, m_real_elapsed(0)
				, m_simulation_elapsed(0)
				, m_fixed(0.050) // 20 fps
			{
				m_start = std::chrono::high_resolution_clock::now();
				m_last = m_start;
				m_last_fixed = m_start;

				m_quant.fixed.delta = m_fixed;
			}

		private:
			std::vector<basic_system*> m_systems;
			quant m_quant;
			quant::resolution m_real_elapsed;
			quant::resolution m_simulation_elapsed;
			quant::resolution m_ratio;
			quant::resolution m_fixed;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_last;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_last_fixed;
		};
	}
}