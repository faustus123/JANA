
// Copyright 2020, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef JANA2_JEVENTPOOL_H
#define JANA2_JEVENTPOOL_H

#include <JANA/JEvent.h>
#include <JANA/JFactoryGenerator.h>

class JEventPool {
private:

    struct alignas(64) LocalPool {
        std::mutex mutex;
        std::vector<std::shared_ptr<JEvent>> events;
    };

    std::vector<JFactoryGenerator*>* m_generators;
    size_t m_pool_size;
    size_t m_location_count;
    bool m_limit_total_events_in_flight;
    std::unique_ptr<LocalPool[]> m_pools;

public:
    inline JEventPool(std::vector<JFactoryGenerator*>* generators,
                      size_t pool_size, size_t location_count, bool limit_total_events_in_flight)
        : m_generators(generators)
        , m_pool_size(pool_size)
        , m_location_count(location_count)
        , m_limit_total_events_in_flight(limit_total_events_in_flight)
    {
        assert(m_location_count >= 1);
        m_pools = std::unique_ptr<LocalPool[]>(new LocalPool[location_count]());

        for (int j=0; j<m_location_count; ++j) {
            for (int i=0; i<m_pool_size; ++i) {
                auto event = std::make_shared<JEvent>();
                auto factory_set = new JFactorySet(*m_generators);
                event->SetFactorySet(factory_set);
                put(event, j);
            }
        }
    }

    inline std::shared_ptr<JEvent> get(size_t location) {

        LocalPool& pool = m_pools[location % m_location_count];
        std::lock_guard<std::mutex> lock(pool.mutex);

        if (pool.events.empty()) {
            if (m_limit_total_events_in_flight) {
                return nullptr;
            }
            else {
                auto event = std::make_shared<JEvent>();
                auto factory_set = new JFactorySet(*m_generators);
                event->SetFactorySet(factory_set);
                return event;
            }
        }
        else {
            auto event = std::move(pool.events.back());
            pool.events.pop_back();
            return event;
        }
    }

    inline void put(std::shared_ptr<JEvent>& event, size_t location) {

        event->mFactorySet->Release();
        LocalPool& pool = m_pools[location % m_location_count];
        std::lock_guard<std::mutex> lock(pool.mutex);

        if (pool.events.size() < m_pool_size) {
            pool.events.push_back(std::move(event));
        }
    }
};


#endif //JANA2_JEVENTPOOL_H
