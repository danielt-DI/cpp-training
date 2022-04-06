#include <iostream>
#include "filehandler.h"
int main() {

    char to_print[] = "Some test text";
    char buff[10] = {0};
    file_handler fh;

    // open failure test (user does not have write permissions)
    fs::path p_open_failure("no_w_permissions.txt");
    fh.open(p_open_failure, std::ios::out);
    std::cout << "is open : " << fh.is_open() << std::endl;
    fh.seekp(0);
    fh.write(to_print, sizeof(to_print) - 1);
    fh.seekg(5);
    fh.read(buff, 4);
    std::cout << "buff : " << buff << std::endl;

    // open success
    fs::path p_success("test_file.txt");
    fh.open(p_success, std::ios::out | std::ios::in | std::ios::trunc);
    std::cout << "is open : " << fh.is_open() << std::endl;

    // write test
    fh.seekp(0);
    fh.write(to_print, sizeof(to_print) - 1);

    // read test
    fh.seekg(5);
    fh.read(buff, 4);
    buff[4] = '\0';
    std::cout << "buff : " << buff << std::endl;

    std::cout << "Finished" << std::endl;

    return 0;
}
