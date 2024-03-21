#include <iostream>
#include <thread>
#include <mutex>

std::vector<std::mutex> forks(5);

const int n = 5;

void ZeroEvenOdd() {
    forks[2].lock();
    forks[1].lock();
}

void printNumber(const int& value) {
    std::cout << value;
    std::cout.flush();
}

// printNumber(x) outputs "x", where x is an integer.
void zero() {
    for (int i = 0; i < n; ++i) {
        forks[0].lock();
        printNumber(0);
        if (i % 2 == 0) {
            forks[1].unlock();
        } else {
            forks[2].unlock();
        }
    }
}

void even() {
    for (int i = 2; i <= n; i += 2) {
        forks[2].lock();
        printNumber(i);
        forks[0].unlock();
    }
}

void odd() {
    for (int i = 1; i <= n; i += 2) {
        forks[1].lock();
        printNumber(i);
        forks[0].unlock();
    }
}

int main() {

    ZeroEvenOdd();

    std::thread z(zero);
    std::thread o(odd);
    std::thread e(even);

    z.join();
    o.join();
    e.join();

    return 0;
}