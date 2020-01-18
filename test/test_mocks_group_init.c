#include "unity_fixture.h"

TEST_GROUP(MocksInit);

/*******************************************************************************
 * Test setup and teardown
 ******************************************************************************/
TEST_SETUP(MocksInit)
{
}

TEST_TEAR_DOWN(MocksInit)
{
}

/*******************************************************************************
 * Test cases
 ******************************************************************************/
/*
 * Scenario:
 * Given:
 * When:
 * Then:
 */
TEST(MocksInit, Dummy)
{
  TEST_FAIL_MESSAGE("Start here");
}

/*******************************************************************************
 * Test group runner
 ******************************************************************************/
TEST_GROUP_RUNNER(MocksInit)
{
  RUN_TEST_CASE(MocksInit, Dummy);
}
