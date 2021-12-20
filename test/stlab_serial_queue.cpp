#include "stlab/concurrency/optional.hpp"
#include <chrono>
#include <gmock/gmock.h>

#include <future>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include <unordered_map>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/utility.hpp>

using namespace stlab;
constexpr int N = 100000;

struct registry_base {
  virtual ~registry_base() = default;
  using key_type = std::string;
  using value_type = std::string;
  using map_t = std::unordered_map<key_type, value_type>;
};

struct sync_registry : public registry_base {
  mutable std::mutex mtx;
  map_t _map;

  void set(key_type k, value_type v) {
    std::lock_guard<std::mutex> lg{mtx};
    _map[k] = v;
  }
  [[nodiscard]] value_type get(key_type k) const {
    std::lock_guard<std::mutex> lg{mtx};
    return _map.at(k);
  }
};

TEST(stlab, sync_registry) {
  sync_registry registry;
  registry.set("key", "value");

  ASSERT_EQ("value", registry.get("key"));
};

TEST(stlab, DISABLED_sync_registry_brute_force) {
  sync_registry registry;
  for (auto i = 0; i < N; ++i) {
    auto th = std::thread(
        [i, &registry] { registry.set(std::to_string(i), "value"); });
    th.detach();
  }

  while (registry._map.size() != N) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  ASSERT_EQ("value", registry.get("0"));
}

struct async_registry : public registry_base {
  serial_queue_t _q;
  std::shared_ptr<map_t> _map;

  async_registry()
      : _q(default_executor, schedule_mode::all),
        _map(std::make_shared<map_t>()) {}

  auto set(key_type k, value_type v) -> future<void> {
    return _q([&](key_type k,
                  value_type v) { _map->emplace(std::move(k), std::move(v)); },
              std::move(k), std::move(v));
  }
  [[nodiscard]] auto get(key_type k) const -> future<value_type> {
    return _q([&](key_type k) { return _map->at(k); }, std::move(k));
  }

  auto size() const -> future<decltype(_map->size())> {
    return _q([&]() { return _map->size(); });
  }
};

TEST(stlab, async_registry) {
  async_registry registry;
  std::size_t i = 0;
  auto f = registry.set("key", "value");

  auto result = registry.get("key");

  // Waiting just for illustration purpose
  while (!result.get_try()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  ASSERT_EQ("value", result.get_try());
}

TEST(stlab, continuation) {
  async_registry registry;
  auto f = registry.set("key", "value");
  auto result = f | [&] { return registry.get("key"); };

  // Waiting just for illustration purpose
  while (!result.get_try()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  ASSERT_EQ("value", result.get_try());
}

TEST(stlab, DISABLED_async_registry_brute_force) {
  async_registry registry;
  for (auto i = 0; i < N; ++i) {
    auto f = registry.set(std::to_string(i), "value");
    f.detach();
  }

  auto size_future = registry.size();
  while (!size_future.is_ready() || size_future.get_try().value() != N) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  auto result = registry.get("0");

  // Waiting just for illustration purpose
  while (!result.get_try()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  ASSERT_EQ("value", result.get_try());
}
