#include <iostream>
#include <string>
#include "BET.h"

int main() {
    BET tree;
    std::string postfix;

    std::cout << "Enter postfix expression (or 'exit' to quit): ";
    while (std::getline(std::cin, postfix)) {
        if (postfix == "exit") break;

        if (tree.buildFromPostfix(postfix)) {
            std::cout << "Infix expression: ";
            tree.printInfixExpression();
            std::cout << "Postfix expression: ";
            tree.printPostfixExpression();
            std::cout << "Number of nodes: " << tree.size() << std::endl;
            std::cout << "Number of leaf nodes: " << tree.leaf_nodes() << std::endl;
        } else {
            std::cerr << "Failed to build binary expression tree.\n";
        }

        std::cout << "\nEnter another postfix expression (or 'exit' to quit): ";
    }

    return 0;
}

