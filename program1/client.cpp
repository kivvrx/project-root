#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/asio.hpp>
#include "lib.h"

using namespace boost::asio;
using namespace ip;

std::mutex buffer_mutex;
std::condition_variable buffer_cv;
std::string shared_buffer;
bool data_ready = false;
bool processing_done = true; 

void inputThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(buffer_mutex);

        buffer_cv.wait(lock, [] { return processing_done; });

        std::string input;
        std::cout << "Enter a string: ";
        std::getline(std::cin, input);


        if (input.empty() || input.length() > 64 || !std::all_of(input.begin(), input.end(), ::isdigit)) {
            std::cerr << "Input error!" << std::endl;
            continue;
        }

        processString(input); 

        shared_buffer = input;
        data_ready = true;
        processing_done = false; 
        lock.unlock();
        buffer_cv.notify_one(); 
    }
}

void processingThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(buffer_mutex);
        buffer_cv.wait(lock, [] { return data_ready; });

        std::string data = shared_buffer;
        shared_buffer.clear();
        data_ready = false;
        lock.unlock();

        std::cout << "Processed data: " << data << std::endl;

        int sum = calculateSum(data);
        std::cout << "Sum of numeric values: " << sum << std::endl; 

        try {
            io_context io;
            tcp::socket socket(io);
            tcp::resolver resolver(io);
            connect(socket, resolver.resolve("127.0.0.1", "1234"));


            write(socket, buffer(std::to_string(sum) + "\n"));
        }
        catch (std::exception& e) {
            std::cerr << "Connection error: " << e.what() << std::endl;
        }


        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            processing_done = true;
        }
        buffer_cv.notify_one();
    }
}

int main() {
    std::thread t1(inputThread);
    std::thread t2(processingThread);
    t1.join();
    t2.join();
    return 0;
}
