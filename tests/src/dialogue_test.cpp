// name: dialogue_test.cpp

#include "catch.hpp"

#include <px/rl/dialogue.hpp>

using namespace px;
using namespace px::rl;

TEST_CASE("dialogue", "[dialogue]")
{
	dialogue<std::string> dlg;

	dlg.add_reply("Hello");
	dlg.add_reply("ignore");
	dlg.add_reply("World");

	dlg.link_answers(0, { 1, 2 });
	dlg.link_answers(1, { 1 });
	dlg.link_answers(2, { 0, 1 });

	dialogue_evaluator<std::string> script;

	int run = 0;
	script.set_action([&](auto, auto) { ++run; });
	script.set_conditional([](auto reply, auto) { return *(reply.node()) != "ignore"; });

	dialogue_evaluator<std::string> s{ [](auto reply, auto) { return *(reply.node()) != "ignore"; }, [&](auto, auto) { ++run; } };

	auto state = dlg.start(0, script);

	REQUIRE(*(state.current()->node()) == "Hello");
	REQUIRE(state.total() == 2);
	REQUIRE(state.visible() == 1);
	REQUIRE(state.is_visible(0) == false);
	REQUIRE(state.is_visible(1) == true);

	state.select(0);
	REQUIRE(run == 1);
	REQUIRE(*(state.current()->node()) == "World");

	state.select(0);
	REQUIRE(run == 2);
	REQUIRE(*(state.current()->node()) == "Hello");

	state.select_first();
	REQUIRE(*(state.current()->node()) == "World");
}