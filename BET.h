#include <iostream>
#include <stack>
#include <string>
#include <sstream>
class BET {
private:
    struct BinaryNode {
        std::string element;
        BinaryNode* left;
        BinaryNode* right;
        BinaryNode(const std::string& e, BinaryNode* l = nullptr, BinaryNode* r = nullptr)
            : element(e), left(l), right(r) {}
    };
    BinaryNode* root;
    
    int getPrecedence(const std::string& op) const {
        if (op == "*" || op == "/") return 2;
        if (op == "+" || op == "-") return 1;
        return 0;  // For operands
    }
    
    void printInfixExpression(BinaryNode* n, bool isRoot = true, const BinaryNode* parent = nullptr) const {
        if (!n) return;
        
        bool needParens = false;
        if (!isRoot && isOperator(n->element)) {
            // Check if parent exists and has higher or equal precedence
            if (parent && isOperator(parent->element)) {
                int parentPrec = getPrecedence(parent->element);
                int currentPrec = getPrecedence(n->element);
                
                // Need parentheses if:
                // 1. Parent has higher precedence
                // 2. Parent has equal precedence and this is a right child
                needParens = (currentPrec < parentPrec) || 
                            (currentPrec == parentPrec && parent->right == n);
            }
        }
        
        if (needParens) std::cout << "( ";
        
        if (n->left) {
            printInfixExpression(n->left, false, n);
        }
        
        std::cout << n->element << " ";
        
        if (n->right) {
            printInfixExpression(n->right, false, n);
        }
        
        if (needParens) std::cout << ") ";
    }
    
    void printPostfixExpression(BinaryNode* n) const {
        if (!n) return;
        printPostfixExpression(n->left);
        printPostfixExpression(n->right);
        std::cout << n->element << " ";
    }
    
    void makeEmpty(BinaryNode*& t) {
        if (!t) return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }
    
    BinaryNode* clone(BinaryNode* t) const {
        if (!t) return nullptr;
        return new BinaryNode(t->element, clone(t->left), clone(t->right));
    }
    
    size_t size(BinaryNode* t) const {
        if (!t) return 0;
        return 1 + size(t->left) + size(t->right);
    }
    
    size_t leaf_nodes(BinaryNode* t) const {
        if (!t) return 0;
        if (!t->left && !t->right) return 1;
        return leaf_nodes(t->left) + leaf_nodes(t->right);
    }
    
    bool isOperator(const std::string& token) const {
        return token == "+" || token == "-" || token == "*" || token == "/";
    }

public:
    // Public interfaces remain the same
    BET() : root(nullptr) {}
    
    BET(const std::string& postfix) : root(nullptr) {
        buildFromPostfix(postfix);
    }
    
    BET(const BET& rhs) : root(clone(rhs.root)) {}
    
    ~BET() {
        makeEmpty(root);
    }
    
    const BET& operator=(const BET& rhs) {
        if (this != &rhs) {
            makeEmpty(root);
            root = clone(rhs.root);
        }
        return *this;
    }
    bool buildFromPostfix(const std::string& postfix) {
        makeEmpty(root);
        std::stack<BinaryNode*> s;  // Use STL stack
        std::istringstream iss(postfix);
        std::string token;
        int operandCount = 0;
        int operatorCount = 0;

        while (iss >> token) {
            if (isOperator(token)) {
                // Check if there are enough operands for this operator
                if (s.size() < 2) {
                    std::cerr << "Error: Not enough operands for operator " << token << std::endl;
                    return false;
                }
                operatorCount++;
                BinaryNode* right = s.top(); s.pop();
                BinaryNode* left = s.top(); s.pop();
                s.push(new BinaryNode(token, left, right));
            } else {
                // It's an operand
                operandCount++;
                s.push(new BinaryNode(token));
            }
        }

    // Check for valid expression
    // Case 1: Single operand is valid
    if (operandCount == 1 && operatorCount == 0) {
        root = s.top();
        return true;
    }

    // Case 2: For all other cases, we need operandCount = operatorCount + 1
    if (operandCount != operatorCount + 1) {
        std::cerr << "Error: Imbalanced expression. Too many ";
        if (operandCount > operatorCount + 1)
            std::cerr << "operands." << std::endl;
        else
            std::cerr << "operators." << std::endl;
        return false;
    }

    root = s.top();
    return true;
}
    void printInfixExpression() const {
        if (root) {
            printInfixExpression(root);
            std::cout << std::endl;
        }
    }
    
    void printPostfixExpression() const {
        if (root) {
            printPostfixExpression(root);
            std::cout << std::endl;
        }
    }
    
    size_t size() const {
        return size(root);
    }
    
    size_t leaf_nodes() const {
        return leaf_nodes(root);
    }
    
    bool empty() const {
        return root == nullptr;
    }
};
