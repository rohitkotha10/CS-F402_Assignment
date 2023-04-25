#include <chrono>
#include <iostream>
#include <string>

class Timer {
public:
    void start(std::string name);
    void display();

private:
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point curTime;
    std::string name;
};

void Timer::start(std::string name) {
    this->name = name;
    startTime = std::chrono::system_clock::now();
}
void Timer::display() {
    curTime = std::chrono::system_clock::now();
    std::chrono::duration<float> dur = curTime - startTime;
    std::cout << name << " Completed in " << dur.count() << "s" << std::endl;
}
