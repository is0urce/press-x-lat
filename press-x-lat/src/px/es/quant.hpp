// name: quant.hpp
// type: c++ header
// desc: struct
// auth: is0urce

#pragma once

// struct for holding various time deltas
// ratio is speed of the game, paused on 0, bullet time at (0, 1), normal game at 1.0 and fast-forward if greater
// quarks is helper struct
// - elapsed - total elapsed time from start
// - delta - time from last update
// real for real-time deltas
// simulation has deltas for ingame world timers (i.e. modified with ratio)
// fixed is for fixed uptates, elapsed time in fixed update not represent "time" of update should have be, but an actual time

namespace px
{
	namespace es
	{
		struct quant
		{
			struct quark
			{
				double elapsed;
				double delta;
			} real, simulation, fixed;
			double ratio;

			bool stopped() const noexcept
			{
				return ratio == 0;
			}
		};
	}
}