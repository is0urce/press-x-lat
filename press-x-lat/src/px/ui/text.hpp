// name: text.hpp
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_TEXT_HPP
#define PX_UI_TEXT_HPP

#include <px/ui/stack_panel.hpp>
#include <px/ui/text_alignment.hpp>

#include <px/common/color.hpp>
#include <functional>
#include <string>

namespace px
{
	namespace ui
	{
		class text : public stack_panel
		{
		public:
			typedef std::function<std::string()> string_fn;

		public:
			void align(text_alignment align)
			{
				m_align = align;
			}
			void set_color(color c)
			{
				m_color = c;
			}
			void set_text(string_fn text, color c)
			{
				m_text = text;
				m_color = c;
			}
			void set_text(string_fn text)
			{
				m_text = text;
			}
			void set_text(std::string str, color c)
			{
				m_text = [str]() { return str; };
				m_color = c;
			}
			void set_text(std::string str)
			{
				m_text = [str]() { return str; };
			}
			void set_text(const char* c_str, color c)
			{
				m_text = [str = std::string(c_str)]() { return str; };
				m_color = c;
			}
			void set_text(const char* c_str)
			{
				m_text = [str = std::string(c_str)]() { return str; };
			}

		public:
			text(string_fn fn, color c) : m_color(c), m_text(fn), m_align(text_alignment::left)
			{
				if (!fn) throw std::runtime_error("px::ui::text - string function is null");
			}
			text(std::string str, color c)
				: text([str]() { return str; }, c)
			{
			}
			text(const char* c_str, color c)
				: text([str = std::string(c_str)]() { return str; }, c)
			{
			}
			virtual ~text() {}

		protected:
			virtual void draw_stacked(shell::canvas& cnv) const override
			{
				auto str = m_text();

				point2 pen = start();
				switch (m_align)
				{
				case text_alignment::right:
					pen.move_axis<0>(bounds().range().x() - static_cast<int>(str.length()));
					break;
				case text_alignment::center:
					pen.move_axis<0>((bounds().range().x() - static_cast<int>(str.length())) / 2);
					break;
				case text_alignment::left:
					break;
				}
				cnv.write(pen, str, m_color);
			}

		private:
			string_fn m_text;
			color m_color;
			text_alignment m_align;
		};
	}
}

#endif