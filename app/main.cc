#include <iostream>
#include <protection/RingBuffer.h>

int main(int argc, char const *argv[])
{


    RingBuffer<int> bf(3);

    bf.tryPush(5);
    bf.tryPush(5);
    bf.tryPush(5);
    if(bf.tryPush(5)) std::cout << "error, full\n";

    for (int i = 0; i < 3; i++)
    {
        std::cout << bf.tryPop().value() << " ";
    }
    
    std::cout << "\n";

    return 0;
}
