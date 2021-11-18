#include <iostream>

    extern "C" {
        int squareNum(int x) {
            return x * x;
        }
        void sayHello()
        {
            std::cout<<"Hello";
        }
    }