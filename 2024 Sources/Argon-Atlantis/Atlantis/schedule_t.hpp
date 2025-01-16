#pragma once

#include <deque>
#include <mutex>


namespace atlantis {
    template <class T>
    class schedule_t {
    private:
        std::deque<T> q;
        mutable std::mutex m;

    public:
        auto empty() -> bool
        {
            return q.empty();
        }

        auto enqueue(const T& t) -> void
        {
            std::lock_guard< std::mutex > lock(m);

            this->q.push_back(t);
        }

        auto dequeue() -> T
        {
            std::unique_lock< std::mutex > lock(m);

            if (!q.empty())
            {
                T val = q.front();
                q.pop_front();
                return val;
            }

            return T();
        }

        auto clear_queue() -> void
        {
            std::lock_guard< std::mutex > lock(m);

            while (!q.empty())
                q.pop_front();
        }
    };
}