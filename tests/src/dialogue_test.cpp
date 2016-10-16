// name: dialogue_test.cpp

#include "catch.hpp"

#include <px/rl/dialogue.hpp>

using namespace px;
using namespace px::rl;

TEST_CASE("dialogue", "[dialogue]")
{
	dialogue<std::string> dlg;

	dlg.add_reply("Hello");
	dlg.add_reply("World");

	dlg.link_answers(0, { 1 });
	dlg.link_answers(1, { 0 });

	auto state = dlg.start(0);

	REQUIRE(*(state.current()->node()) == "Hello");
	state.select(0);
	REQUIRE(*(state.current()->node()) == "World");
	state.select(0);
	REQUIRE(*(state.current()->node()) == "Hello");
}