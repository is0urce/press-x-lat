// name: ingame.hpp
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_UI_SKILL_PANEL_HPP
#define PX_CORE_UI_SKILL_PANEL_HPP

#include <px/ui/stack_panel.hpp>
#include <px/shell/key.hpp>

#include <px/ui/button.hpp>

namespace px
{
	namespace core
	{
		class environment;
		class skill_panel : public ui::stack_panel
		{
		public:
			virtual ~skill_panel();
			skill_panel(environment &e);

		protected:
			bool key_stacked(shell::key code) override;
			bool hover_stacked(point2 const& position) override;
			void draw_stacked(shell::canvas &cnv) const override;

		private:
			void update();

		private:
			environment* m_environment;
			size_t m_slots;
			std::vector<std::weak_ptr<ui::button>> m_skills;
		};
	}
}

#endif