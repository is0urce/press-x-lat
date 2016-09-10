// name: map_panel.hpp
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_UI_MAP_PANEL_HPP
#define PX_CORE_UI_MAP_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board.hpp>
#include <px/ui/text.hpp>

#include <px/core/gen/world.hpp>
#include <px/core/gen/landmark.hpp>

#include <px/shell/key.hpp>

#include <memory>

namespace px
{
	namespace core
	{
		class map_panel : public ui::stack_panel
		{
		public:
			void bind(world &w)
			{
				m_world = &w;

			}
			void tear()
			{
				m_world = nullptr;
			}

		public:
			map_panel()	{}
			virtual ~map_panel() {}

		protected:
			virtual void draw_stacked(shell::canvas& cnv) const override
			{
				if (m_world)
				{
					if (auto m = m_world->map())
					{
						int w = static_cast<int>(m->width());
						int h = static_cast<int>(m->height());

						point2 center = bounds().start() + bounds().range() / 2;
						point2 start = center.moved(point2(-w / 2, -h / 2));
						rectangle map = { start, {w, h} };
						point2 pen = start.moved({ 0, h - 1 });

						cnv.rectangle(map.inflated(1), { 1, 1, 1 });
						m->enumerate([&](auto const& location, auto &cell) {
							point2 pos = pen + point2(location.x(), -location.y());

							cnv.write(pos, cell.img.glyph, cell.img.tint);
							cnv.pset(pos, cell.img.bg);
						});

						point2 hover_location = m_hover - pen;
						hover_location.mirror<1>();
						if (m->contains(hover_location))
						{
							const auto &hover_cell = m->at(hover_location);
							cnv.write(start, to_string(hover_cell.location), color(1, 1, 0));
							if (hover_cell.landmark)
							{
								cnv.write(start, hover_cell.landmark->name(), color(1, 1, 0));
							}
						}
					}
				}
			}
			virtual bool hover_stacked(const point2 &position) override
			{
				m_hover = position;
				return false;
			}
			virtual bool key_stacked(shell::key code) override
			{
				bool close = code == shell::key::command_cancel || code == shell::key::move_none;
				if (close)
				{
					deactivate();
				}
				return close;
			}
			virtual bool click_stacked(const point2 &position, unsigned int button) override
			{
				bool close = !bounds().contains(position);
				if (close)
				{
					deactivate();
				}
				return close;
			}

		private:
			world* m_world;
			point2 m_hover;
		};
	}
}

#endif