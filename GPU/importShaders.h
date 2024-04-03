#include <fstream>
#include <iostream>
#include <string>

using std::string;

char *importShader(string shadername)
{
    std::fstream source;
    source.open("shaders/" + shadername, std::ios::in);

    if (source.is_open())
    {
        std::string contents((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
        char *arr = new char[contents.length()];
        return strcpy(arr, contents.c_str());
    }

    std::cout << shadername << " NOT FOUND" << std::endl;
    return nullptr;
}