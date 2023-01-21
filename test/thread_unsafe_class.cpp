#include <gmock/gmock.h>

#include <future>

class thread_unsafe
{
  bool status;

public:
  thread_unsafe()
    : status(false)
  {}
  bool getStatus() const { return status; }

  void setStatus(bool status) { thread_unsafe::status = status; }
};

TEST(thread_unsafe, basic)
{
  thread_unsafe ts;
  ASSERT_FALSE(ts.getStatus());
}

// concurrency by Scott Meyers
template<typename F, typename... Args>
inline auto
reallyAsync(F&& f, Args&&... args)
{
  return std::async(
    std::launch::async, std::forward<F>(f), std::forward<Args>(args)...);
}

TEST(thread_unsafe, thsan)
{
  thread_unsafe ts;
  bool status{};

  ASSERT_FALSE(status);

  auto th1 =
    reallyAsync([](thread_unsafe& ts) { ts.setStatus(true); }, std::ref(ts));

  auto th2 = reallyAsync(
    [](thread_unsafe const& ts, bool& status) { status = ts.getStatus(); },
    std::cref(ts),
    std::ref(status));
}