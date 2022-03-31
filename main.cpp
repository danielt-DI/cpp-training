#include <iostream>
#include "filehandler.h"
int main() {

    char to_print[] = "Some test text";
    char buff[10];
    fs::path p("/home/daniel/CLionProjects/cpp_training_new/test_file.txt");
    file_handler fh;
    fh.open(p, std::ios::out | std::ios::in | std::ios::trunc);

    // state bits :
    std::cout << "goodbit : " << fh.good()
                << ", eofbit : " << fh.eof()
                << ", failbit : " << fh.fail()
                << ", badbit : " << fh.bad()
                << ", exception mask : " << fh.exceptions()
                << std::endl;

    fh.seekp(0);
    fh.write(to_print, sizeof(to_print) - 1);
    fh.seekg(5);
    fh.read(buff, 4);
    buff[4] = '\0';

    std::cout << buff << std::endl;

    std::cout << "Finished" << std::endl;

    return 0;
}
