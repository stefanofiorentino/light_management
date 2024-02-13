#include <gmock/gmock.h>

#include <future>

int const MY_ID = 28;
const std::string MY_STRING = "Hello, World!";
const int num_mailboxes = 32;
std::atomic<int> mailbox_receiver[num_mailboxes];
std::string mailbox_data[num_mailboxes];

void
do_work(std::string const& mailbox_data_content)
{
    ASSERT_EQ(MY_STRING, mailbox_data_content);
}

// concurrency by Scott Meyers
template<typename F, typename... Args>
inline auto
reallyAsync(F&& f, Args&&... args)
{
    return std::async(
        std::launch::async, std::forward<F>(f), std::forward<Args>(args)...);
}

// Atomic-fence synchronization
TEST(atomic_thread_fence, cppreference)
{
    // The writer threads update non-atomic shared data
    mailbox_data[MY_ID] = MY_STRING;
    // and then update mailbox_receiver[i] as follows:
    auto f1 = reallyAsync([&]() {
        for (int i = 0; i < num_mailboxes; i++) {
            std::atomic_store_explicit(
                &mailbox_receiver[i], i, std::memory_order_release);
        }
    });

    // Reader thread needs to check all mailbox[i], but only needs to sync with
    // one.
    auto f2 = reallyAsync([&]() {
        for (int i = 0; i < num_mailboxes; ++i)
            if (std::atomic_load_explicit(&mailbox_receiver[i],
                                          std::memory_order_relaxed) == MY_ID) {
                // synchronize with just one writer
                std::atomic_thread_fence(std::memory_order_acquire);
                // guaranteed to observe everything done in the writer thread
                // before the atomic_store_explicit()
                do_work(mailbox_data[i]);
            }
    });
}