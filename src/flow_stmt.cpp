#include "Evalvisitor.h"

std::any EvalVisitor::visitFlow_stmt(Parser::Flow_stmtContext *ctx) {
  if (ctx->break_stmt() != nullptr) {
    return visit(ctx->break_stmt());
  } else if (ctx->continue_stmt()) {
    return visit(ctx->continue_stmt());
  } else if (ctx->return_stmt()) {
    return visit(ctx->return_stmt());
  }
  return std::pair<std::string, int>("None", 0);
}

std::any EvalVisitor::visitBreak_stmt(Parser::Break_stmtContext *ctx) {
  Control ans(2);
  return ans;
}

std::any EvalVisitor::visitContinue_stmt(Parser::Continue_stmtContext *ctx) {
  Control ans(1);
  return ans;
}

std::any EvalVisitor::visitReturn_stmt(Parser::Return_stmtContext *ctx) {
  Control ans(3);
  ans.return_val = std::pair<std::string, int>("None", 0);
  if (ctx->testlist() != nullptr) {
    std::vector<std::any> tmp =
        std::any_cast<std::vector<std::any>>(visit(ctx->testlist()));
    for (int i = 0; i < (int)tmp.size(); i++) {
      VariableToVal(tmp[i]);
    }
    if (tmp.size() == 1) {
      // Only one return value, set return_val to std::any type
      Control res(3);
      res.return_val = tmp[0];
      return res;
    }
    // Multiple return value, set return_val to std::vector type
    ans.return_val = tmp;
  }
  return ans;
}

std::any EvalVisitor::visitCompound_stmt(Parser::Compound_stmtContext *ctx) {
  if (ctx->if_stmt() != nullptr) {
    return visit(ctx->if_stmt());
  } else if (ctx->while_stmt()) {
    return visit(ctx->while_stmt());
  } else {
    return visit(ctx->funcdef());
  }
}

std::any EvalVisitor::visitIf_stmt(Parser::If_stmtContext *ctx) {
  std::vector<Python3Parser::TestContext *> test_vector = ctx->test();
  std::vector<Python3Parser::SuiteContext *> suite_vector = ctx->suite();
  for (int i = 0; i < (int)test_vector.size(); i++) {
    std::any tmp = visit(test_vector[i]);
    if (AnyToBool(tmp)) {
      return visit(suite_vector[i]);
    }
  }
  // Visit else branch
  if (test_vector.size() != suite_vector.size()) {
    return visit(suite_vector.back());
  }
  return std::pair<std::string, int>("None", 0);
}

std::any EvalVisitor::visitWhile_stmt(Parser::While_stmtContext *ctx) {
  while (AnyToBool(visit(ctx->test()))) {
    std::any ans = visit(ctx->suite());
    if (ans.type() == typeid(Control)) {
      Control res = std::any_cast<Control>(ans);
      if (res.op == 2) { // Break case
        return std::pair<std::string, int>("None", 0);
      } else if (res.op == 3) { // Return case
        return res;
      }
    }
  }
  return std::pair<std::string, int>("None", 0);
}