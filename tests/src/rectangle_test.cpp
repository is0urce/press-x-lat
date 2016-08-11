// name: matrix_test.cpp

#include "catch.hpp"

#include <px/common/rectangle.hpp>

TEST_CASE("rectangle", "[rectangle]")
{
	px::rectangle rect({ 2, 2 }, { 10, 10 });
	px::rectangle orig = rect;

	rect.inflate(1);

	REQUIRE(rect.start() == px::point2(1, 1));
	REQUIRE(rect.corner() == px::point2(13, 13));
	REQUIRE(rect.range() == px::point2(12, 12));

	REQUIRE(orig.inflated(1) == rect);
	REQUIRE(orig.inflated(10).deflated(10) == orig);

	rect.deflate(1);
	REQUIRE(rect == orig);
}