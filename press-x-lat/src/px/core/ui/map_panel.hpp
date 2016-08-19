// name: map_panel.hpp
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_UI_MAP_PANEL_HPP
#define PX_CORE_UI_MAP_PANEL_HPP

#include <px/ui/stack_panel.hpp>

#include <px/ui/board_panel.hpp>
#include <px/ui/static_text_panel.hpp>

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
			map_panel()
			{
				//emplace<ui::board_panel>({ { 0, 0 },{ 0, 0 },{ 0, 1 },{ 1, 0 } }, color(0.3, 0.3, 0.3));
				//emplace<ui::board_panel>({ { 0, 0 },{ 0, 1 },{ 0, -1 },{ 1, 1 } }, color(0.8, 0.8, 0.8));
				//emplace<ui::static_text_panel>({ { 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 } }, "[MAP]", color(1, 1, 1));
			}
			virtual ~map_panel() {}

		protected:
			virtual void draw_panel(shell::canvas& cnv) const override
			{
				stack_panel::draw_panel(cnv);

				if (m_world)
				{
					auto m = m_world->map();
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
			virtual bool hover_control(const point2 &position) override
			{
				m_hover = position;
				return stack_panel::hover_control(position);
			}
			virtual bool key_control(shell::key code) override
			{
				bool result = stack_panel::key_control(code);

				if (!result)
				{
					bool result = code == shell::key::command_cancel || code == shell::key::move_none;
					if (result)
					{
						deactivate();
					}
					else
					{
						m_world->generate(std::rand());
					}
				}
				return result;
			}
			virtual bool click_control(const point2 &position, unsigned int button) override
			{
				bool result = stack_panel::click_control(position, button);

				return result;
			}

		private:
			world* m_world;
			point2 m_hover;
		};
	}
}

#endif