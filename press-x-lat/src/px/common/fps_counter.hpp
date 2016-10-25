// name: fps_counter.h
// type: c++ header
// desc: class declaration

// class dedicated to count fps

#ifndef PX_FPS_COUNTER_HPP
#define PX_FPS_COUNTER_HPP

#include <px/common/timer.hpp>

namespace px
{
	class fps_counter
	{
	public:
		typedef float precision_t;

	public:
		const float fps_interval = 0.2f;

	public:
		fps_counter()
			: m_frames(0), m_fps(0)
		{
		}
		~fps_counter()
		{
		}

	public:
		precision_t fps() const
		{
			return m_fps;
		}
		void frame_processed()
		{
			++m_frames;
			auto time = m_performance.measure();
			if (time > fps_interval)
			{
				// calculate fps
				m_fps = m_frames / time;

				// restart counters
				m_frames = 0;
				m_performance.restart();
			}
		}

	private:
		timer m_performance;
		unsigned int m_frames;
		precision_t m_delay;
		precision_t m_fps;
	};
}

#endif