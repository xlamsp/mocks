#include "mocks.h"
#include "unity_fixture.h"

/*******************************************************************************
 * MockInit test group
 * Testing functionality related to mocks initialisation and cleanup
 ******************************************************************************/
TEST_GROUP(MocksOneThread);

/*******************************************************************************
 * Local variables and supplemantary procedures
 ******************************************************************************/

#define NUM_THREADS                             (1)
#define NUM_EXPECTATIONS                        (4)
#define CONTEXT_BUFFER_SIZE                     (64)

#define DEFAULT_THREAD_INDEX                    (0)

void test_mocks_reset_static_variables (void);

pthread_t thread_main;

/*******************************************************************************
 * Test setup and teardown
 ******************************************************************************/
TEST_SETUP(MocksOneThread)
{
  test_mocks_reset_static_variables();

  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_init(NUM_THREADS, CONTEXT_BUFFER_SIZE),
    "Mocks initialization failed");

  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_init_thread(DEFAULT_THREAD_INDEX, &thread_main, NUM_EXPECTATIONS),
    "Mocks thread initialization failed");
}

TEST_TEAR_DOWN(MocksOneThread)
{
  mocks_cleanup();
}

/*******************************************************************************
 * Test cases
 ******************************************************************************/
/*
 * Scenario: "expect" called once succeeds;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_expect() with proper thread index and empty context;
 * Then:     Returned code mocks_success.
 */
TEST(MocksOneThread, ExpectCalledOnceSucceeds)
{
  /*-------------------------------------------
  | Perform test and Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_expect(DEFAULT_THREAD_INDEX, 0, 0, NULL),
    "Expected status: mocks_success");
}

/*
 * Scenario: "invoke" called prior to "expect" fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_invoke() without preceeding mocks_expect();
 * Then:     Returned code mocks_no_more_expectations.
 */
TEST(MocksOneThread, InvokeCalledBeforeExpectFails)
{
  uint32_t    expectation_id;
  uint32_t    context_size;
  void       *context_data;

  /*-------------------------------------------
  | Perform test and Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_no_more_expectations,
    mocks_invoke(&expectation_id, &context_size, &context_data),
    "Expected status: mocks_no_more_expectations");
}

/*******************************************************************************
 * Test group runner
 ******************************************************************************/
TEST_GROUP_RUNNER(MocksOneThread)
{
  RUN_TEST_CASE(MocksOneThread, ExpectCalledOnceSucceeds);
  RUN_TEST_CASE(MocksOneThread, InvokeCalledBeforeExpectFails);
}
