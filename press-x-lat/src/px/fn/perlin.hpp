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

		template <size_t W, size_t H>
		class perlin
		{
		public:
			struct gradient
			{
				double x;
				double y;
			};

		public:
			perlin()
			{
				static_assert(W > 1 && H > 1, "px::perlin require W > 1 && H > 1");
			}
			// rng - random function should generate numbers in range 0..1
			template <typename Generator>
			perlin(Generator rng) : perlin()
			{
				fill(rng);
			}
			// rng - random function should generate numbers in range 0..1
			template <typename Generator>
			void fill(Generator rng)
			{
				m_matrix.fill([&rng](unsigned int i, unsigned int j) -> gradient
				{
					double num = pi * 2 * rng();
					//int random = std::rand();
					return{ std::cos(num), std::sin(num) };
					//return{ std::cos((double)random), std::sin((double)random) };
				});
			}

			// get smoothed value in any position
			double sample(double x, double y) const
			{
				// get remainder for 'repeat tiling' effect
				// tiling also used in fractal sublevel sampling
				x = modulo(x, W);
				y = modulo(y, H);

				// grid cell square coordinates
				//int x0 = static_cast<int>(std::floor(x));
				//int y0 = static_cast<int>(std::floor(y));
				//int x1 = x0 + 1;
				//int y1 = y0 + 1;

				int x0 = static_cast<int>(x);
				int y0 = static_cast<int>(y);
				int x1 = x0 + 1;
				int y1 = y0 + 1;

				//auto g00 = vertice_gradient(x0, y0, x, y);
				//auto g10 = vertice_gradient(x1, y0, x, y);
				//auto g01 = vertice_gradient(x0, y1, x, y);
				//auto g11 = vertice_gradient(x1, y1, x, y);

				//// interpolation weights
				double sx = x - x0;
				double sy = y - y0;

				//auto d = lerp(g00, g10, sx);
				//auto u = lerp(g01, g11, sx);

				//return lerp(d, u, sy);

				double n0, n1, ix0, ix1;
				n0 = vertice_gradient(x0, y0, x, y);
				n1 = vertice_gradient(x1, y0, x, y);
				ix0 = lerp(n0, n1, sx);
				n0 = vertice_gradient(x0, y1, x, y);
				n1 = vertice_gradient(x1, y1, x, y);
				ix1 = lerp(n0, n1, sx);

				return lerp(ix0, ix1, sy);
			}

			// multisample with fractal sublevels
			double sample(double x, double y, unsigned int iterations) const
			{
				double total = 0;
				double multiplier = 1;
				for (; iterations > 0; --iterations)
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
				auto dx = x - vx;
				auto dy = y - vy;

				vx = (vx == W) ? 0 : vx;
				vy = (vy == H) ? 0 : vy;

				const gradient &g = m_matrix.at(point2(vx, vy));
				return dx * g.x + dy * g.y;
			}

			static double modulo(double a, unsigned int b)
			{
				return a - std::floor(a / b) * b;
			}

			// linear interpolation helper function
			template <typename T>
			static T lerp(T a, T b, T w)
			{
				return a * (1 - w) + b * w;
			}

		private:
			matrix2<gradient, W, H> m_matrix; // gradient pairs
		};
	}
}