// name: perlin.hpp
// auth: is0urce
// type: c++ template class
// desc: perlin noise

// 2d perilin noise with key values (gradients) at integer positions
// supports repeat tiling and sampling with fractal octaves

#include <px/common/matrix.hpp>

#include <cmath>

namespace px
{
	namespace fn
	{
		namespace
		{
			const double pi = 3.141592653589793238463;
		}

		template <unsigned int _W, unsigned int _H>
		class perlin
		{
		public:
			struct gradient
			{
				double x;
				double y;
			};
		private:
			matrix2<gradient, _W, _H> m_matrix; // gradient pairs

		public:
			// rng - random function should generate numbers in range 0..1
			// size - determines area of unique tile generated
			perlin()
			{
				if (_W == 0) throw std::runtime_error("w = 0");
				if (_H == 0) throw std::runtime_error("h = 0");
			}
			template <typename _Op>
			perlin(_Op rng) : perlin()
			{
				fill(rng);
			}
			template <typename _Op>
			void fill(_Op rng)
			{
				m_matrix.fill([&rng](unsigned int i, unsigned int j) -> gradient
				{
					auto num = pi * 2 * rng();
					return{ std::cos(num), std::sin(num) };
				});
			}

			// get smoothed value in any position
			double sample(double x, double y) const
			{
				// get remainder for 'repeat tiling' effect
				x = modulo(x, _W);
				y = modulo(x, _H);

				// grid cell square coordinates
				int x0 = (int)std::floor(x);
				int y0 = (int)std::floor(y);
				int x1 = x0 + 1;
				int y1 = y0 + 1;

				// interpolation weights
				auto sx = x - (double)x0;
				auto sy = y - (double)y0;

				auto g00 = vertice_gradient(x0, y0, x, y);
				auto g10 = vertice_gradient(x1, y0, x, y);
				auto g01 = vertice_gradient(x0, y1, x, y);
				auto g11 = vertice_gradient(x1, y1, x, y);

				auto d = lerp(g00, g10, sx);
				auto u = lerp(g01, g11, sx);

				return lerp(d, u, sy);
			}

			// multisample with fractal sublevels
			double sample(double x, double y, unsigned int iterations) const
			{
				double total = 0;

				double multiplier = 1;
				for (unsigned int i = 0; i < iterations; ++i)
				{
					total += sample(x, y) * multiplier;
					x *= 2;
					y *= 2;
					multiplier *= 0.5;
				}
				return total;
			}

		private:
			// gradient influence of vertex vx,vt to point y
			double vertice_gradient(int vx, int vy, double x, double y) const
			{
				// distance
				auto dx = x - (double)vx;
				auto dy = y - (double)vy;

				vx = vx >= _W ? 0 : vx;
				vy = vy >= _H ? 0 : vy;

				const gradient &g = m_matrix.at(vx, vy);
				return dx * g.x + dy * g.y;
			}

			static double modulo(double a, unsigned int b)
			{
				return a - std::floor(a / b) * b;
			}

			// linear interpolation helper function
			template <typename _T>
			static _T lerp(_T a, _T b, _T w)
			{
				return a * (1 - w) + b * w;
			}
		};
	}
}