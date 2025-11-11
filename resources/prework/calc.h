// 你需要提交这份代码
#pragma once

#include "visitor.h"
#include <any>

struct calculator : visitor {
  /// TODO: 完成所有需求
  ~calculator() override = default;
  std::any visit_num(num_node *ctx) override { return ctx->number; }
  std::any visit_add(add_node *ctx) override {
    return calc(ctx->lnode, ctx->rnode, 0);
  }
  std::any visit_sub(sub_node *ctx) override {
    return calc(ctx->lnode, ctx->rnode, 1);
  }
  std::any visit_mul(mul_node *ctx) override {
    return calc(ctx->lnode, ctx->rnode, 2);
  }
  std::any visit_div(div_node *ctx) override {
    return calc(ctx->lnode, ctx->rnode, 3);
  }
  std::any calc(node *l, node *r, int op) {
    std::any vall = l->accept(this), valr = r->accept(this);
    if (std::any_cast<long long>(&vall) != nullptr) {
      long long x = std::any_cast<long long>(vall);
      long long y = std::any_cast<long long>(valr);
      if (op == 0) {
        return x + y;
      } else if (op == 1) {
        return x - y;
      } else if (op == 2) {
        return x * y;
      } else {
        return x / y;
      }
    } else {
      double x = std::any_cast<double>(vall);
      double y = std::any_cast<double>(valr);
      if (op == 0) {
        return x + y;
      } else if (op == 1) {
        return x - y;
      } else if (op == 2) {
        return x * y;
      } else {
        return x / y;
      }
    }
  }
};
