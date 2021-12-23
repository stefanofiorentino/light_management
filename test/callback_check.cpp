#include <gmock/gmock.h>

#include <atomic>
#include <iostream>
#include <thread>

#include <uv.h>

static uv_timer_t timeout_handle;
static uv_prepare_t prepare_handle;
static volatile std::atomic_bool callback_called_with_success;
static volatile std::atomic_bool to_be_skipped;
static std::thread th;
static constexpr std::size_t TIMEOUT = 5;
static constexpr std::size_t REPEAT = 5;
static constexpr std::size_t TASK_EXECUTION_TIME = 100;
static constexpr std::size_t SUCCESS_ALLOWED_TASK_EXECUTION_TIME = 200;
static constexpr std::size_t FAILING_ALLOWED_TASK_EXECUTION_TIME =  50;
static std::size_t counter = 0;

static void pass_through_cb(uv_handle_t *handle) {}
static void success_cb(uv_handle_t *handle) {
  ASSERT_TRUE(atomic_load(&callback_called_with_success));
}
static void failing_cb(uv_handle_t *handle) {
  ASSERT_FALSE(atomic_load(&callback_called_with_success));
}

static void timeout_cb(uv_timer_t *handle) {
  ASSERT_EQ(handle, &timeout_handle);
  auto &timeout = *static_cast<std::size_t *>(handle->data);
  if ((counter++) * REPEAT > timeout) {
    atomic_store(&to_be_skipped, true);
    uv_timer_stop(handle);
    if (!uv_is_closing((uv_handle_t *)handle)) {
      uv_close((uv_handle_t *)handle, failing_cb);
    }
  } else if (atomic_load(&callback_called_with_success)) {
    uv_timer_stop(handle);
    if (!uv_is_closing((uv_handle_t *)handle)) {
      uv_close((uv_handle_t *)handle, success_cb);
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
    if (atomic_load(&to_be_skipped))
      return;
    OnAsyncResult();
  });

  auto rc = uv_prepare_stop(handle);
  ASSERT_EQ(0, rc);
  uv_close((uv_handle_t *)handle, pass_through_cb);
}

struct callback_check_fixture: ::testing::Test{
  callback_check_fixture()
  {
    counter = 0;
    atomic_store(&callback_called_with_success, false);
    atomic_store(&to_be_skipped, false);
  }
};

TEST_F(callback_check_fixture, success) {
  int rc;

  rc = uv_prepare_init(uv_default_loop(), &prepare_handle);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  rc = uv_prepare_start(&prepare_handle, prepare_cb);
  ASSERT_TRUE(EXIT_SUCCESS == rc);

  rc = uv_timer_init(uv_default_loop(), &timeout_handle);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  rc = uv_timer_start(&timeout_handle, timeout_cb, TIMEOUT, REPEAT);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  timeout_handle.data =
      const_cast<std::size_t *>(&SUCCESS_ALLOWED_TASK_EXECUTION_TIME);

  rc = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  ASSERT_TRUE(EXIT_SUCCESS == rc);

  ASSERT_TRUE(atomic_load(&callback_called_with_success));
  ASSERT_TRUE(th.joinable());
  th.join();
}

TEST_F(callback_check_fixture, timeout) {
  int rc;

  rc = uv_prepare_init(uv_default_loop(), &prepare_handle);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  rc = uv_prepare_start(&prepare_handle, prepare_cb);
  ASSERT_TRUE(EXIT_SUCCESS == rc);

  rc = uv_timer_init(uv_default_loop(), &timeout_handle);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  rc = uv_timer_start(&timeout_handle, timeout_cb, 5, 5);
  ASSERT_TRUE(EXIT_SUCCESS == rc);
  timeout_handle.data =
      const_cast<std::size_t *>(&FAILING_ALLOWED_TASK_EXECUTION_TIME);

  rc = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  ASSERT_TRUE(EXIT_SUCCESS == rc);

  ASSERT_FALSE(atomic_load(&callback_called_with_success));
  ASSERT_TRUE(th.joinable());
  th.join();
}
