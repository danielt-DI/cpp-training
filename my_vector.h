//
// Created by daniel on 29/03/2022.
//

#ifndef CPP_TRAINING_NEW_MY_VECTOR_H
#define CPP_TRAINING_NEW_MY_VECTOR_H

#include <iostream>
#include <memory>
#include <algorithm>

template <typename T>
class my_vector {
    std::unique_ptr<T[]> buff;
    int vec_capacity;
    int vec_size;

public:
    // constructor
    my_vector(int capacity = 1): vec_capacity(capacity), vec_size(0), buff(std::make_unique<T[]>(capacity)) {}

    // copy constructor
    my_vector(const my_vector& vec);

    // copy assigment operator
    my_vector& operator=(const my_vector& vec);

    // move constructor
    my_vector(my_vector&& vec):vec_size(vec.vec_size), vec_capacity(vec.vec_capacity), buff(std::move(vec.buff)) {}

    // move assignment operator
    my_vector& operator=(const my_vector&& vec);

    // destructor
    //virtual ~my_vector(); - removed, buff is now a smart pointer.

    // vector interface
    int size() const {return vec_size;}
    int capacity() const {return vec_capacity;}
    void push_back(T elem);
    void print();
};


// copy constructor
template <typename T>
my_vector<T>::my_vector(const my_vector &vec): vec_capacity(vec.vec_capacity), vec_size(vec.vec_size), buff(std::make_unique<T[]>(vec.vec_capacity)) {
    std::copy(vec.buff.get(), std::next(vec.buff.get(), vec.vec_size), buff.get());
}

// copy assignment operator
template <typename T>
my_vector<T>& my_vector<T>::operator=(const my_vector &vec) {
    vec_size = vec.vec_size;
    vec_capacity = vec.vec_capacity;
    buff.reset();
    buff = std::make_unique<T[]>(vec.vec_capacity);

    std::copy(vec.buff.get(), std::next(vec.buff.get(), vec.vec_size), buff.get());
}

// move assignment operator
template <typename T>
my_vector<T>& my_vector<T>::operator=(const my_vector &&vec) {
    vec_size = vec.vec_size;
    vec_capacity = vec.vec_capacity;
    buff = vec.buff.release();
}

// vector interface
template <typename T>
void my_vector<T>::push_back(T elem) {
    if(vec_size == vec_capacity) {
        vec_capacity *= 2;
        std::unique_ptr<T[]> tmp = std::make_unique<T[]>(vec_capacity);
        std::copy(buff.get(), std::next(buff.get(), vec_size), tmp.get());
        buff.reset();
        buff = std::move(tmp);
    }

    buff[vec_size++] = elem;
}

template <typename T>
void my_vector<T>::print() {
    for(int i = 0; i < vec_size; i++) {
        std::cout << buff[i] << " ";
    }
    std::cout << std::endl;
}


#endif //CPP_TRAINING_NEW_MY_VECTOR_H