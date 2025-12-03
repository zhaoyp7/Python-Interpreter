#include "Evalvisitor.h"

std::any EvalVisitor::visitTest(Parser::TestContext *ctx) {
  return visit(ctx->or_test());
}

std::any EvalVisitor::visitOr_test(Parser::Or_testContext *ctx) {
  std::vector<Python3Parser::And_testContext *> and_vector = ctx->and_test();
  int sz = and_vector.size();
  if (sz == 1) { // No or operator
    return visit(and_vector[0]);
  }
  std::any res;
  for (int i = 0; i < sz; i++) {
    res = visit(and_vector[i]);
    VariableToVal(res);
    if (AnyToBool(res)) {
      return (bool)true;
    }
  }
  return (bool)false;
}

std::any EvalVisitor::visitAnd_test(Parser::And_testContext *ctx) {
  std::vector<Python3Parser::Not_testContext *> not_vector = ctx->not_test();
  int sz = not_vector.size();
  if (sz == 1) { // No and operator
    return visit(not_vector[0]);
  }
  std::any res = (bool)true;
  for (int i = 0; i < sz; i++) {
    res = visit(not_vector[i]);
    VariableToVal(res);
    if (!AnyToBool(res)) {
      return (bool)false;
    }
  }
  return (bool)true;
}

std::any EvalVisitor::visitNot_test(Parser::Not_testContext *ctx) {
  if (ctx->not_test() != nullptr) {
    std::any tmp = visit(ctx->not_test());
    VariableToVal(tmp);
    bool res = AnyToBool(tmp);
    return !res;
  } else {
    return visit(ctx->comparison());
  }
}

std::any EvalVisitor::visitComparison(Parser::ComparisonContext *ctx) {
  std::vector<Python3Parser::Arith_exprContext *> arith_expr_vector =
      ctx->arith_expr();
  std::vector<Python3Parser::Comp_opContext *> comp_op_vector = ctx->comp_op();
  int sz = arith_expr_vector.size();
  std::any ans = visit(arith_expr_vector[0]);
  if (sz == 1) { // No comparison operator
    return ans;
  }
  for (int i = 1; i < sz; i++) {
    std::any nxt = visit(arith_expr_vector[i]);
    VariableToVal(ans);
    VariableToVal(nxt);
    bool ans_none = (ans.type() == typeid(std::pair<std::string, int>));
    bool nxt_none = (nxt.type() == typeid(std::pair<std::string, int>));
    std::string op = AnyToString(visit(comp_op_vector[i - 1]));
    if (op == "<") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) >= AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double) || nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) >= AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) >= AnyToInt(nxt)) {
        return (bool)false;
      }
    } else if (op == ">") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) <= AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double) || nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) <= AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) <= AnyToInt(nxt)) {
        return (bool)false;
      }
    } else if (op == "<=") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) > AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double) || nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) > AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) > AnyToInt(nxt)) {
        return (bool)false;
      }
    } else if (op == ">=") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) < AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double) || nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) < AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) < AnyToInt(nxt)) {
        return (bool)false;
      }
    } else if (op == "==") {
      if (ans_none || nxt_none) { // Resolve None == operator
        if (!ans_none || !nxt_none) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(std::string) ||
                 nxt.type() == typeid(std::string)) {
        if (nxt.type() != typeid(std::string) ||
            ans.type() != typeid(std::string)) {
          // Resolve str == others
          return (bool)false;
        }
        if (AnyToString(ans) != AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double) || nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) != AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) != AnyToInt(nxt)) {
        return (bool)false;
      }
    } else if (op == "!=") {
      if (ans_none || nxt_none) { // Resolve None != operator
        if (ans_none && nxt_none) {
          return (bool)false;
        }
      }
      if (ans.type() == typeid(std::string) ||
          nxt.type() == typeid(std::string)) {
        if (ans.type() != typeid(std::string) ||
            nxt.type() != typeid(std::string)) {
          // Resolve str == others
          return (bool)false;
        }
        if (AnyToString(ans) == AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double) || nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) == AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) == AnyToInt(nxt)) {
        return (bool)false;
      }
    }
    ans = nxt; // Only calculate once
  }
  return (bool)true;
}

std::any EvalVisitor::visitComp_op(Parser::Comp_opContext *ctx) {
  if (ctx->LESS_THAN() != nullptr) {
    return (std::string) "<";
  } else if (ctx->GREATER_THAN() != nullptr) {
    return (std::string) ">";
  } else if (ctx->EQUALS() != nullptr) {
    return (std::string) "==";
  } else if (ctx->GT_EQ() != nullptr) {
    return (std::string) ">=";
  } else if (ctx->LT_EQ() != nullptr) {
    return (std::string) "<=";
  } else if (ctx->NOT_EQ_2() != nullptr) {
    return (std::string) "!=";
  }
  return -1;
}

