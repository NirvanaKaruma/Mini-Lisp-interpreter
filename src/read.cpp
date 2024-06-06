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

bool is_blockComment_balanced(const std::string& input){
    auto pos = input.find("#|");
    if (pos == std::string::npos) return true;
        auto endPos = input.find("|#", pos);
    if (endPos == std::string::npos) {
        return false;
    } else {
        return true;
    }       
}

void deleteLineComment(std::string& input){
    auto pos = input.find(";");
    if (pos == std::string::npos) return;
    input.erase(pos);
}

void deleteBlockComment(std::string& input){
    auto pos = input.find("#|");
    if (pos == std::string::npos) return;
    auto endPos = input.find("|#", pos);
    if (endPos == std::string::npos) {
        throw FileError("Block comment is not closed");
        std::exit(0);
    }
    input.erase(pos, endPos - pos + 2);
}

void deletenewline(std::string& input){
    while(input.find("\n") != std::string::npos){
        auto pos = input.find("\n");
        input.erase(pos, 1);
    }
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
            deleteLineComment(line);
            deletenewline(line);
            if(line.empty()) continue;
            //  缩进规则
            int newIndent = std::count(line.begin(), line.end(), '(') - std::count(line.begin(), line.end(), ')');
            indentLevel += newIndent;

            input += line;
            if(!is_blockComment_balanced(input)) continue;
            deleteBlockComment(input);
            if(!is_parentheses_balanced(input)) {
                input += " ";
                continue;
            }
            if(input.empty()) continue;
            auto result = evaluate(input, *env);
            std::cout << result->toString() << std::endl;
            input.clear();
            indentLevel = 0;
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
            deleteLineComment(line);
            if(line.empty()) continue;
            input += line;
            if(!is_blockComment_balanced(input)) continue;
            deleteBlockComment(input);
            if(!is_parentheses_balanced(input)) {
                input += " ";
                continue;
            }
            if(!input.empty()){
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

std::string readInput(){
    std::string input;
    std::getline(std::cin, input);
    while(input.empty()){
        std::cout << "Please enter a non-empty string: ";
        std::getline(std::cin, input);
    }
    return input;
}