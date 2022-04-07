//
// Created by daniel on 07/04/2022.
//

#ifndef CPP_TRAINING_NEW_CONCURRENCY_H
#define CPP_TRAINING_NEW_CONCURRENCY_H

#include <future>
#include <iostream>
#include <fstream>
#include <sstream>

void read_file(std::string& filename, std::promise<std::stringstream>& p) {
    // ifstream init
    std::ifstream in;
    in.exceptions(std::ios::failbit | std::ios::badbit);
    try {
        in.open(filename);
    } catch (std::ios::failure e){
        std::cout << e.what() << std::endl;
        p.set_exception(std::current_exception());
        return;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    p.set_value(std::move(buffer));
}

void concurrency_exe_tester() {
    std::string filename;

    std::cout << "please enter file name : ";
    std::cin >> filename;

    std::promise<std::stringstream> p;
    std::future<std::stringstream> f(p.get_future());

    std::thread t1{read_file, std::ref(filename), std::ref(p)};

    t1.join();

    try {
        std::cout << "content of the file : " << std::endl << f.get().str() << std::endl;
    } catch (std::ios::failure) {
        std::cout << "Ooopsi, failed getting contents of the file" << std::endl;
    }
}

#endif //CPP_TRAINING_NEW_CONCURRENCY_H
