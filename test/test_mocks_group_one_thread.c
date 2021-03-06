#include "mocks.h"
#include "unity_fixture.h"
#include <string.h>

/*******************************************************************************
 * MocksOneThread test group
 * Testing functionality of mocks in single-thread environment
 ******************************************************************************/
TEST_GROUP(MocksOneThread);

/*******************************************************************************
 * Local variables and supplemantary procedures
 ******************************************************************************/

#define NUM_THREADS                             (1)
#define NUM_EXPECTATIONS                        (4)
#define CONTEXT_BUFFER_SIZE                     (64)

#define DEFAULT_THREAD_INDEX                    (0)

#define EMPTY_CONTEXT_SIZE                      (0)
#define EMPTY_CONTEXT_DATA                      (NULL)

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
  const mocks_expectation_t expected = {
    .id             = 0,
    .context_size   = EMPTY_CONTEXT_SIZE,
    .context_data   = EMPTY_CONTEXT_DATA
  };

  mocks_return_code   rc;

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  rc = mocks_expect(
    DEFAULT_THREAD_INDEX,
    &expected);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc,
    "Expected status: mocks_success");
}

/*
 * Scenario: "expect" called with NULL expectation fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_expect() with NULL expectation;
 * Then:     Returned code mocks_invalid_argument.
 */
TEST(MocksOneThread, ExpectCalledWithNullExpectationFails)
{
  mocks_expectation_t *expected = NULL;

  mocks_return_code   rc;

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  rc = mocks_expect(
    DEFAULT_THREAD_INDEX,
    expected);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_invalid_argument, rc,
    "Expected status: mocks_invalid_argument");
}

/*
 * Scenario: "expect" called with negative context size fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_expect() with negative context_size;
 * Then:     Returned code mocks_invalid_ctx_size.
 */
TEST(MocksOneThread, ExpectCalledWithNegativeContextSizeFails)
{
  const mocks_expectation_t expected = {
    .id           = 0,
    .context_size = -1,
    .context_data = EMPTY_CONTEXT_DATA
  };

  mocks_return_code   rc;

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  rc = mocks_expect(
    DEFAULT_THREAD_INDEX,
    &expected);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_invalid_ctx_size, rc,
    "Expected status: mocks_invalid_ctx_size");
}

/*
 * Scenario: "expect" called with positive context size and NULL data fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_expect() with positive context_size
 *           and NULL context_data;
 * Then:     Returned code mocks_invalid_ctx.
 */
TEST(MocksOneThread, ExpectCalledWithPositiveContextSizeAndNullDataFails)
{
  const mocks_expectation_t expected = {
    .id           = 0,
    .context_size = 1,
    .context_data = EMPTY_CONTEXT_DATA
  };

  mocks_return_code   rc;

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  rc = mocks_expect(
    DEFAULT_THREAD_INDEX,
    &expected);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_invalid_ctx, rc,
    "Expected status: mocks_invalid_ctx");
}

/*
 * Scenario: "invoke" called with NULL expectation fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_invoke() with NULL expectation;
 * Then:     Returned code mocks_invalid_argument.
 */
TEST(MocksOneThread, InvokeCalledWithNullExpectationFails)
{
  mocks_expectation_t *invoked = NULL;

  mocks_return_code   rc;

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  rc = mocks_invoke(invoked);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_invalid_argument, rc,
    "Expected status: mocks_invalid_argument");
}

/*
 * Scenario: "invoke" called prior to "expect" fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_invoke() without preceeding mocks_expect();
 * Then:     Returned code mocks_no_more_expectations.
 */
TEST(MocksOneThread, InvokeCalledBeforeExpectFails)
{
  mocks_expectation_t invoked;

  mocks_return_code   rc;

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  rc = mocks_invoke(&invoked);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_no_more_expectations, rc,
    "Expected status: mocks_no_more_expectations");
}

/*
 * Scenario: "invoke" called after "expect" succeeds;
 * Given:    Called mocks_expect() with empty context;
 * When:     Called mocks_invoke();
 * Then:     Returned code mocks_success and matching expectation_id
 *           and empty context.
 */
