
#include "./read.h"
#include <fstream>
#include <iostream>

#include "./error.h"
#include "./eval_env.h"
#include "./parser.h"

#include "./tokenizer.h"
#include "./value.h"
#include "./file_input.h"

#include <stack>
#include <string>

ValuePtr evaluate(std::string input, EvalEnv& env){
    auto tokens = Tokenizer::tokenize(input);
    Parser parser(std::move(tokens));
    auto value = parser.parse();
    return env.eval(std::move(value));
}

bool is_parentheses_balanced(const std::string& input){
    std::stack<char> stack;
    for(char c : input){
        switch (c)
        {
        case '(':
            stack.push(c);
            break;
        case ')':
            if(stack.empty()){
                throw FileError("Parentheses are not balanced");
                std::exit(0);
            }
            stack.pop();
            break;
        default:
            break;
        }
    }
    return stack.empty();
}

void REPLmode(){
    std::shared_ptr<EvalEnv> env{new EvalEnv};
    std::string input="";
    while(true){
        try {
            if(is_parentheses_balanced(input)) std::cout << ">>> ";
            else std::cout << "... ";
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            input += line;
            if(!is_parentheses_balanced(input)){
                continue;
            }
            auto result = evaluate(input, *env);
            std::cout << result->toString() << std::endl;
            input.clear();
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            input.clear();
        }        
    }
}

void filemode(const std::string& input){
    std::shared_ptr<EvalEnv> env{new EvalEnv};
    fileInput in;
    std::ifstream file;
    file.open(input);
    if(!file.is_open()){
        throw FileError("File not found");
        std::exit(0);
    }
    in = file;
    while(in){
        try{
            std::string line;
            getline(in, line);
            auto result = evaluate(line, *env);
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    file.close();
}