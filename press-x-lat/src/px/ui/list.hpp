// name: list.hpp
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_UI_LIST_HPP
#define PX_UI_LIST_HPP

#include <px/ui/stack_panel.hpp>

#include <px/common/color.hpp>

#include <memory>
#include <string>
#include <functional>

namespace px
{
	namespace ui
	{
		template <typename List, typename Item>
		class list : public stack_panel
		{
		public:
			typedef List list_type;
			typedef Item item_type;

			typedef std::function<std::string(item_type const&)> formatter_fn;
			typedef std::function<color(item_type const&)> color_fn;
			typedef std::function<bool(item_type const&)> filter_fn;
			typedef std::function<void(item_type&)> click_fn;
			typedef std::function<void(item_type&)> hover_fn;

			typedef std::weak_ptr<list_type> list_ptr;

		public:
			list()
				: m_line_size(1)
				, m_color([](auto const& /*item*/) { return color(1.0, 1.0, 1.0); })
				, m_formatter([](auto const& /*item*/) { return ""; })
			{
				reset();
			}
			virtual ~list() {}

		public:
			void bind_data(list_ptr list)
			{
				m_list = list;
				reset();
			}
			void set_filter(filter_fn filter)
			{
				m_filter = filter;
			}
			void set_format(formatter_fn format)
			{
				m_formatter = format;
			}
			void set_color(color_fn color_selector)
			{
				m_color = color_selector;
			}
			void set_color(color color_all)
			{
				m_color = [color_all](auto const& /*item*/) -> color { return color_all; };
			}
			void set_click(click_fn fn)
			{
				m_click = fn;
			}
			void set_hover(hover_fn fn)
			{
				m_hover = fn;
			}

		protected:
			virtual void draw_stacked(shell::canvas& cnv) const override
			{
				auto total_bounds = bounds();
				int text_width = total_bounds.range().x();
				auto shared = m_list.lock();
				if (shared && text_width > 0)
				{
					point2 pen = total_bounds.start();

					pen.move_axis<1>(-m_skip * m_line_size);

					shared->enumerate([&](auto const& element) {
						if (valid(element))
						{
							if (total_bounds.contains(pen))
							{
								cnv.write_n(pen, m_formatter(element), m_color(element), text_width);
							}
							pen.move_axis<1>(m_line_size);
						}
					});
				}
			}
			virtual bool scroll_stacked(int scroll) override
			{
				if (scroll > 0 && scrollable_up())
				{
					--m_skip;
				}
				else if (scroll < 0 && scrollable_down(lines(), count()))
				{
					++m_skip;
				}
				return true;
			}
			virtual bool click_stacked(point2 const& position, unsigned int) override
			{
				auto shared = m_list.lock();
				auto total_bounds = bounds();

				bool result = false;

				if (total_bounds.contains(position) && shared)
				{
					point2 pen = total_bounds.start();
					point2 record_range(total_bounds.range().x(), m_line_size);

					unsigned int item_index = m_skip;
					pen.move_axis<1>(-m_skip * m_line_size);

					item_type * find;
					shared->enumerate([&](auto element) {
						if (valid(element))
						{
							if (rectangle(pen, record_range).contains(position))
							{
								find = &element;
								result = true;
							}

							pen.move_axis<1>(m_line_size);
							++item_index;
						}
					});

					if (find && m_click)
					{
						m_click(*find);
					}
				}
				return result;
			}

		private:
			unsigned int count() const
			{
				unsigned int result = 0;
				auto shared = m_list.lock();
				if (shared)
				{
					shared->enumerate([&](auto const& element) {
						if (valid(element))
						{
							++result;
						}
					});
				}
				return result;
			}
			unsigned int lines() const
			{
				return bounds().range().y() / m_line_size;
			}
			bool scrollable_up() const noexcept
			{
				return m_skip > 0;
			}
			bool scrollable_down(unsigned int lines, unsigned int count) const noexcept
			{
				return m_skip + lines < count;
			}
			void reset() noexcept
			{
				m_skip = 0;
				m_counted = 0;
				m_drawn = 0;
			}

			bool valid(item_type const& element) const
			{
				return !m_filter || m_filter && m_filter(element);
			}

		private:
			std::weak_ptr<list_type> m_list;
			formatter_fn m_formatter;
			color_fn m_color;
			filter_fn m_filter;
			hover_fn m_hover;
			click_fn m_click;

			unsigned int m_line_size;
			int m_skip;
			int m_counted;
			int m_drawn;
		};
	}
}

#endif