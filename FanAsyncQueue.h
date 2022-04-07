//
// Created by daniel on 17/04/2022.
//

#ifndef CPP_TRAINING_NEW_FANASYNCQUEUE_H
#define CPP_TRAINING_NEW_FANASYNCQUEUE_H


#include <cstdint>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

typedef struct {
    int32_t fd;
    int32_t pid;
} EventItem;

template<typename T>
class FanAsyncQueue {
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;

public:
    void push(T&& ev_item) {
        std::unique_lock<std::mutex> lk{m_mutex};
        m_queue.push(ev_item);
        m_cond.notify_one();
    }

    T wait_pop() {
        T tmp;

        std::unique_lock<std::mutex> lk{m_mutex};
        m_cond.wait(lk, [&]{return !m_queue.empty();});

        tmp = m_queue.front();
        m_queue.pop();

        return tmp;
    }

    bool empty() {return m_queue.empty();}
};

#endif //CPP_TRAINING_NEW_FANASYNCQUEUE_H
