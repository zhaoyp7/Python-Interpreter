# 四则运算计算器

## 题目描述

现在，你需要实现全部的四则运算的功能，即加减乘除。简单起见，我们已经将复杂的输入的表达式解析成了树形结构，其定义如下:

- 每个节点 $node$ 可以是一个数字，或者是一个运算符。其表示一个值，记作 $val(node)$ 。
- 如果一个节点是数字节点，数字的值为 $n$ ，那么 $val(node) = n$ 。
- 如果一个节点是运算符节点，运算符为 $op$ ，那么其有两个子节点 $l$ 和 $r$ ，且 $val(node) = val(l) ~\ op ~\ val(r)$ 。

## std::any

需要注意的是，在我们的代码中出现了 `std::any` 。其表现类似 python 中的变量，即可以保存任何类型的值。通过 `std::any_cast` 函数 ，我们可以判断 `any` 是否判断了某一类型的值，并且获取其值。具体如何使用，请参考 [cpprefence](https://en.cppreference.com/w/cpp/utility/any) 。

最基本的用法大致如下:

```C++
std::any x = (int)1;
x = 2;

// 如果存储的是 int 类型，返回指向该值的指针
// 否则返回空指针 nullptr
int *ptr = std::any_cast <int> (&x);

// 如果存储的是 int 类型，返回该值
// 否则抛出异常
int  val = std::any_cast <int>  (x);

// 如果存储的是 int 类型，返回该值的引用
// 否则抛出异常
int &ref = std::any_cast <int&> (x);

```

在本题中，我们保证同一颗表达式树中，数字节点的值都是 `long long` 类型或者 `double` 类型。当然，你需要保证输出 `std::any` 的类型与之相同。

## 题目模板

节点的具体定义在 [visitor.h](visitor.h) 。你不可以修改这些代码。

```C++
// 你不可以修改这份代码
#pragma once
#include <any>

struct visitor;

struct node {
    virtual std::any accept(visitor *) = 0;
    virtual ~node() = default;
};

struct num_node;
struct add_node;
struct sub_node;
struct mul_node;
struct div_node;

struct visitor {
    virtual std::any visit(node *ctx) { return ctx->accept(this); }
    virtual std::any visit_num(num_node *) = 0;
    virtual std::any visit_add(add_node *) = 0;
    virtual std::any visit_sub(sub_node *) = 0;
    virtual std::any visit_mul(mul_node *) = 0;
    virtual std::any visit_div(div_node *) = 0;
    virtual ~visitor() = default;
};

struct num_node final : node {
    std::any number;
    std::any accept(visitor *ctx) override { return ctx->visit_num(this); }
    ~num_node() override = default;
};

struct add_node final : node {
    node *lnode;
    node *rnode;
    std::any accept(visitor *ctx) override { return ctx->visit_add(this); }
    ~add_node() override = default;
};

struct sub_node final : node {
    node *lnode;
    node *rnode;
    std::any accept(visitor *ctx) override { return ctx->visit_sub(this); }
    ~sub_node() override = default;
};

struct mul_node final : node {
    node *lnode;
    node *rnode;
    std::any accept(visitor *ctx) override { return ctx->visit_mul(this); }
    ~mul_node() override = default;
};

struct div_node final : node {
    node *lnode;
    node *rnode;
    std::any accept(visitor *ctx) override { return ctx->visit_div(this); }
    ~div_node() override = default;
};

```

现在，你需要按照要求实现一个 `calculator` 类。对于一个 `calculator` 对象，通过虚函数重写基类 `visitor` 的接口。我们希望每次调用其 `visit` 方法，可以返回表达式的结果，且不修改原本的表达式。

```C++
#include "visitor.h"

struct calculator : visitor {
    /// TODO: 完成所有需求，这应该不难
    ~calculator() override = default;
};
```

你需要提交这份[代码](calc.h)
