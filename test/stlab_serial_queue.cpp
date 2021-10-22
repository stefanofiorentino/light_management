#include <gmock/gmock.h>

#include <iostream>
#include <string>
#include <thread>
#include <numeric>
#include <unordered_map>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/utility.hpp>

using namespace stlab;

struct registry_base
{
    virtual ~registry_base() = default;
    using key_type           = std::string;
    using value_type         = std::string;
    using map_t              = std::unordered_map<key_type, value_type>;
};

struct mutex_registry : public registry_base
{
    mutable std::mutex mtx;
    map_t              _map;

    void set( key_type k, value_type v )
    {
        std::lock_guard<std::mutex> lg{mtx};
        _map[k] = v;
    }
    [[nodiscard]] value_type get( key_type k ) const
    {
        std::lock_guard<std::mutex> lg{mtx};
        return _map.at( k );
    }
};


TEST( stlab, mutex_registry )
{
    mutex_registry registry;
    registry.set( "key", "value" );

    ASSERT_EQ("value", registry.get( "key" ));
};

struct serial_queue_registry : public registry_base
{
    serial_queue_t         _q;
    std::shared_ptr<map_t> _map;

    serial_queue_registry()
        : _q( default_executor, schedule_mode::all )
        , _map( std::make_shared<map_t>() )
    {
    }

    auto set( key_type k, value_type v ) -> future<void>
    {
        return _q( [&]( key_type k, value_type v ) {
            _map->emplace( std::move( k ), std::move( v ) );
        },
                   std::move( k ), std::move( v ) );
    }
    [[nodiscard]] auto get( key_type k ) const -> future<value_type>
    {
        return _q( [&]( key_type k ) { return _map->at( k ); }, std::move( k ) );
    }
};

TEST( stlab, serial_queue_registry )
{
    serial_queue_registry registry;
    std::size_t           i = 0;
    auto                  f = registry.set( "key", "value" );

    auto result = registry.get( "key" );

    // Waiting just for illustration purpose
    while ( ! result.get_try() )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    ASSERT_EQ("value", result.get_try());
}
