//
// Created by daniel on 07/04/2022.
//

#ifndef CPP_TRAINING_NEW_FANOTIFYHANDLER_H
#define CPP_TRAINING_NEW_FANOTIFYHANDLER_H

#include <fcntl.h>
#include <filesystem>
#include <set>
#include <sys/fanotify.h>
#include <sys/epoll.h>
#include <thread>
#include <queue>
#include "FanAsyncQueue.h"

#define MAX_EVENTS 10

class FaNotifyHandler {
    int is_running = 0;

    // fanotify
    int fan_fd;

    FanAsyncQueue<EventItem>& event_q;
    FanAsyncQueue<struct fanotify_response>& reply_q;

    // epoll
    struct epoll_event re_events[MAX_EVENTS];
    int ep_fd;

    // polling thread
    std::thread t;

    // private functions
    void throw_errno(const std::string& msg);
    void poll_fanotify_fd();
    void read_fan_events(int fd);
    void handle_fan_replies();

public:
    // c'tor
    FaNotifyHandler(std::set<std::filesystem::path>& paths,
                    FanAsyncQueue<EventItem>& event_q,
                    FanAsyncQueue<struct fanotify_response>& reply_q);

    // d'tor
    virtual ~FaNotifyHandler();

    // FaNotify handler interface
    void start();
    void stop();
    void wait() {t.join();}

    // optional - add option to change maximum events
};

void FaNotifyHandler_tester();

#endif //CPP_TRAINING_NEW_FANOTIFYHANDLER_H