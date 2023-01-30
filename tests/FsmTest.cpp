#define CONFIG_CATCH_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../ComuFrame/Fsm.h"

TEST_CASE("fsm unit test") {
	Fsm testFsm;
	REQUIRE(testFsm.getCurrState() == Fsm::Registration);
	testFsm.nextState();
	REQUIRE(testFsm.getCurrState() == Fsm::Syncronization);
	testFsm.crash();
	REQUIRE(testFsm.getCurrState() == Fsm::Syncronization);
	testFsm.nextState();
	testFsm.nextState();
	testFsm.crash();
	REQUIRE(testFsm.getCurrState() == Fsm::Crash);
}