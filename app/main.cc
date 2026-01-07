#include <iostream>
#include <protection/SafeSocket.h>

int main(int argc, char const *argv[])
{
    SafeSocket sock("127.0.0.1", 9000);

    sock.sendString("Hello server\n");

    std::string line;
    if (sock.recvString(line)) {
        std::cout << "Received: " << line << "\n";
    }

    
    return 0;
}