TEST(MocksOneThread, InvokeCalledAfterExpectSucceeds)
{
  const mocks_expectation_t expected = {
    .id           = 0,
    .context_size = EMPTY_CONTEXT_SIZE,
    .context_data = EMPTY_CONTEXT_DATA
  };

  mocks_expectation_t invoked = {
    .id           = 12345,
    .context_size = 100,
    .context_data = (void*)0x12345
  };

  mocks_return_code   rc;

  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected),
    "mocks_expect() failed");

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  rc = mocks_invoke(&invoked);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc,
    "mocks_invoke() failed");

  TEST_ASSERT_EQUAL_MESSAGE(expected.id, invoked.id,
    "mocks_invoke() returned bad expectation_id");

  TEST_ASSERT_EQUAL_MESSAGE(EMPTY_CONTEXT_SIZE, invoked.context_size,
    "mocks_invoke() returned bad context_size");

  TEST_ASSERT_EQUAL_PTR_MESSAGE(EMPTY_CONTEXT_DATA, invoked.context_data,
    "mocks_invoke() returned bad context_data");
}

/*
 * Scenario: "invoke" called twice after single "expect" fails;
 * Given:    Called mocks_expect() with empty context once;
 * When:     Called mocks_invoke() twice;
 * Then:     Returned code mocks_no_more_expectations.
 */
TEST(MocksOneThread, InvokeCalledTwiceAfterSingleExpectFails)
{
  const mocks_expectation_t expected = {
    .id           = 0,
    .context_size = EMPTY_CONTEXT_SIZE,
    .context_data = EMPTY_CONTEXT_DATA
  };

  mocks_expectation_t invoked = {
    .id           = 12345,
    .context_size = 100,
    .context_data = (void*)0x12345
  };

  mocks_return_code   rc;

  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected),
    "mocks_expect() failed");

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  /* first invoke */
  rc = mocks_invoke(&invoked);

  /* second invoke */
  rc = mocks_invoke(&invoked);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_no_more_expectations, rc,
    "Expected status: mocks_no_more_expectations");
}

/*
 * Scenario: "invoke" called twice after "expect" twice succeeds;
 * Given:    Called mocks_expect() with empty context twice;
 * When:     Called mocks_invoke() twice;
 * Then:     Returned code mocks_success for both "invoke", expectation_id's
 *           matched the values provided in "expect", empty context.
 */
TEST(MocksOneThread, InvokeCalledTwiceAfterExpectTwiceSucceeds)
{
  const mocks_expectation_t expected1 = {
    .id           = 1,
    .context_size = EMPTY_CONTEXT_SIZE,
    .context_data = EMPTY_CONTEXT_DATA
  };

  const mocks_expectation_t expected2 = {
    .id           = 2,
    .context_size = EMPTY_CONTEXT_SIZE,
    .context_data = EMPTY_CONTEXT_DATA
  };

  mocks_expectation_t invoked1 = {
    .id           = 12345,
    .context_size = 100,
    .context_data = (void*)0x12345
  };

  mocks_expectation_t invoked2 = {
    .id           = 56789,
    .context_size = 200,
    .context_data = (void*)0x56789
  };

  mocks_return_code   rc1, rc2;

  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  /* first expect */
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected1),
    "First mocks_expect() failed");

  /* second expect */
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected2),
    "Second mocks_expect() failed");

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  /* first invoke */
  rc1 = mocks_invoke(&invoked1);

  /* second invoke */
  rc2 = mocks_invoke(&invoked2);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  /* first invoke results */
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc1,
    "First mocks_invoke() failed");

  TEST_ASSERT_EQUAL_MESSAGE(expected1.id, invoked1.id,
    "First mocks_invoke() returned bad expectation_id");

  TEST_ASSERT_EQUAL_MESSAGE(EMPTY_CONTEXT_SIZE, invoked1.context_size,
    "First mocks_invoke() returned bad context_size");

  TEST_ASSERT_EQUAL_PTR_MESSAGE(EMPTY_CONTEXT_DATA, invoked1.context_data,
    "First mocks_invoke() returned bad context_data");

  /* second invoke results */
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc2,
    "Second mocks_invoke() failed");

  TEST_ASSERT_EQUAL_MESSAGE(expected2.id, invoked2.id,
    "Second mocks_invoke() returned bad expectation_id");

  TEST_ASSERT_EQUAL_MESSAGE(EMPTY_CONTEXT_SIZE, invoked2.context_size,
    "Second mocks_invoke() returned bad context_size");

  TEST_ASSERT_EQUAL_PTR_MESSAGE(EMPTY_CONTEXT_DATA, invoked2.context_data,
    "Second mocks_invoke() returned bad context_data");
}

/*
 * Scenario: "expect" called up to max number of expectations succeeds;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_expect() NUM_EXPECTATION times with proper
 *           thread index and empty context;
 * Then:     Returned code mocks_success.
 */
TEST(MocksOneThread, ExpectCalledUpToMaxExpectationsSucceeds)
{
  mocks_expectation_t expected = {
    .context_size = EMPTY_CONTEXT_SIZE,
    .context_data = EMPTY_CONTEXT_DATA
  };

  int                 i;
  mocks_return_code   rc;

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  for (i = 0; i < NUM_EXPECTATIONS; i++) {
    expected.id = i;
    rc = mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected);
  }

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc,
    "Expected status: mocks_success");
}

/*
 * Scenario: "expect" called more than max number of expectations fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_expect() NUM_EXPECTATION + 1 times with proper
 *           thread index and empty context;
 * Then:     Returned code mocks_no_room_for_expectation.
 */
TEST(MocksOneThread, ExpectCalledMoreThanMaxExpectationsFails)
{
  mocks_expectation_t expected = {
    .context_size = EMPTY_CONTEXT_SIZE,
    .context_data = EMPTY_CONTEXT_DATA
  };

  int                 i;
  mocks_return_code   rc;

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  /* call "expect" up to max number of expectations */
  for (i = 0; i < NUM_EXPECTATIONS; i++) {
    expected.id = i;
    rc = mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected);
  }

  /* call "expect" one more time to exceed max number of expectations */
  expected.id = i;
  rc = mocks_expect(
    DEFAULT_THREAD_INDEX,
    &expected);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_no_room_for_expectation, rc,
    "Expected status: mocks_no_room_for_expectation");
}

/*
 * Scenario: "expect" can pass non-empty context up to context_buffer_size size;
 * Given:    Called mocks_expect() with non-empty context;
 * When:     Destroyed the passed context data and called mocks_invoke();
 * Then:     Returned code mocks_success and matching expectation_id
 *           and retrieved context matching original context.
 */
TEST(MocksOneThread, ExpectCanPassNonEmptyContext)
{
  uint8_t reference_data[CONTEXT_BUFFER_SIZE];

  uint8_t expected_data[sizeof(reference_data)];
  const mocks_expectation_t expected = {
    .id           = 0,
    .context_size = sizeof(reference_data),
    .context_data = expected_data
  };

  mocks_expectation_t invoked = {
    .id           = 12345,
    .context_size = 100,
    .context_data = NULL
  };

  int                 i;
  mocks_return_code   rc;

  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  for (i = 0; i < sizeof(reference_data); i++) {
    reference_data[i] = (uint8_t)i;
  }
  memcpy(expected_data, reference_data, sizeof(reference_data));

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  /* pass non-empty context to "expect" */
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected),
    "mocks_expect() failed");

  /* destroy the passed context to make sure the data was saved */
  memset(expected_data, 0xaa, sizeof(expected_data));

  /* retrieve expectation with "invoke" */
  rc = mocks_invoke(&invoked);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc,
    "mocks_invoke() failed");

  TEST_ASSERT_EQUAL_MESSAGE(expected.id, invoked.id,
    "mocks_invoke() returned bad expectation_id");

  TEST_ASSERT_EQUAL_MESSAGE(expected.context_size, invoked.context_size,
    "mocks_invoke() returned bad context_size");

  TEST_ASSERT_NOT_NULL_MESSAGE(invoked.context_data,
    "mocks_invoke() returned NULL context_data");

  TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
    reference_data, invoked.context_data, invoked.context_size,
    "mocks_invoke() context_data does not match");
}

