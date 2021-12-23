#include <gmock/gmock.h>

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

#include <uv.h>

static constexpr std::size_t TIMEOUT = 1;
static constexpr std::size_t REPEAT = 1;
static constexpr std::size_t TASK_EXECUTION_TIME = 25;
static constexpr std::size_t SUCCESS_ALLOWED_TASK_EXECUTION_TIME = 50;
static constexpr std::size_t FAILING_ALLOWED_TASK_EXECUTION_TIME = 5;

static uv_timer_t timeout_handle;
static uv_prepare_t prepare_handle;
static volatile std::atomic_bool callback_called_with_success;
static volatile std::atomic_bool to_be_skipped;
static std::thread th;
static std::size_t counter = 0;
static std::size_t timeout = 0;

static void pass_through_cb(uv_handle_t *handle) {}
static void failing_cb(uv_handle_t *handle) {
  ASSERT_FALSE(atomic_load(&callback_called_with_success));
}

static void timeout_cb(uv_timer_t *handle) {
  ASSERT_EQ(handle, &timeout_handle);
  if ((counter++) * REPEAT > timeout) {
    atomic_store(&to_be_skipped, true);
    uv_timer_stop(handle);
    if (!uv_is_closing((uv_handle_t *)handle)) {
      uv_close((uv_handle_t *)handle, failing_cb);
    }
  } else if (atomic_load(&callback_called_with_success)) {
    uv_timer_stop(handle);
    if (!uv_is_closing((uv_handle_t *)handle)) {
      uv_close((uv_handle_t *)handle, pass_through_cb);
    }
  }
}

static void OnAsyncResult() {
  atomic_store(&callback_called_with_success, true);
}

static void prepare_cb(uv_prepare_t *handle) {
  ASSERT_EQ(handle, &prepare_handle);

  // simulating an external lib's API call
  // a more libuv way would be to create a `work' request
  th = std::thread([] {
    std::this_thread::sleep_for(std::chrono::milliseconds(TASK_EXECUTION_TIME));
    if (atomic_load(&to_be_skipped)) {
      return;
    }
    OnAsyncResult();
  });

  auto rc = uv_prepare_stop(handle);
  ASSERT_EQ(0, rc);
  uv_close((uv_handle_t *)handle, pass_through_cb);
}

struct base_callback_check_fixture : ::testing::Test {
  base_callback_check_fixture() {
    counter = 0;
    atomic_store(&callback_called_with_success, false);
    atomic_store(&to_be_skipped, false);
  }
  virtual ~base_callback_check_fixture() {
    if (th.joinable())
      th.join();
  };

protected:
  int rc;
};

struct success_callback_check_fixture : base_callback_check_fixture {
  success_callback_check_fixture() {
    timeout = SUCCESS_ALLOWED_TASK_EXECUTION_TIME;
  }
  ~success_callback_check_fixture() {
    assert(atomic_load(&callback_called_with_success));
  }
};

struct failing_callback_check_fixture : base_callback_check_fixture {
  failing_callback_check_fixture() {
    timeout = FAILING_ALLOWED_TASK_EXECUTION_TIME;
  }
  ~failing_callback_check_fixture() {
    assert(!atomic_load(&callback_called_with_success));
  }
};

TEST_F(success_callback_check_fixture, success) {
  rc = uv_prepare_init(uv_default_loop(), &prepare_handle);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  rc = uv_prepare_start(&prepare_handle, prepare_cb);
  ASSERT_TRUE(EXIT_SUCCESS == rc);

  rc = uv_timer_init(uv_default_loop(), &timeout_handle);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  rc = uv_timer_start(&timeout_handle, timeout_cb, TIMEOUT, REPEAT);
  ASSERT_TRUE(EXIT_SUCCESS == rc);

  rc = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
}

TEST_F(failing_callback_check_fixture, timeout) {
  rc = uv_prepare_init(uv_default_loop(), &prepare_handle);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  rc = uv_prepare_start(&prepare_handle, prepare_cb);
  ASSERT_TRUE(EXIT_SUCCESS == rc);

  rc = uv_timer_init(uv_default_loop(), &timeout_handle);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  rc = uv_timer_start(&timeout_handle, timeout_cb, TIMEOUT, REPEAT);
  ASSERT_TRUE(EXIT_SUCCESS == rc);

  rc = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
}
