// 这是一份测试代码，仅供参考
// #include "calc_std.h"
#include "calc.h"
#include <iostream>


node *generate(int);

// Expected: 17711
signed main() {
    node *root = generate(20);
    visitor *calc = new calculator;
    auto result = calc->visit(root);
    if (auto value = std::any_cast <double> (&result)) {
        std::cout << "Result: " << *value << '\n';
    } else {
        std::cout << "Wrong Type" << '\n';
    }
    return 0;
}


// 生成器，小孩子不懂写着玩的
node *generate(int depth) {
    if (depth <= 0) {
        num_node *ret = new num_node;
        ret->number = (double)1.0;
        return ret;
    }
    add_node *root = new add_node;
    root->lnode = generate(depth - 1);
    root->rnode = generate(depth - 2);
    return root;
}