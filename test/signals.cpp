#include <gmock/gmock.h>

#include <stdlib.h>
#include <signal.h>
#include <atomic>
#include <thread>

volatile std::atomic_bool ready;

static void catch_function(int /*signo*/) {
    atomic_store(&ready, true);
}

// C# fa partire il test 
// - manda DO7 alto su PADA
// - legge dalla emulator board il valore corrispondente a DO7
// - manda segnale per spegnere su PADA

TEST(signals, signals)
{
    atomic_store(&ready, false);
    // Set above function as signal handler for the SIGINT signal:
    if (signal(SIGUSR1, catch_function) == SIG_ERR) {
        ASSERT_TRUE(false);
    }

    std::atomic_bool called{false};
    std::thread th([&called]{
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        kill(getpid(),SIGUSR1);
        std::atomic_store(&called, true);
    });
    th.detach();

    while(!atomic_load(&ready)){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ASSERT_TRUE(std::atomic_load(&called));
}