/*
 * Scenario: "expect" passing context exceeding context_buffer_size fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_expect() with context exceeding context_buffer_size;
 * Then:     Returned code mocks_no_room_for_ctx_data.
 */
TEST(MocksOneThread, ExpectPassingContextExceedingMaxSizeFails)
{
  uint8_t expected_data[CONTEXT_BUFFER_SIZE + 1];
  const mocks_expectation_t expected = {
    .id           = 0,
    .context_size = sizeof(expected_data),
    .context_data = expected_data
  };

  int                 i;
  mocks_return_code   rc;

  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  for (i = 0; i < sizeof(expected_data); i++) {
    expected_data[i] = (uint8_t)i;
  }

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  rc = mocks_expect(
    DEFAULT_THREAD_INDEX,
    &expected);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_no_room_for_ctx_data, rc,
    "Expected status: mocks_no_room_for_ctx_data");
}

/*
 * Scenario: "expect" can pass multiple non-empty context expectations;
 * Given:    Called mocks_expect() with different non-empty contexts twice;
 * When:     Called mocks_invoke() twice;
 * Then:     Both "invoke" return mocks_success and matching expectation_id's
 *           and retrieved contexts matching original contexts.
 */
TEST(MocksOneThread, ExpectCanPassMultipleNonEmptyContexts)
{
  const int expectation_id1 = 10;
  const int expectation_id2 = 20;
  const uint8_t reference_data1[] = {5, 6, 7, 8, 9};
  const uint8_t reference_data2[] = {10, 20, 30, 40};

  mocks_expectation_t expected;

  mocks_expectation_t invoked1 = {
    .id           = 12345,
    .context_size = 100,
    .context_data = NULL
  };

  mocks_expectation_t invoked2 = {
    .id           = 56789,
    .context_size = 150,
    .context_data = NULL
  };

  mocks_return_code   rc1, rc2;

  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  /* first "expect" */
  expected.id = expectation_id1;
  expected.context_size = sizeof(reference_data1);
  expected.context_data = (void*)reference_data1;
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected),
    "First mocks_expect() failed");

  /* second "expect" */
  expected.id = expectation_id2;
  expected.context_size = sizeof(reference_data2);
  expected.context_data = (void*)reference_data2;
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success,
    mocks_expect(
      DEFAULT_THREAD_INDEX,
      &expected),
    "Second mocks_expect() failed");

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  /* first "invoke" */
  rc1 = mocks_invoke(&invoked1);

  /* second "invoke" */
  rc2 = mocks_invoke(&invoked2);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  /* verify first "invoke" results */
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc1,
    "First mocks_invoke() failed");

  TEST_ASSERT_EQUAL_MESSAGE(expectation_id1, invoked1.id,
    "First mocks_invoke() returned bad expectation_id");

  TEST_ASSERT_EQUAL_MESSAGE(sizeof(reference_data1), invoked1.context_size,
    "First mocks_invoke() returned bad context_size");

  TEST_ASSERT_NOT_NULL_MESSAGE(invoked1.context_data,
    "First mocks_invoke() returned NULL context_data");

  TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
    reference_data1, invoked1.context_data, invoked1.context_size,
    "First mocks_invoke() context_data does not match");

  /* verify second "invoke" results */
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc2,
    "Second mocks_invoke() failed");

  TEST_ASSERT_EQUAL_MESSAGE(expectation_id2, invoked2.id,
    "Second mocks_invoke() returned bad expectation_id");

  TEST_ASSERT_EQUAL_MESSAGE(sizeof(reference_data2), invoked2.context_size,
    "Second mocks_invoke() returned bad context_size");

  TEST_ASSERT_NOT_NULL_MESSAGE(invoked2.context_data,
    "Second mocks_invoke() returned NULL context_data");

  TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
    reference_data2, invoked2.context_data, invoked2.context_size,
    "Second mocks_invoke() context_data does not match");
}

