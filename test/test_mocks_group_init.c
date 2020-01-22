#include "mocks.h"
#include "unity_fixture.h"

/*******************************************************************************
 * MockInit test group
 * Testing functionality related to mocks initialisation and cleanup
 ******************************************************************************/
TEST_GROUP(MocksInit);

/*******************************************************************************
 * Local variables and supplemantary procedures
 ******************************************************************************/
void test_mocks_reset_static_variables (void);

pthread_t thread_main;

/*******************************************************************************
 * Test setup and teardown
 ******************************************************************************/
TEST_SETUP(MocksInit)
{
  test_mocks_reset_static_variables();
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

/*
 * Scenario: Initialization of the thread with bad arguments fails;
 * Given:    Mocks initialized;
 * When:     Called mocks_init_thread() with thread_index >= number_of_threads
 *           or thread_id == NULL or number_of_expectation == 0;
 * Then:     Returned codes are mocks_thread_index_out_of_range,
 *           mocks_thread_id_is_null or mocks_thread_bad_number_of_expectations
 *           respectively.
 */
TEST(MocksInit, MocksInitThreadWithBadArgumentsFails)
{
  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  const int number_of_threads = 1;

  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_init(number_of_threads, 1),
    "Mocks initialization failed");

  /*-------------------------------------------
  | Perform test and Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_thread_index_out_of_range,
    mocks_init_thread(number_of_threads, &thread_main, 1),
    "Expected status: mocks_thread_index_out_of_range");

  TEST_ASSERT_EQUAL_MESSAGE(mocks_thread_id_is_null,
    mocks_init_thread(number_of_threads - 1, NULL, 1),
    "Expected status: mocks_thread_id_is_null");

  TEST_ASSERT_EQUAL_MESSAGE(mocks_thread_bad_number_of_expectations,
    mocks_init_thread(number_of_threads - 1, &thread_main, 0),
    "Expected status: mocks_thread_bad_number_of_expectations");
}

/*
 * Scenario: Initialization of the thread after mocks cleaned up returns
 *           mocks_not_initialized;
 * Given:    Mocks initialized and then cleaned up;
 * When:     Called mocks_init_thread() with valid thread_index, thread_id
 *           and number_of_expectation;
 * Then:     Returned code mocks_not_initialized.
 */
TEST(MocksInit, MocksInitThreadAfterMocksCleanupReturnsNotInitialized)
{
  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  const int number_of_threads = 1;

  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_init(number_of_threads, 1),
    "Mocks initialization failed");

  mocks_cleanup();

  /*-------------------------------------------
  | Perform test and Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_not_initialized,
    mocks_init_thread(number_of_threads - 1, &thread_main, 1),
    "Expected status: mocks_not_initialized");
}

/*
 * Scenario: Initialization of the thread after mocks init succeeds;
 * Given:    Mocks initialized;
 * When:     Called mocks_init_thread() with valid thread_index, thread_id
 *           and number_of_expectation;
 * Then:     Returned code mocks_success.
 */
TEST(MocksInit, MocksInitThreadAfterMocksInitSucceeds)
{
  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  const int number_of_threads = 1;

  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_init(number_of_threads, 1),
    "Mocks initialization failed");

  /*-------------------------------------------
  | Perform test and Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_init_thread(number_of_threads - 1, &thread_main, 1),
    "Expected status: mocks_success");
}

/*
 * Scenario: Verification after mocks init succeeds;
 * Given:    Mocks initialized;
 * When:     Called mocks_verify();
 * Then:     Returned code mocks_success.
 */
TEST(MocksInit, MocksVerifyAfterMocksInitSucceeds)
{
  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  const int number_of_threads = 1;

  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_init(number_of_threads, 1),
    "Mocks initialization failed");

  /*-------------------------------------------
  | Perform test and Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_verify(),
    "Returned status not match");
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
  RUN_TEST_CASE(MocksInit, MocksInitThreadWithBadArgumentsFails);
  RUN_TEST_CASE(MocksInit, MocksInitThreadAfterMocksCleanupReturnsNotInitialized);
  RUN_TEST_CASE(MocksInit, MocksInitThreadAfterMocksInitSucceeds);
  RUN_TEST_CASE(MocksInit, MocksVerifyAfterMocksInitSucceeds);
}
