#include "Evalvisitor.h"

std::any EvalVisitor::visitStmt(Parser::StmtContext *ctx) {
  if (ctx->simple_stmt() != nullptr) {
    return visit(ctx->simple_stmt());
  } else {
    return visit(ctx->compound_stmt());
  }
}

std::any EvalVisitor::visitSimple_stmt(Parser::Simple_stmtContext *ctx) {
  return visit(ctx->small_stmt());
}

std::any EvalVisitor::visitSmall_stmt(Parser::Small_stmtContext *ctx) {
  if (ctx->expr_stmt() != nullptr) {
    return visit(ctx->expr_stmt());
  } else {
    return visit(ctx->flow_stmt());
  }
}

std::any EvalVisitor::visitExpr_stmt(Parser::Expr_stmtContext *ctx) {
  std::vector<Python3Parser::TestlistContext *> testlist_vector =
      ctx->testlist();
  int sz = testlist_vector.size();

  if (sz == 1) { // No augassign or assign operator
    return visit(testlist_vector[0]);
  }
  if (ctx->augassign() != nullptr) {
    // Augassign operator : "name op tmp"
    std::string op = AnyToString(visit(ctx->augassign()));
    std::vector<std::any> tmp_name =
        std::any_cast<std::vector<std::any>>(visit(testlist_vector[0]));
    std::string name =
        (std::any_cast<std::pair<std::string, int>>(tmp_name[0])).first;
    std::any tmp =
        std::any_cast<std::vector<std::any>>(visit(testlist_vector[1]))[0];
    VariableToVal(tmp);
    std::any ans = GetValue(name);
    if (op == "+=") {
      if (tmp.type() == typeid(std::string)) {
        ans = AnyToString(ans) + AnyToString(tmp);
      } else if (tmp.type() == typeid(double) || ans.type() == typeid(double)) {
        ans = AnyToDouble(ans) + AnyToDouble(tmp);
      } else {
        ans = AnyToInt(ans) + AnyToInt(tmp);
      }
    } else if (op == "-=") {
      if (tmp.type() == typeid(double) || ans.type() == typeid(double)) {
        ans = AnyToDouble(ans) - AnyToDouble(tmp);
      } else {
        ans = AnyToInt(ans) - AnyToInt(tmp);
      }
    } else if (op == "*=") {
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
      } else if (tmp.type() == typeid(double) || ans.type() == typeid(double)) {
        ans = AnyToDouble(ans) * AnyToDouble(tmp);
      } else {
        ans = AnyToInt(ans) * AnyToInt(tmp);
      }
    } else if (op == "/=") {
      ans = AnyToDouble(ans) / AnyToDouble(tmp);
    } else if (op == "//=") {
      ans = AnyToInt(ans) / AnyToInt(tmp);
    } else if (op == "%=") {
      ans = AnyToInt(ans) % AnyToInt(tmp);
    }
    SetValue(name, ans);
  } else {
    // Assign operator
    std::vector<std::any> res =
        std::any_cast<std::vector<std::any>>(visit(testlist_vector[sz - 1]));
    std::vector<std::any> ans;
    for (int i = 0; i < res.size(); i++) {
      if (res[i].type() == typeid(std::vector<std::any>)) {
        std::vector<std::any> tmp =
            std::any_cast<std::vector<std::any>>(res[i]);
        for (auto x : tmp) {
          ans.push_back(x);
        }
      } else {
        ans.push_back(res[i]);
      }
    }
    for (int i = 0; i < ans.size(); i++) {
      VariableToVal(ans[i]);
    }
    for (int i = sz - 2; i >= 0; i--) {
      std::vector<std::any> tmp =
          std::any_cast<std::vector<std::any>>(visit(testlist_vector[i]));
      for (int j = 0; j < ans.size(); j++) {
        std::pair<std::string, int> temp =
            std::any_cast<std::pair<std::string, int>>(tmp[j]);
        SetValue(temp.first, ans[j]);
      }
    }
  }
  return std::pair<std::string, int>("None", 0);
}

std::any EvalVisitor::visitAugassign(Parser::AugassignContext *ctx) {
  if (ctx->ADD_ASSIGN() != nullptr) {
    return (std::string) "+=";
  } else if (ctx->SUB_ASSIGN() != nullptr) {
    return (std::string) "-=";
  } else if (ctx->MULT_ASSIGN() != nullptr) {
    return (std::string) "*=";
  } else if (ctx->DIV_ASSIGN() != nullptr) {
    return (std::string) "/=";
  } else if (ctx->IDIV_ASSIGN() != nullptr) {
    return (std::string) "//=";
  } else if (ctx->MOD_ASSIGN() != nullptr) {
    return (std::string) "%=";
  }
  return 0;
}