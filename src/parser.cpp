#include "./parser.h"
#include "./error.h"
Parser::Parser(std::deque<TokenPtr> token) : tokens(std::move(token)) {}


ValuePtr Parser::parse(){
    if (tokens.empty()) {
        throw SyntaxError("Unexpected end of input");
    }

    auto token = std::move(tokens.front());
    tokens.pop_front();     // 获取需要解析的token后立刻丢弃列表中该元素

    if (token->getType() == TokenType::NUMERIC_LITERAL){
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }

    if (token->getType() == TokenType::BOOLEAN_LITERAL){
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }

    if (token->getType() == TokenType::STRING_LITERAL){
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }

    if (token->getType() == TokenType::IDENTIFIER){
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    }
    
    if (token->getType() == TokenType::LEFT_PAREN){
        return this->parseTails();
    }

    if (token->getType() == TokenType::QUOTE) {
        return handleQuote(TokenType::QUOTE);
    }

    if (token->getType() == TokenType::QUASIQUOTE) {
        return handleQuote(TokenType::QUASIQUOTE);
    }

    if (token->getType() == TokenType::UNQUOTE) {
        return handleQuote(TokenType::UNQUOTE);
    }

    throw SyntaxError("Unimplemented");
}

ValuePtr Parser::parseTails(){

    // 如果是右括号，则返回空表
    if (tokens.front()->getType() == TokenType::RIGHT_PAREN){
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    
    // 如果不是右括号，则递归解析
    auto car = this->parse();

    if (tokens.empty()) {
        throw SyntaxError("Unexpected end of input");
    }

    // 如果接下来是点，则返回一个对子
    if (tokens.front()->getType() == TokenType::DOT){
        tokens.pop_front();
        auto cdr = this->parse();
        // 检测下一个token是否是右括号，如果不是则报错
        if (tokens.front()->getType() != TokenType::RIGHT_PAREN){
            throw SyntaxError("Unexpected token, expected for ')'");
        }
        tokens.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    } 
    // 如果接下来不是点，则递归解析
    else {
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}

// 处理引号的辅助函数
ValuePtr Parser::handleQuote(TokenType quoteType) {
    // 解析下一个标记
    auto quotedValue = this->parse();
    // 创建一个表示引号类型的 SymbolValue
    std::string symbolName;
    switch (quoteType) {
        case TokenType::QUOTE:
            symbolName = "quote";
            break;
        case TokenType::QUASIQUOTE:
            symbolName = "quasiquote";
            break;
        case TokenType::UNQUOTE:
            symbolName = "unquote";
            break;
        default:
            throw SyntaxError("Unimplemented");
    }
    auto quoteSymbol = std::make_shared<SymbolValue>(symbolName);
    // 创建一个表示列表的 PairValue
    return std::make_shared<PairValue>(quoteSymbol, std::make_shared<PairValue>(quotedValue, std::make_shared<NilValue>()));
}
