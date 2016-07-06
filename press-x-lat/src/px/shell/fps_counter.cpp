// name: fps_counter.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "fps_counter.h"

#include <px/common/timer.hpp>

namespace
{
	const float fps_interval = 0.5f;
}

namespace px
{
	namespace shell
	{
		fps_counter::fps_counter() : m_frames(0), m_fps(0)
		{
			m_performance = std::make_unique<timer>();
		}

		fps_counter::~fps_counter()
		{
		}

		fps_counter::precision_t fps_counter::fps() const
		{
			return m_fps;
		}

		void fps_counter::frame_processed()
		{
			++m_frames;
			auto time = m_performance->measure();
			if (time > fps_interval)
			{
				// calculate fps
				m_fps = m_frames / time;

				// restart counters
				m_frames = 0;
				m_performance->restart();
			}
		}
	}
}

