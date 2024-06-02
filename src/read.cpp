
#include "./read.h"
#include <fstream>
#include <iostream>

#include "./error.h"
#include "./eval_env.h"
#include "./parser.h"

#include "./tokenizer.h"
#include "./value.h"
#include "./rational.h"

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

void printIndented(const std::string& line, int indentLevel){    
    std::cout << line;
    for(int i = 0; i < indentLevel; ++i){
        std::cout << "  ";
    }
}

void REPLmode(){
    std::shared_ptr<EvalEnv> env{new EvalEnv};
    std::string input="";
    int indentLevel = 0;
    while(true){
        try {
            if(is_parentheses_balanced(input))  printIndented(">>> ", indentLevel);
            else printIndented("... ", indentLevel);
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            int newIndent = std::count(line.begin(), line.end(), '(');
            input += line;
            indentLevel += newIndent - std::count(line.begin(), line.end(), ')');
            if(!is_parentheses_balanced(input)){
                continue;
            }
            auto result = evaluate(input, *env);
            std::cout << result->toString() << std::endl;
            input.clear();
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            input.clear();
            indentLevel = 0;
        }        
    }
}

void filemode(const std::string& filename) {
    std::shared_ptr<EvalEnv> env{new EvalEnv};
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileError("File not found");
        std::exit(0);
    }

    std::string input;
    std::string line;

    while(std::getline(file, line)){
        try {
            input += line;
            if(is_parentheses_balanced(input)){
                auto result = evaluate(input, *env);
                input.clear();
            }
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            input.clear();
        }
    }

    file.close();
}