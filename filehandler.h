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
    file_handler(fs::path p) {fh_fstream.open(p.string(), std::ios::out | std::ios::in | std::ios::trunc);}

    // destructor
    virtual ~file_handler() { if(fh_fstream.is_open()) {fh_fstream.close();}}

    // file handler interface

    void open(const std::string &filename,std::ios_base::openmode mode = std::ios_base::in|std::ios_base::out) {fh_fstream.open(filename, mode);}
    bool is_open() { return fh_fstream.is_open();};
    void close() {fh_fstream.close();}
    void read(char* buff, std::streamsize count) {fh_fstream.read(buff, count);}
    void write(const char* buff, std::streamsize count) {fh_fstream.write(buff, count);}
    int sync() {return fh_fstream.sync();}

    // positioning
    void seekg(std::ios::pos_type pos) {fh_fstream.seekg(pos);}
    void seekg(std::ios::off_type off, std::ios_base::seekdir dir) {fh_fstream.seekg(off, dir);};
    void seekp(std::ios::pos_type pos) {fh_fstream.seekp(pos);}
    void seekp(std::ios::off_type off, std::ios_base::seekdir dir) {fh_fstream.seekp(off, dir);}

    // state
    bool good() const {return fh_fstream.good();}
    bool eof() const {return fh_fstream.eof();}
    bool fail() const {return fh_fstream.fail();}
    bool bad() const {return fh_fstream.bad();}
    bool operator!() const {return !fh_fstream;}

    std::ostream& operator<<(const char* s) {fh_fstream << s << std::endl; return fh_fstream;}
};

#endif //CPP_TRAINING_NEW_FILEHANDLER_H
