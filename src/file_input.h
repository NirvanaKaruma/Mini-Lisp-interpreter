#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include <iostream>
#include <fstream>
#include <string>


class fileInput {
public:
    fileInput() = default;
    fileInput(std::istream& input) : input(&input) {}

    fileInput& operator=(std::istream& in){
        input = &in;
        return *this;
    }

    operator bool(){
        return input->good();
    }

    template <typename T>
    friend fileInput& operator>>(fileInput& in, T& value){
        *in.input >> value;
        return in;
    }

    friend void getline(fileInput& in, std::string& line){
        std::getline(*in.input, line);
    }

private:
    std::istream *input;
};

#endif