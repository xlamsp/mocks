#include "mocks.h"
#include "unity_fixture.h"


/*******************************************************************************
 * MockInit test group
 * Testing functionality related to mocks initialisation and cleanup
 ******************************************************************************/
TEST_GROUP(MocksInit);

pthread_t thread_main;

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
 * Scenario: Initialization of the mocks with 0 threads or empty context fails;
 * Given:    Mocks not initialized;
 * When:     Called mocks_init() with either number_of_threads or
 *           context_buffer_size equal to 0;
 * Then:     Initialization failed, mocks_not_initialized returned.
 */
TEST(MocksInit, MocksInitWithZeroThreadsOrEmptyContextFails)
{
  TEST_ASSERT_EQUAL_MESSAGE(mocks_not_initialized, mocks_init(0, 1),
    "number_of_threads == 0, context_buffer_size != 0");
  TEST_ASSERT_EQUAL_MESSAGE(mocks_not_initialized, mocks_init(1, 0),
    "number_of_threads != 0, context_buffer_size == 0");
  TEST_ASSERT_EQUAL_MESSAGE(mocks_not_initialized, mocks_init(0, 0),
    "number_of_threads == 0, context_buffer_size == 0");
}

/*
 * Scenario: Verification of the non-initialized mocks indicates status
 *           'not initialized';
 * Given:    Mocks not initialized;
 * When:     Called mocks_verify();
 * Then:     Returned code mocks_not_initialized.
 */
TEST(MocksInit, MocksVerifyWhenNotInitializedReturnsNotInitialized)
{
  TEST_ASSERT_EQUAL_MESSAGE(mocks_not_initialized, mocks_verify(),
    "Returned status not match");
}

/*
 * Scenario: Initialization of the thread when mocks non-initialized fails;
 * Given:    Mocks not initialized;
 * When:     Called mocks_init_thread();
 * Then:     Returned code mocks_not_initialized.
 */
TEST(MocksInit, MocksInitThreadWhenNotInitializedFails)
{
  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_not_initialized, mocks_verify(),
    "Before calling mocks_init_thread()");

  /*-------------------------------------------
  | Perform test and Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_not_initialized,
    mocks_init_thread(0, &thread_main, 1),
    "Returned status not match");
}

/*
 * Scenario: Initialization of the mocks with 1 thread and 1-byte context
 *           buffer succeeds;
 * Given:    Mocks not initialized;
 * When:     Called mocks_init() with either number_of_threads = 1 and
 *           context_buffer_size = 1;
 * Then:     Initialization succeed, mocks_success returned.
 */
TEST(MocksInit, MocksInitWithOneThreadsAndOneByteContextSucceeds)
{
  /*-------------------------------------------
  | Perform test and Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, mocks_init(1, 1),
    "number_of_threads == 1, context_buffer_size == 1");
}


/*******************************************************************************
 * Test group runner
 ******************************************************************************/
TEST_GROUP_RUNNER(MocksInit)
{
  RUN_TEST_CASE(MocksInit, MocksInitWithZeroThreadsOrEmptyContextFails);
  RUN_TEST_CASE(MocksInit, MocksVerifyWhenNotInitializedReturnsNotInitialized);
  RUN_TEST_CASE(MocksInit, MocksInitThreadWhenNotInitializedFails);
  RUN_TEST_CASE(MocksInit, MocksInitWithOneThreadsAndOneByteContextSucceeds);
}
