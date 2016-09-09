// name: progress_bar.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_PROGRESS_BAR_HPP
#define PX_UI_PROGRESS_BAR_HPP

#include <px/ui/stack_panel.hpp>

#include <px/common/color.hpp>

#include <functional>
#include <tuple>

namespace px
{
	namespace ui
	{
		class progress_bar : public stack_panel
		{
		public:
			struct progress
			{
			public:
				int current;
				int maximum;
				progress() = default;
				progress(int val, int max) : current(val), maximum(max) {}
			};
			typedef std::function<progress()> progress_fn;

		public:
			void set_empty_color(color c) noexcept
			{
				m_empty_color = c;
			}
			void set_fill_color(color c) noexcept
			{
				m_fill_color = c;
			}
			void set_progress_function(progress_fn fn)
			{
				if (!fn) throw std::runtime_error("px::ui::progress_bar::set_progress_function(fn) fn is null");

				m_progress = fn;
			}

		public:
			progress_bar(color empty, color fill, progress_fn fn) : m_empty_color(empty), m_fill_color(fill)
			{
				set_progress_function(fn);
			}
			virtual ~progress_bar() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				int length = bounds().range().x();

				auto progress = m_progress();

				int current = progress.current;
				int max = progress.maximum;

				max = (max == 0) ? 1 : max;

				int fill = length * current / max;
				fill = (fill == 0 && current > 0) ? 1 : fill;
				fill = (fill < 0) ? 0 : fill;

				cnv.rectangle({ bounds().start(), { fill, bounds().range().y() } }, m_empty_color);
				cnv.rectangle({ bounds().start().moved_axis<0>(fill), { bounds().range().moved_axis<0>(-fill) } }, m_fill_color);
			}

		private:
			progress_fn m_progress;
			color m_empty_color;
			color m_fill_color;
		};
	}
}

#endif