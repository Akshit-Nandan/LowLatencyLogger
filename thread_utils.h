#pragma once
#include <iostream>
#include <atomic>
#include <thread>
#include <windows.h>

namespace Common
{

    template <typename T, typename... A>
    inline auto createAndStartThread(int core_id, const std::string &name, T &&func, A &&...args) noexcept
    {
        std::atomic<bool> running(false), failed(false);

        auto thread_body = [&]()
        {
            if (core_id >= 0)
            {
                // Set core affinity using Windows API
                DWORD_PTR mask = 1 << core_id;
                if (!SetThreadAffinityMask(GetCurrentThread(), mask))
                {
                    std::cerr << "Failed to set core affinity for " << name << " " << GetCurrentThreadId() << " to " << core_id << std::endl;
                    failed = true;
                    return;
                }
                std::cout << "Set core affinity for " << name << " " << GetCurrentThreadId() << " to " << core_id << std::endl;
            }
            running = true;
            std::forward<T>(func)((std::forward<A>(args))...);
        };

        auto t = new std::thread(thread_body);

        while (!running && !failed)
        {
            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(1s);
        }

        if (failed)
        {
            t->join();
            delete t;
            t = nullptr;
        }

        return t;
    }

}