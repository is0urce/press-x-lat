// name: list_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_LIST_PANEL_HPP
#define PX_UI_LIST_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/common/color.hpp>

#include <string>
#include <memory>

namespace px
{
	namespace ui
	{
		namespace
		{
			struct nop_callback
			{
				template <typename E>
				bool operator()(const E&) const
				{
					return true;
				}
			};
			struct white_color_provider
			{
				template <typename E>
				color operator()(const E&) const
				{
					return color(1, 1, 1);
				}
			};
			struct permissive_filter
			{
				template <typename E>
				bool operator()(const E&) const
				{
					return true;
				}
			};
		}
		template <typename List, typename Formatter, typename Color = white_color_provider, typename Filter = permissive_filter, typename Hover = nop_callback, typename Click = nop_callback>
		class list_panel : public stack_panel
		{
		public:
			typedef List list_type;
			typedef std::weak_ptr<list_type> list_ptr;
		private:
			std::weak_ptr<list_type> m_list;
			Formatter m_formatter;
			Color m_color;
			Filter m_filter;
			Hover m_hover;
			Click m_click;
			unsigned int m_line_size;
			int m_skip;
			int m_counted;
			int m_drawn;

		public:
			list_panel(const Formatter &formatter = Formatter(), const Color& c = Color(), const Filter& filter = Filter(), const Hover &hover = Hover(), const Click &click = Click())
				: m_formatter(formatter), m_color(c)
				, m_hover(hover), m_click(click)
				, m_line_size(1)
			{
				reset();
			}
			virtual ~list_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv); // call base

				auto total_bounds = bounds();
				int text_width = total_bounds.range().x();
				auto shared = m_list.lock();
				if (shared && text_width > 0)
				{
					point2 pen = total_bounds.start();

					pen.move_axis<1>(-m_skip * m_line_size);

					shared->enumerate([&](const auto &element) {
						if (m_filter(element))
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
			virtual bool scroll_control(int scroll) override
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

		private:
			unsigned int count() const
			{
				unsigned int result = 0;
				auto shared = m_list.lock();
				if (shared)
				{
					shared->enumerate([&](const auto &element) {
						if (m_filter(element))
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
			bool scrollable_up() const
			{
				return m_skip > 0;
			}
			bool scrollable_down(unsigned int lines, unsigned int count) const
			{
				return m_skip + lines < count;
			}
			void reset()
			{
				m_skip = 0;
				m_counted = 0;
				m_drawn = 0;
			}

		public:
			void bind_data(list_ptr list)
			{
				m_list = list;
				reset();
			}
		};
	}
}

#endif