#include "Evalvisitor.h"
#include <any>

bool AnyToBool (std::any tmp) {
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp);
  } else if (tmp.type() == typeid(std::string)) {
    return std::any_cast<std::string>(tmp) != "";
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    return std::any_cast<int2048>(tmp);
    // return !std::any_cast<int2048>(tmp).CheckZero();
  }
  return 0;
} 
double AnyToDouble (std::any tmp) {
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp) ? 1 : 0;
  } else if (tmp.type() == typeid(std::string)) {
    return StringToDouble(std::any_cast<std::string>(tmp));
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    return std::any_cast<int2048>(tmp);
    // TODO
  }
  return 0;
}
int2048 AnyToInt (std::any tmp) {
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp);
  } else if (tmp.type() == typeid(std::string)) {
    return StringToInt(std::any_cast<std::string>(tmp));
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    return std::any_cast<int2048>(tmp);
  }
  return 0;
  // return int2048();
}
std::string AnyToString (std::any tmp) {
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp) ? "True" : "False";
  } else if (tmp.type() == typeid(std::string)) {
    return std::any_cast<std::string>(tmp);
  } else if (tmp.type() == typeid(double)) {
    return DoubleToString(std::any_cast<double>(tmp));
  } else if (tmp.type() == typeid(int2048)) {
    return IntToString(std::any_cast<int2048>(tmp));
  }
  return "";
};
double StringToDouble (std::string str) {
  int sz = str.size(), flag = 1, pos = 0;
  double ans = 0, val = 1;
  if (str[pos] == '+') {
    pos++;
  } else if (str[pos] == '-') {
    pos++;
    flag = -1;
  }
  bool point = 0;
  while (pos < sz) {
    if (str[pos] == '.') {
      point = 1;
    } else if (point == 0) {
      ans = ans * 10 + str[pos] - '0';
    } else if (point == 1) {
      val *= 0.1;
      ans += (str[pos] - '0') * val;
    }
    pos++;
  }
  return flag * ans;
} 
std::string DoubleToString(double val) {
  int flag = (val >= 0 ? 1 : -1);
  val = std::abs(val);
  int tmp = int(val);
  std::string ans = "";
  val = val - tmp;
  while (tmp) {
    ans += tmp % 10 + '0';
    tmp /= 10;
  }
  for (int i = 0, j = ans.size() - 1; i < j; i++, j--){
    std::swap(ans[i], ans[j]);
  }
  ans += '.';
  for (int i = 1; i <= 6; i++) {
    val *= 10;
    ans += int(val) + '0';
    val = val - int(val);
  }
  if (flag == -1) {
    ans = '-' + ans;
  }
  return ans;
};
std::string IntToString(int2048 val) {
  std::string ans = "";
  int flag = (val >= 0 ? 1 : -1);
  while (val) {
    ans += val % 10 + '0';
    val /= 10;
  }
  for (int i = 0, j = ans.size() - 1; i < j; i++, j--){
    std::swap(ans[i], ans[j]);
  }
  if (flag == -1) {
    ans = '-' + ans;
  } 
  return ans;
};
int2048 StringToInt (std::string str) {
  int sz = str.size(), flag = 1, pos = 0;
  double ans = 0, val = 1;
  if (str[pos] == '+') {
    pos++;
  } else if (str[pos] == '-') {
    pos++;
    flag = -1;
  }
  while (pos < sz) {
    ans = ans * 10 + str[pos] - '0';
  }
  return flag * ans;
} 

