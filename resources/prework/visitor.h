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
