#include "Evalvisitor.h"

std::any EvalVisitor::visitTestlist(Parser::TestlistContext *ctx) {
  std::vector<Parser::TestContext *> test_vector = ctx->test();
  std::vector<std::any> ans;
  for (int i = 0; i < test_vector.size(); i++) {
    ans.push_back(visit(test_vector[i]));
  }
  return ans;
}

std::any EvalVisitor::visitArglist(Parser::ArglistContext *ctx) {
  std::vector<Parser::ArgumentContext *> argument_vector = ctx->argument();
  int sz = argument_vector.size();
  std::vector<std::any> ans;
  for (int i = 0; i < sz; i++) {
    std::any tmp = visit(argument_vector[i]);
    if (tmp.type() == typeid(std::vector<std::any>)) {
      std::vector<std::any> temp = std::any_cast<std::vector<std::any>>(tmp);
      for (auto x : temp) {
        ans.push_back(x);
      }
    } else {
      ans.push_back(tmp);
    }
  }
  return ans;
}

std::any EvalVisitor::visitArgument(Parser::ArgumentContext *ctx) {
  std::vector<Parser::TestContext *> test_vector = ctx->test();
  int sz = test_vector.size();
  if (sz == 1) {
    std::any ans = visit(test_vector[0]);
    VariableToVal(ans);
    return ans;
  } else {
    // Resolve assign operator
    // We cannot set value here, in case of "foo(a = 1, b = 2)"
    std::any ans = visit(test_vector[1]);
    VariableToVal(ans);
    return std::pair(visit(test_vector[0]), ans);
  }
}