std::any EvalVisitor::GetValue(std::any variable) {
  std::string name = std::any_cast<std::string>(variable);
  if (variables_stack.back().find(name) != variables_stack.back().end()) {
    return variables_stack.back()[name];
  } else {
    return variables_stack.front()[name];
  }
}
std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
  variables_stack.push_back(std::map<std::string, std::any>());
  for (auto stmt : ctx->stmt()) {
    visit(stmt);
  }
  return 0;
}
std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
  Function function;
  std::string name = ctx->NAME()->getText();
  function.parameter_list = std::any_cast<std::vector <Parameter>>(visit(ctx->parameters()));
  function.suite = ctx->suite();
  functions[name] = function;
  return 0;
}
std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
  std::vector <Parameter> res;
  if (ctx->typedargslist() != nullptr) {
    res = std::any_cast<std::vector <Parameter>>(visit(ctx->typedargslist()));
  }
  return res;
}
std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {
  std::vector tfpdef_vector = ctx->tfpdef();
  std::vector test_vector = ctx->test();
  std::vector <Parameter> res;
  int all = tfpdef_vector.size(), unval = all - test_vector.size();
  for (int i = 0; i < unval; i++) {
    Parameter tmp;
    tmp.name = AnyToString(visit(ctx->tfpdef(i)));
    tmp.val = std::any();
    res.push_back(tmp);
  }
  for (int i = unval; i < all; i++) {
    Parameter tmp;
    tmp.name = AnyToString(visit(ctx->tfpdef(i)));
    tmp.val = visit(ctx->test(i - unval));
    res.push_back(tmp);
  }
  return res;
}
std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {
  return ctx->NAME()->getText();
}
std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
  if (ctx->simple_stmt() != nullptr) {
    return visit(ctx->simple_stmt());
  } else {
    return visit(ctx->compound_stmt());
  }
}
std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
  return visit(ctx->small_stmt());
}
std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
  if (ctx->expr_stmt() != nullptr) {
    return visit(ctx->expr_stmt());
  } else {
    return visit(ctx->flow_stmt());
  }
}
std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {}
std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {}
std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
  // if (ctx->break_stmt() != nullptr) {
  //   return visit(ctx->break_stmt());
  // } else if (ctx->continue_stmt()) {
  //   return visit(ctx->continue_stmt());
  // } else {
  //   return visit(ctx->return_stmt());
  // }
}
std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {}
std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {}
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {}
std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
  if (ctx->if_stmt() != nullptr) {
    return visit(ctx->if_stmt());
  } else if (ctx->while_stmt()) {
    return visit(ctx->while_stmt());
  } else {
    return visit(ctx->funcdef());
  }
}
std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {}
std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {}
std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {}
std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
  return visit(ctx->or_test());
}
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
  std::vector<Python3Parser::And_testContext *> and_vector = ctx->and_test();
  std::any res;
  for (int i = 0; i < and_vector.size(); i++) {
    res = visit(and_vector[i]);
    if (AnyToBool(res)) {
      return true;
    }
  }
  return false;
}
std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
    std::vector<Python3Parser::Not_testContext *> not_vector = ctx->not_test();
  std::any res = (bool) true;
  for (int i = 0; i < not_vector.size(); i++) {
    res = visit(not_vector[i]);
    if (!AnyToBool(res)) {
      return false;
    }
  }
  return true;
}
std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
  if (ctx->not_test() != nullptr) {
    return !AnyToBool(visit(ctx->not_test()));
  } else {
    return visit(ctx->comparison());
  }
}
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
  std::vector<Python3Parser::Arith_exprContext *> arith_expr_vector = ctx->arith_expr();
  std::vector<Python3Parser::Comp_opContext *> comp_op_vector = ctx->comp_op();
  std::any ans = visit(arith_expr_vector[0]);
  for (int i = 1; i < (int)arith_expr_vector.size(); i++) {
    std::any nxt = visit(arith_expr_vector[i]);
    std::string op = AnyToString(visit(comp_op_vector[i - 1]));
    if (op == "<") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) >= AnyToString(nxt)) {
          return false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) >= AnyToDouble(nxt)) {
          return false;
        }
      } else if (AnyToInt(ans) >= AnyToInt(nxt)) {
        return false;
      }
    } else if (op == '>') {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) <= AnyToString(nxt)) {
          return false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) <= AnyToDouble(nxt)) {
          return false;
        }
      } else if (AnyToInt(ans) <= AnyToInt(nxt)) {
        return false;
      }
    } else if (op == "<=") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) > AnyToString(nxt)) {
          return false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) > AnyToDouble(nxt)) {
          return false;
        }
      } else if (AnyToInt(ans) > AnyToInt(nxt)) {
        return false;
      }
    } else if (op == ">=") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) < AnyToString(nxt)) {
          return false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) < AnyToDouble(nxt)) {
          return false;
        }
      } else if (AnyToInt(ans) < AnyToInt(nxt)) {
        return false;
      }
    } else if (op == "==") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) != AnyToString(nxt)) {
          return false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) != AnyToDouble(nxt)) {
          return false;
        }
      } else if (AnyToInt(ans) != AnyToInt(nxt)) {
        return false;
      }
    } else if (op == "!=") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) == AnyToString(nxt)) {
          return false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) == AnyToDouble(nxt)) {
          return false;
        }
      } else if (AnyToInt(ans) == AnyToInt(nxt)) {
        return false;
      }
    }
    ans = nxt;
  }
  return true;
}
std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
  if (ctx->LESS_THAN() != nullptr) {
    return "<";
  } else if (ctx->GREATER_THAN() != nullptr) {
    return ">";
  } else if (ctx->EQUALS() != nullptr) {
    return "==";
  } else if (ctx->GT_EQ() != nullptr) {
    return ">=";
  } else if (ctx->LT_EQ() != nullptr) {
    return "<=";
  } else if (ctx->NOT_EQ_2() != nullptr) {
    return "!=";
  } 
  return -1;
}
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {}
std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {}
std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {}
std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) {}
std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {}
std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {}
std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {}
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {}
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {}
std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {}
std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {}
std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {}