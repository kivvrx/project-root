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
bool processing_done = true; // Флаг завершения обработки

void inputThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(buffer_mutex);
        // Ждем, пока обработка предыдущих данных завершится
        buffer_cv.wait(lock, [] { return processing_done; });

        std::string input;
        std::cout << "Введите строку: ";
        std::getline(std::cin, input);

        // Проверка ввода (оставляем в основном коде)
        if (input.empty() || input.length() > 64 || !std::all_of(input.begin(), input.end(), ::isdigit)) {
            std::cerr << "Ошибка ввода!" << std::endl;
            continue;
        }

        processString(input); // Используем функцию из библиотеки

        shared_buffer = input;
        data_ready = true;
        processing_done = false; // Обработка началась
        lock.unlock();
        buffer_cv.notify_one(); // Уведомляем поток обработки
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

        std::cout << "Обработанные данные: " << data << std::endl;

        int sum = calculateSum(data);
        std::cout << "Сумма числовых значений: " << sum << std::endl; // Вывод суммы на экран

        try {
            io_context io;
            tcp::socket socket(io);
            tcp::resolver resolver(io);
            connect(socket, resolver.resolve("127.0.0.1", "1234"));

            // Передаем сумму в виде строки в Программу №2
            write(socket, buffer(std::to_string(sum) + "\n"));
        }
        catch (std::exception& e) {
            std::cerr << "Ошибка подключения: " << e.what() << std::endl;
        }

        // Уведомляем, что обработка завершена
        {
            std::lock_guard<std::mutex> lock(buffer_mutex);
            processing_done = true;
        }
        buffer_cv.notify_one();
    }
}




int main() {
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());
    std::cin.imbue(std::locale());
    std::thread t1(inputThread);
    std::thread t2(processingThread);
    t1.join();
    t2.join();
    return 0;
}