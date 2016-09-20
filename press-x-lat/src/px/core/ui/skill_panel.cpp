// name: skill_panel.cpp
// type: c++
// desc: class functions
// auth: is0urce

#include "skill_panel.hpp"

#include <px/core/environment.hpp>

#include <px/core/sys/location_component.hpp>
#include <px/core/sys/body_component.hpp>
#include <px/core/sys/character_component.hpp>

namespace px
{
	namespace core
	{
		namespace
		{
			static const int slot_width = 9;
			static const int slot_gap = 1;
		}
		skill_panel::skill_panel(core::environment& e) : m_environment(&e)
		{
			update();
		}
		skill_panel::~skill_panel()
		{
		}

		bool skill_panel::key_stacked(shell::key code)
		{
			update();
			return false;
		}
		bool skill_panel::hover_stacked(point2 const&)
		{
			update();
			return false;
		}

		void skill_panel::draw_stacked(shell::canvas& cnv) const
		{
		}

		void skill_panel::update()
		{
			if (auto player = m_environment->player())
			{
				if (body_component* body = *player)
				{
					if (character_component* character = *body)
					{
						//auto total = character->skils_total();
						//while (m_skills.size() < total)
						//{
						//	int index = m_skills.size();
						//	auto button = emplace<ui::button>({ {0.0, 0.0}, { index * (slot_width + slot_gap), 0}, { slot_width, 1} });

						//	button->set_color({ 1, 1, 0});
						//	button->text()->set_text("skill#", 0x000000);

						//	m_skills.push_back(button);
						//}

						clear();
						m_skills.clear();
						for (size_t index = 1, total = character->skils_total(); index < total; ++index)
						{
							if (auto skill = character->skill(index))
							{
								auto button = emplace<ui::button>({ { 0.0, 0.0 },{ static_cast<int>(index - 1) * (slot_width + slot_gap), 0 },{ slot_width, 1 } });
								button->text()->set_text([str = std::to_string(index) + " " + skill->alias()]() { return str; }, 0x000000);
								button->set_color({ 1, 1, 0 });
							}
						}
					}
				}
			}
		}
	}
}