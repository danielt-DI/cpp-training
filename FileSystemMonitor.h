//
// Created by daniel on 17/04/2022.
//

#ifndef CPP_TRAINING_NEW_FILESYSTEMMONITOR_H
#define CPP_TRAINING_NEW_FILESYSTEMMONITOR_H


#include <set>
#include <filesystem>
#include <sys/fanotify.h>
#include <thread>
#include "FanAsyncQueue.h"

class FileSystemMonitor {
    int is_running = 0;
    std::set<std::filesystem::path> files_paths;
    std::set<std::filesystem::path> exec_paths;
    FanAsyncQueue<EventItem>& event_q;
    FanAsyncQueue<struct fanotify_response>& reply_q;

    // monitor thread
    std::thread t;

    // private functions
    void throw_errno(const std::string& msg);
    void monitor_events();
public:
    // c'tor
    FileSystemMonitor(std::set<std::filesystem::path>& paths,
                      std::set<std::filesystem::path>& exec_paths,
                      FanAsyncQueue<EventItem>& event_q,
                      FanAsyncQueue<struct fanotify_response>& reply_q);

    // d'tor
    virtual ~FileSystemMonitor();

    // File system monitor interface
    void start();
    void stop();
    void wait() {t.join();}
};


#endif //CPP_TRAINING_NEW_FILESYSTEMMONITOR_H
