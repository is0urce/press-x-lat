// name: automata_test.cpp

#include "catch.hpp"

#include <px/fn/cell_automata.hpp>

TEST_CASE("cell_automata", "[cell_automata]")
{
	const unsigned int W = 100;
	const unsigned int H = 100;
	int w = std::rand() % 20 + 20;
	int h = std::rand() % 20 + 20;

	auto generator = [](unsigned int x, unsigned int y) -> int { return 0; };

	// constructors
	px::fn::cell_automata<int, W, H> map(generator);

	map.next_generation<int>([](int a, int e) { return a + 1; }, 0, [](int e) { return e; });
	REQUIRE((map->at({0, 0})) == 9);
}