#ifndef PARSER_H
#define PARSER_H

#include <deque>
#include <memory>
#include <string>
#include "./token.h"
#include "./value.h"

using TokenPtr = std::unique_ptr<Token>;

class Parser {
public:
    Parser(std::deque<TokenPtr> token);
    ValuePtr parse();
    
private:
    std::deque<TokenPtr> tokens;
    ValuePtr parseTails();
    ValuePtr handleQuote(TokenType quoteType);

};


#endif