#include <iostream>
#include "my_vector.h"

struct my_struct {
    std::string str;
    int x;

    friend std::ostream& operator<<(std::ostream& os, const my_struct& s) {
        os << s.str << " " << s.x;
        return os;
    }
};

int main() {
    my_vector<int> intv(3);
    std::cout << "Capacity : " << intv.capacity() << std::endl;
    intv.push_back(5);
    intv.push_back(5);
    intv.push_back(5);
    intv.push_back(5);
    std::cout << "Capacity : " << intv.capacity() << std::endl;
    intv.print();

    std::cout << std::endl;

    my_vector intvv(intv);
    intvv.push_back(10);
    intvv.print();
    intv.print();

    std::cout << std::endl;

    my_vector<std::string> strv(2);
    strv.push_back("Hello");
    strv.push_back("World");
    strv.push_back("!!");
    strv.print();

    std::cout << std::endl;

    my_vector<my_struct> structv;
    structv.push_back({"my age is : ", 3});

    structv.print();
    std::cout << std::endl;


    std::cout << "Finished" << std::endl;

    return 0;
}
