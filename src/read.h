#ifndef READ_H
#define READ_H

#include <string>

bool is_parentheses_balanced(const std::string& input);
bool is_blockComment_balanced(const std::string& input);
std::string readInput();

void REPLmode();
void filemode(const std::string& input);

#endif