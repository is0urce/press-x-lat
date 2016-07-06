// name: fps_counter.h
// type: c++ header
// desc: class declaration

// class dedicated to count fps

#ifndef PX_SHELL_FPS_COUNTER
#define PX_SHELL_FPS_COUNTER

#include <memory>

namespace px
{
	class timer;
	namespace shell
	{
		class fps_counter
		{
		public:
			typedef float precision_t;

		private:
			std::unique_ptr<timer> m_performance;
			unsigned int m_frames;
			precision_t m_delay;
			precision_t m_fps;

		public:
			fps_counter();
			~fps_counter();

		public:
			precision_t fps() const;
			void frame_processed();
		};
	}
}

#endif