std::any EvalVisitor::visitArith_expr(Parser::Arith_exprContext *ctx) {
  std::vector<Python3Parser::TermContext *> term_vector = ctx->term();
  std::vector<Python3Parser::Addorsub_opContext *> addorsub_op_vector =
      ctx->addorsub_op();
  std::any ans = visit(term_vector[0]);
  int sz = term_vector.size();
  for (int i = 1; i < sz; i++) {
    std::any tmp = visit(term_vector[i]);
    std::string op = AnyToString(visit(addorsub_op_vector[i - 1]));
    VariableToVal(ans);
    VariableToVal(tmp);
    if (op == "+") {
      if (ans.type() == typeid(std::string) &&
          tmp.type() == typeid(std::string)) {
        ans = AnyToString(ans) + AnyToString(tmp);
      } else if (ans.type() == typeid(double) || tmp.type() == typeid(double)) {
        ans = AnyToDouble(ans) + AnyToDouble(tmp);
      } else {
        ans = AnyToInt(ans) + AnyToInt(tmp);
      }
    } else if (op == "-") {
      if (ans.type() == typeid(double) || tmp.type() == typeid(double)) {
        ans = AnyToDouble(ans) - AnyToDouble(tmp);
      } else {
        ans = AnyToInt(ans) - AnyToInt(tmp);
      }
    }
  }
  return ans;
}

std::any EvalVisitor::visitAddorsub_op(Parser::Addorsub_opContext *ctx) {
  if (ctx->ADD() != nullptr) {
    return (std::string) "+";
  } else if (ctx->MINUS() != nullptr) {
    return (std::string) "-";
  }
  return 0;
}

std::any EvalVisitor::visitTerm(Parser::TermContext *ctx) {
  std::vector<Python3Parser::FactorContext *> factor_vector = ctx->factor();
  std::vector<Python3Parser::Muldivmod_opContext *> muldivmod_op_vector =
      ctx->muldivmod_op();
  std::any ans = visit(factor_vector[0]);
  int sz = factor_vector.size();
  for (int i = 1; i < sz; i++) {
    std::any tmp = visit(factor_vector[i]);
    VariableToVal(ans);
    VariableToVal(tmp);
    std::string op = AnyToString(visit(muldivmod_op_vector[i - 1]));
    if (op == "*") {
      if (ans.type() == typeid(std::string)) {
        std::string str = AnyToString(ans), res = "";
        int2048 len = AnyToInt(tmp);
        while (len.CheckZero() == 0 && len.getsign() == 1) {
          len.minus1();
          res += str;
        }
        ans = res;
      } else if (tmp.type() == typeid(std::string)) {
        std::string str = AnyToString(tmp), res = "";
        int2048 len = AnyToInt(ans);
        while (len.CheckZero() == 0 && len.getsign() == 1) {
          len.minus1();
          res += str;
        }
        ans = res;
      } else if (ans.type() == typeid(double) || tmp.type() == typeid(double)) {
        ans = AnyToDouble(ans) * AnyToDouble(tmp);
      } else {
        ans = AnyToInt(ans) * AnyToInt(tmp);
      }
    } else if (op == "/") {
      ans = AnyToDouble(ans) / AnyToDouble(tmp);
    } else if (op == "//") {
      ans = AnyToInt(ans) / AnyToInt(tmp);
    } else if (op == "%") {
      ans = AnyToInt(ans) % AnyToInt(tmp);
    }
  }
  return ans;
}

std::any EvalVisitor::visitMuldivmod_op(Parser::Muldivmod_opContext *ctx) {
  if (ctx->STAR() != nullptr) {
    return (std::string) "*";
  } else if (ctx->DIV() != nullptr) {
    return (std::string) "/";
  } else if (ctx->IDIV() != nullptr) {
    return (std::string) "//";
  } else if (ctx->MOD() != nullptr) {
    return (std::string) "%";
  }
  return 0;
}

std::any EvalVisitor::visitFactor(Parser::FactorContext *ctx) {
  if (ctx->atom_expr() != nullptr) {
    return visit(ctx->atom_expr());
  } else if (ctx->ADD() != nullptr) {
    std::any ans = visit(ctx->factor());
    VariableToVal(ans);
    return ans;
  } else if (ctx->MINUS() != nullptr) {
    std::any ans = visit(ctx->factor());
    VariableToVal(ans);
    if (ans.type() == typeid(double)) {
      if (AnyToDouble(ans)) {
        ans = -AnyToDouble(ans);
      }
    } else {
      ans = -AnyToInt(ans);
    }
    return ans;
  } else { // This branch may not entered
    return visit(ctx->factor());
  }
}