/*
 * Scenario: "expect" passing context exceeding remaining context buffer fails;
 * Given:    Mocks and thread initialized;
 * When:     Called mocks_expect() with non empty context twice, the first
 *           context uses half of the context buffer, the second one uses
 *           the remaining context buffer + 1 byte;
 * Then:     First "expect" returns mocks_success,
 *           second - mocks_no_room_for_ctx_data.
 */
TEST(MocksOneThread, ExpectPassingContextExceedingRemainingSizeFails)
{
  uint8_t expected_data[CONTEXT_BUFFER_SIZE / 2 + 1];
  mocks_expectation_t expected = {
    .context_data = expected_data
  };

  int                 i;
  mocks_return_code   rc1, rc2;

  /*-------------------------------------------
  | Set expectations
  -------------------------------------------*/
  for (i = 0; i < sizeof(expected_data); i++) {
    expected_data[i] = (uint8_t)i;
  }

  /*-------------------------------------------
  | Perform test
  -------------------------------------------*/
  /* first "expect" */
  expected.id = 1;
  expected.context_size = CONTEXT_BUFFER_SIZE / 2;
  rc1 = mocks_expect(
    DEFAULT_THREAD_INDEX,
    &expected);

  /* second "expect" */
  expected.id = 2;
  expected.context_size = CONTEXT_BUFFER_SIZE - CONTEXT_BUFFER_SIZE / 2 + 1;
  rc2 = mocks_expect(
    DEFAULT_THREAD_INDEX,
    &expected);

  /*-------------------------------------------
  | Verify results
  -------------------------------------------*/
  TEST_ASSERT_EQUAL_MESSAGE(mocks_success, rc1,
    "First expected status: mocks_success");

  TEST_ASSERT_EQUAL_MESSAGE(mocks_no_room_for_ctx_data, rc2,
    "Second expected status: mocks_no_room_for_ctx_data");
}

/*******************************************************************************
 * Test group runner
 ******************************************************************************/
TEST_GROUP_RUNNER(MocksOneThread)
{
  RUN_TEST_CASE(MocksOneThread, ExpectCalledOnceSucceeds);
  RUN_TEST_CASE(MocksOneThread, ExpectCalledWithNullExpectationFails);
  RUN_TEST_CASE(MocksOneThread, ExpectCalledWithNegativeContextSizeFails);
  RUN_TEST_CASE(MocksOneThread,
    ExpectCalledWithPositiveContextSizeAndNullDataFails);
  RUN_TEST_CASE(MocksOneThread, InvokeCalledWithNullExpectationFails);
  RUN_TEST_CASE(MocksOneThread, InvokeCalledBeforeExpectFails);
  RUN_TEST_CASE(MocksOneThread, InvokeCalledAfterExpectSucceeds);
  RUN_TEST_CASE(MocksOneThread, InvokeCalledTwiceAfterSingleExpectFails);
  RUN_TEST_CASE(MocksOneThread, InvokeCalledTwiceAfterExpectTwiceSucceeds);
  RUN_TEST_CASE(MocksOneThread, ExpectCalledUpToMaxExpectationsSucceeds);
  RUN_TEST_CASE(MocksOneThread, ExpectCalledMoreThanMaxExpectationsFails);
  RUN_TEST_CASE(MocksOneThread, ExpectCanPassNonEmptyContext);
  RUN_TEST_CASE(MocksOneThread, ExpectPassingContextExceedingMaxSizeFails);
  RUN_TEST_CASE(MocksOneThread, ExpectCanPassMultipleNonEmptyContexts);
  RUN_TEST_CASE(MocksOneThread, ExpectPassingContextExceedingRemainingSizeFails);
}
