//
// Created by daniel on 07/04/2022.
//

#include <unistd.h>
#include <iostream>
#include <climits>
#include "FaNotifyHandler.h"

FaNotifyHandler::FaNotifyHandler(std::set<std::filesystem::path>& paths,
                                 FanAsyncQueue<EventItem>& event_q,
                                 FanAsyncQueue<struct fanotify_response>& reply_q) : event_q(event_q), reply_q(reply_q) {
    // init fanotify instance
    fan_fd = fanotify_init(FAN_CLASS_CONTENT | FAN_NONBLOCK, O_RDONLY);
    if(fan_fd == -1) {
        throw_errno("failed to initialize fanotify");
    }

    // mark provided files to notify on open and ask for permissions
    for(std::filesystem::path p : paths) {
        if(fanotify_mark(fan_fd, FAN_MARK_ADD, FAN_OPEN_PERM, AT_FDCWD, absolute(p).c_str())) {
            throw_errno("failed to mark " + p.string());
        }
    }

    // create an epoll instance
    ep_fd = epoll_create(1);
    if(ep_fd == -1) {
        throw_errno("failed to create epoll instance");
    }
}

FaNotifyHandler::~FaNotifyHandler() {
    // stop handler thread
    if(is_running) stop();

    // close file descriptors
    close(fan_fd);
    close(ep_fd);
}

void FaNotifyHandler::start() {
    std::cout << "starting fanotify handler" << std::endl;
    // lunch polling thread
    is_running = 1;
    t = std::thread(&FaNotifyHandler::poll_fanotify_fd, this);
}

void FaNotifyHandler::stop() {
    is_running = 0;
    t.join();
    std::cout << "fanotify handler stopped" << std::endl;
}

// private definitions

void FaNotifyHandler::throw_errno(const std::string& msg) {
    std::cout << msg << std::endl;
    throw std::system_error(std::make_error_code((std::errc)errno));
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
// event handler thread
void FaNotifyHandler::poll_fanotify_fd() {
    unsigned int nfds;
    struct epoll_event ev = {0};

    // add fanotify file descriptor to epoll instance
    ev.data.fd = fan_fd;
    ev.events = EPOLLIN | EPOLLOUT;
    if(epoll_ctl(ep_fd, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1) {
        throw_errno("failed to add fanotify file descriptor to epoll interest list");
    }

    while(is_running) {
        // wait for new events
        nfds = epoll_wait(ep_fd, re_events, MAX_EVENTS, 5000); // block indefinitely

        // handle errno
        if(nfds == -1) {
            std::cout << "error occured" << std::endl;
            if(errno == EINTR) continue; // interrupted by a signal
            if(errno == EBADF) break; // ep_fd was probably closed due to fanotify handler destruction
            throw_errno("epoll_wait failed");
        } else {
            // read fanotify events
            if (nfds > 0) {
                // loop over all file descriptors (right now only fan_fd)
                for(int i = 0; i <  nfds; i++) {
                    if(re_events[i].events & EPOLLIN) {
                        read_fan_events(re_events[i].data.fd);
                    }
                }
            // handle replies if no new events
            } else if (!reply_q.empty()){
                handle_fan_replies();
            }
        }
    }
}

void FaNotifyHandler::read_fan_events(int fd) {
    const struct fanotify_event_metadata *metadata;
    struct fanotify_event_metadata buff[MAX_EVENTS];
    ssize_t len;
    char path[PATH_MAX];
    ssize_t path_len;
    char procfd_path[PATH_MAX];

    while(is_running) {
        // read events
        len = read(fd, buff, sizeof(buff));
        if(len == -1 && errno != EAGAIN) {
            throw_errno("could not read from fanotify fd");
        }

        if(len == -1) {
            break;
        }

        metadata = buff;

        // push all available events to queue
        while(FAN_EVENT_OK(metadata, len)) {
            if (metadata->vers != FANOTIFY_METADATA_VERSION) {
                throw_errno("Mismatch of fanotify metadata version");
            }

            // for now ignore queue overflow (does not set errno, need to decide what exception to throw)

            if ((metadata->fd >= 0) && (metadata->mask & FAN_OPEN_PERM)) {
                // create EventItem and push to queue
                event_q.push(EventItem{metadata->fd, metadata->pid});
            }

            metadata = FAN_EVENT_NEXT(metadata, len);
        }
    }
}

void FaNotifyHandler::handle_fan_replies() {
    ssize_t  res;

    // get response
    struct fanotify_response response = reply_q.wait_pop();

    // write response
    res = write(fan_fd, &response, sizeof(response));
    if(res == -1) {
        throw_errno("failed to write to fanotify fd");
    }

    // close event's file descriptor
    close(response.fd);
}

#pragma clang diagnostic pop

void FaNotifyHandler_tester() {
    std::filesystem::path test_file("test_file.txt");
    std::set<std::filesystem::path> paths;
    paths.insert(test_file);

    // create queues first

    //FaNotifyHandler fan_handler(paths);
    //fan_handler.start();
    std::cout << "press enter to stop ... ";
    std::cin.get();
    //fan_handler.stop();
}
