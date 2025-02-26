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
                data.resize(buf.size() - 1); 

               
                std::string result = checkString(data);
                if (result == "true") {
                    std::cout << "Success: Received data - " << data << std::endl;
                }
                else {
                    std::cout << "Error: Invalid data - " << data << std::endl;
                }
            }
            catch (...) {
                std::cerr << "Data processing error" << std::endl;
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
    return 0;
}
