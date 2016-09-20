// name: character_system.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_CORE_CHARACTER_SYSTEM_HPP
#define PX_CORE_CHARACTER_SYSTEM_HPP

#include <px/es/i_system.hpp>
#include <px/es/manager.hpp>

#include "character_component.hpp"

#include <px/rl/skill.hpp>
#include <px/rl/skill_book.hpp>

#include <selene.h>

namespace px
{
	namespace core
	{
		class location_component;
		class environment;

		class character_system
			: public es::i_system
			, public es::manager<character_component, 10000>
		{
		public:
			typedef typename character_component::skillbook_type book_type;

		public:
			book_type* book();

		public:
			virtual ~character_system();
			character_system(environment &e);
			character_system(character_system const&) = delete;
			character_system& operator=(character_system const&) = delete;

		protected:
			virtual void element_allocated(character_component &l) override;
			virtual void element_released(character_component &l) override;
			virtual void fixed_update_system() override;

		private:
			void setup_scripts(environment &e);
			void fill(environment &e);

		private:
			book_type m_book;
			sel::State m_script;
		};
	}
}

#endif