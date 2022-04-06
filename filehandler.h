//
// Created by daniel on 29/03/2022.
//

#ifndef CPP_TRAINING_NEW_FILEHANDLER_H
#define CPP_TRAINING_NEW_FILEHANDLER_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
namespace fs = std::filesystem;

class file_handler {
    std::fstream fh_fstream;

public:
    // constructor
    file_handler();

    // destructor
    virtual ~file_handler() { if(fh_fstream.is_open()) fh_fstream.close();}

    // file handler interface
    void open(const fs::path& path,std::ios_base::openmode mode = std::ios_base::in|std::ios_base::out);
    bool is_open() { return fh_fstream.is_open();};
    void close();
    void read(char* buff, std::streamsize count);
    void write(const char* buff, std::streamsize count);
    int sync() {return fh_fstream.sync();}

    // positioning
    void seekg(std::ios::pos_type pos);
    void seekp(std::ios::pos_type pos);

    std::ostream& operator<<(const char* s) {fh_fstream << s << std::endl; return fh_fstream;}
};

file_handler::file_handler() {
    fh_fstream.exceptions(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
}

void file_handler::open(const fs::path& path, std::ios::openmode mode) {
    try {
        fh_fstream.open(path.string(), mode);
    } catch (std::ios::failure& e) {
        std::cout << "could not open file, exception caught and handled" << std::endl;
        fh_fstream.clear();
    } catch (...) {
        std::cout << "unexpected exception" << std::endl;
        throw;
    }

}

void file_handler::close() {
    try {
        fh_fstream.close();
    } catch (std::ios::failure& e) {
        std::cout << "could not close file, exception caught and handled" << std::endl;
        fh_fstream.clear();
    } catch (...) {
        std::cout << "unexpected exception" << std::endl;
        throw;
    }
}

void file_handler::write(const char *buff, std::streamsize count) {
    try {
        fh_fstream.write(buff, count);
    } catch (std::ios::failure& e) {
        std::cout << "could not write to file, exception caught and handled" << std::endl;
        fh_fstream.clear();
    } catch (...) {
        std::cout << "unexpected exception" << std::endl;
        throw;
    }

}

void file_handler::read(char *buff, std::streamsize count) {
    try{
        fh_fstream.read(buff, count);
    } catch (std::ios::failure& e) {
        std::cout << "could not write to file, exception caught and handled" << std::endl;
        fh_fstream.clear();
    } catch (...) {
        std::cout << "unexpected exception" << std::endl;
        throw;
    }
}

void file_handler::seekg(std::ios::pos_type pos) {
    try{
        fh_fstream.seekg(pos);
    } catch (std::ios::failure& e) {
        std::cout << "could not seek-get in file, exception caught and handled" << std::endl;
        fh_fstream.clear();
    } catch (...) {
        std::cout << "unexpected exception" << std::endl;
        throw;
    }

}

void file_handler::seekp(std::ios::pos_type pos) {
    try{
        fh_fstream.seekp(pos);
    } catch (std::ios::failure& e) {
        std::cout << "could not seek-put in file, exception caught and handled" << std::endl;
        fh_fstream.clear();
    } catch (...) {
        std::cout << "unexpected exception" << std::endl;
        throw;
    }
}

#endif //CPP_TRAINING_NEW_FILEHANDLER_H
