#include <fstream>
#include <iostream>
#include <string>

using std::string;

char *importVertexShader(string shadername)
{
    std::fstream source;
    source.open("shaders/" + shadername + ".vs", std::ios::in);

    if (source.is_open())
    {
        std::string contents((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
        char *arr = new char[contents.length()];
        return strcpy(arr, contents.c_str());
    }

    std::cout << shadername << " NOT FOUND" << std::endl;
    return nullptr;
}

char *importFragmentShader(string shadername)
{
    std::fstream source;
    source.open("shaders/" + shadername + ".fs", std::ios::in);

    if (source.is_open())
    {
        std::string contents((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
        char *arr = new char[contents.length()];
        return strcpy(arr, contents.c_str());    }

    std::cout << shadername << " NOT FOUND" << std::endl;
    return nullptr;
}