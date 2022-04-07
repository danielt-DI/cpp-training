//
// Created by daniel on 17/04/2022.
//

#include <iostream>
#include <climits>
#include <unistd.h>
#include <fcntl.h>
#include "FileSystemMonitor.h"

FileSystemMonitor::FileSystemMonitor(std::set<std::filesystem::path>& files_paths,
                                     std::set<std::filesystem::path>& exec_paths,
                                     FanAsyncQueue<EventItem>& event_q,
                                     FanAsyncQueue<struct fanotify_response>& reply_q):
                                     event_q(event_q), reply_q(reply_q), files_paths(files_paths), exec_paths(exec_paths) {

}

FileSystemMonitor::~FileSystemMonitor() {
    if(is_running) stop();
}

void FileSystemMonitor::start() {
    std::cout << "starting file system monitor" << std::endl;
    // lunch monitor thread
    is_running = 1;
    t = std::thread(&FileSystemMonitor::monitor_events, this);
}

void FileSystemMonitor::stop() {
    is_running = 0;
    t.join();
    std::cout << "file system monitor stopped" << std::endl;
}

// private definitions

void FileSystemMonitor::throw_errno(const std::string& msg) {
    std::cout << msg << std::endl;
    throw std::system_error(std::make_error_code((std::errc)errno));
}

void FileSystemMonitor::monitor_events() {
    char path[PATH_MAX] = {0}, proc_name[128] = {0};
    ssize_t path_len, proc_name_len;
    int pinfo;
    char procfd_file_path[PATH_MAX], procfd_proc_path[PATH_MAX];

    while(is_running) {
        EventItem event = event_q.wait_pop();

        // get file path
        snprintf(procfd_file_path, sizeof(procfd_file_path), "/proc/self/fd/%d", event.fd);
        path_len = readlink(procfd_file_path, path, sizeof(path) - 1);
        if (path_len == -1) {
            throw_errno("readlink error");
        }

        std::cout << "got a new event on " << path << std::endl;

        // get process name
        sprintf(procfd_proc_path, "/proc/%d/cmdline", event.pid);
        pinfo = open(procfd_proc_path, O_RDONLY);
        if (pinfo) {
            proc_name_len = read(pinfo, proc_name, 128);
            if (proc_name_len <= 0) {
                throw_errno("could not read process name from /proc");
            }
        }

        std::cout << "event caused by: " << proc_name << std::endl;

        // cross reference the path in the event with lists
        auto file_iter = files_paths.find(path);
        auto proc_iter = exec_paths.find(proc_name);
        if ((file_iter != files_paths.end()) && (proc_iter == exec_paths.end())) {
            // deny
            std::cout << "access denied" << std::endl;
            reply_q.push({event.fd, FAN_DENY});
        } else {
            // approve
            std::cout << "access approved" << std::endl;
            reply_q.push({event.fd, FAN_ALLOW});
        }
    }
}
