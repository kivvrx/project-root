#include <iostream>
#include <boost/asio.hpp>
#include "lib.h"

using namespace boost::asio;
using namespace ip;


int main() {
    try {
        std::locale::global(std::locale(""));
        std::cout.imbue(std::locale());
        std::cin.imbue(std::locale());
        io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 1234));

        while (true) {
            tcp::socket socket(io);
            acceptor.accept(socket);

            try {
                streambuf buf;
                read_until(socket, buf, '\n');
                std::string data = buffer_cast<const char*>(buf.data());
                data.resize(buf.size() - 1); // Убираем '\n'

                // Передаем полученные данные в функцию checkString
                std::string result = checkString(data);
                if (result == "истина") {
                    std::cout << "Успех: Полученные данные - " << data << std::endl;
                }
                else {
                    std::cout << "Ошибка: Некорректные данные - " << data << std::endl;
                }
            }
            catch (...) {
                std::cerr << "Ошибка обработки данных" << std::endl;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Ошибка сервера: " << e.what() << std::endl;
    }
    return 0;
}

