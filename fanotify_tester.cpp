//
// Created by daniel on 20/04/2022.
//

#include "FaNotifyHandler.h"
#include "FileSystemMonitor.h"

int main() {

    // create paths for monitored files
    std::filesystem::path monitored_file1("test_file_deny.txt");

    // create set of monitored files
    std::set<std::filesystem::path> monitored_files;
    monitored_files.insert(absolute(monitored_file1));

    //create queues for fanotify handler and file system monitor
    FanAsyncQueue<EventItem> event_q;
    FanAsyncQueue<struct fanotify_response> reply_q;

    // create and start FaNotifyHandler
    FaNotifyHandler fan_handler(monitored_files, event_q, reply_q);
    fan_handler.start();

    // create paths for tester programs
    std::filesystem::path try_to_open("try_to_open");

    // create a set for tester programs
    std::set<std::filesystem::path> tester_programs;
    tester_programs.insert(absolute(try_to_open));

    // create and start FileSystemMonitor
    FileSystemMonitor FS_mon(monitored_files, tester_programs, event_q, reply_q);
    FS_mon.start();

    fan_handler.wait();
    FS_mon.wait();

    return 0;
}
