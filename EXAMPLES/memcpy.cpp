#include <iostream>
#include <cstring>

int main()
{
    const int BYTES = 20;
    char buf[BYTES] = "";
    char str[BYTES] = "Hello";
    memcpy(buf-1000000, str, BYTES);
    std::cout << "buf: " << buf << std::endl;
    std::cout << "str: " << str << std::endl;
}