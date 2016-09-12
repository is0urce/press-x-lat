// name: list_panel.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_LIST_PANEL_HPP
#define PX_UI_LIST_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/common/color.hpp>

#include <memory>
#include <functional>

namespace px
{
	namespace ui
	{
		namespace
		{
			struct nop_callback
			{
				template <typename E>
				bool operator()(E const&) const
				{
					return true;
				}
			};
			struct white_color_provider
			{
				template <typename E>
				color operator()(E const&) const
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
		template <typename List, typename Formatter, typename Color = white_color_provider, typename Click = nop_callback, typename Hover = nop_callback, typename Filter = permissive_filter>
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
			list_panel(const Formatter &formatter = Formatter(), const Color& c = Color(), const Click &click = Click(), const Hover &hover = Hover(), const Filter& filter = Filter())
				: m_formatter(formatter), m_color(c), m_filter(filter)
				, m_hover(hover), m_click(click)
				, m_line_size(1)
			{
				reset();
			}
			virtual ~list_panel() {}

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

					std::function<void()> click_call; // erasure of generic element type, stored for future call for safe iteration

					shared->enumerate([&](auto const& element) {
						if (m_filter(element))
						{
							if (rectangle(pen, record_range).contains(position))
							{
								click_call = [element, this]() { m_click(element); };
								result = true;
							}

							pen.move_axis<1>(m_line_size);
							++item_index;
						}
					});

					if (click_call)
					{
						click_call();
						result = true;
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
			bool scrollable_up() const noexcept
			{
				return m_skip > 0;
			}
			bool scrollable_down(unsigned int lines, unsigned int count) const
			{
				return m_skip + lines < count;
			}
			void reset() noexcept
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
			void set_filter(Filter filter)
			{
				m_filter = filter;
			}
			void set_format(Formatter format)
			{
				m_formatter = format;
			}
			std::shared_ptr<list_type> lock_data()
			{
				return m_list.lock();
			}
			void set_click(Click fn)
			{
				m_click = fn;
			}
			void set_hover(Hover fn)
			{
				m_hover = fn;
			}
		};
	}
}

#endif