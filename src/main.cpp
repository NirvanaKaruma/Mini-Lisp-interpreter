#include <iostream>
#include <string>
#include <stack>

#include "./tokenizer.h"
#include "./parser.h"
#include "./eval_env.h"
#include "./forms.h"
#include "./file_input.h"
#include "./read.h"
#include "./rational.h"

#include "rjsj_test.hpp"

struct TestCtx {
    std::shared_ptr<EvalEnv> env{new EvalEnv};
    std::string eval(std::string input) {        
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};

int main(int argc, char** argv) {
    //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv6, Lv7, Lv7Lib, Sicp);
    //usage : mini_lisp [file]
    switch (argc) {
        case 1 : 
            REPLmode();
            break;
        case 2 :
            filemode(argv[1]);
            break;
        default :
            std::cout << "usage : mini_lisp [file]" << std::endl;
            break;    
    }
    return 0;
}
