// name: landmark.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_LANDMARK_HPP
#define PX_CORE_LANDMARK_HPP

#include <px/common/matrix.hpp>

#include <px/rl/entity.hpp>
#include <px/rl/tile.hpp>

#include <px/core/image.hpp>
#include <px/core/settings.hpp>

#include <list>
#include <memory>

namespace px
{
	namespace core
	{
		class builder;
		class mapper;
		struct unit_record;

		class landmark : public rl::entity
		{
		public:
			typedef matrix2<rl::tile<image>, settings::cell_width, settings::cell_height> map_type;

		public:
			void generate(unsigned int seed, map_type & map, std::list<unit_record> & units, bool generate_placeables) const;
			void set_pipeline(std::unique_ptr<builder> && b, std::unique_ptr<mapper> && m)
			{
				std::swap(m_builder, b);
				std::swap(m_mapper, m);
			}

		public:
			image appearance;

		public:
			landmark();
			~landmark();

		private:
			std::unique_ptr<builder> m_builder;
			std::unique_ptr<mapper> m_mapper;
		};
	}
}

#endif