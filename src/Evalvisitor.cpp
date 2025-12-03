#include "Evalvisitor.h"
#include "int2048.h"
#include <any>

bool EvalVisitor::AnyToBool(std::any tmp) {
  CheckVariable(tmp);
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp);
  } else if (tmp.type() == typeid(std::string)) {
    return std::any_cast<std::string>(tmp) != "";
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    // return std::any_cast<int2048>(tmp);
    return !std::any_cast<int2048>(tmp).CheckZero();
  }
  return (bool)false;
}

double EvalVisitor::AnyToDouble(std::any tmp) {
  CheckVariable(tmp);
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp) ? 1 : 0;
  } else if (tmp.type() == typeid(std::string)) {
    return StringToDouble(std::any_cast<std::string>(tmp));
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    return std::any_cast<int2048>(tmp);
    // return std::any_cast<long long>(tmp);
  }
  return 0;
}

int2048 EvalVisitor::AnyToInt(std::any tmp) {
  CheckVariable(tmp);
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp);
  } else if (tmp.type() == typeid(std::string)) {
    return StringToInt(std::any_cast<std::string>(tmp));
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    // puts("get int2048  get int2048  get int2048  get int2048");
    return std::any_cast<int2048>(tmp);
  }
  return 0;
  // return int2048();
}

std::string EvalVisitor::AnyToString(std::any tmp) {
  CheckVariable(tmp);
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp) ? "True" : "False";
  } else if (tmp.type() == typeid(std::string)) {
    return std::any_cast<std::string>(tmp);
  } else if (tmp.type() == typeid(double)) {
    return DoubleToString(std::any_cast<double>(tmp));
  } else if (tmp.type() == typeid(int2048)) {
    return IntToString(std::any_cast<int2048>(tmp));
  } else if (tmp.type() == typeid(std::pair<std::string, int>)) {
    return "None";
  }
  return "";
}

double EvalVisitor::StringToDouble(std::string str) {
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
  if (ans) {
    ans *= flag;
  }
  return ans;
}

std::string EvalVisitor::DoubleToString(double val) {
  int flag = (val >= 0 ? 1 : -1);
  val = std::abs(val);
  // long long temp = val * 1000000 + 0.5;
  // printf("temp = %lld\n",temp);
  // val = temp * 0.000001;
  // printf("val = %.9lf\n",val);
  long long tmp = (long long)val;
  std::string ans = "";
  val = val - tmp;
  while (tmp) {
    ans += tmp % 10 + '0';
    tmp /= 10;
  }
  if (ans == "") {
    ans = "0";
  }
  for (int i = 0, j = ans.size() - 1; i < j; i++, j--) {
    std::swap(ans[i], ans[j]);
  }
  ans += '.';
  tmp = val * 1000000 + 0.5;
  for (int i = 1; i <= 6; i++) {
    ans += tmp % 10 + '0';
    tmp /= 10;
  }
  for (int j = ans.size() - 1, i = j - 5; i < j; i++, j--) {
    std::swap(ans[i], ans[j]);
  }
  // for (int i = 1; i <= 6; i++) {
  //   val *= 10;
  //   ans += int(val) + '0';
  //   val = val - int(val);
  // }
  if (flag == -1) {
    ans = '-' + ans;
  }
  return ans;
}

std::string EvalVisitor::IntToString(int2048 val) {
  return (std::string)val;
  /*
  std::string ans = "";
  int flag = (val >= 0 ? 1 : -1);
  if (flag == -1) {
    val = -val;
  }
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
  */
}

int2048 EvalVisitor::StringToInt(std::string str) {
  int2048 ans(str);
  return ans;
  /*
  // std::cout << str << '\n';
  int sz = str.size(), flag = 1, pos = 0;
  int2048 ans = 0;
  if (str[pos] == '+') {
    pos++;
  } else if (str[pos] == '-') {
    pos++;
    flag = -1;
  }
  while (pos < sz) {
    ans = ans * 10 + str[pos] - '0';
    pos++;
  }
  // printf("asdfj;lasjfd;asdf : %lld\n",ans);
  return flag * ans;
  */
}

void EvalVisitor::InitFunction(std::string name, std::vector<std::any> val) {
  std::map<std::string, int> vis;
  for (int i = 0; i < (int)val.size(); i++) {
    // Resolve situations where variables are used as parameters
    if (val[i].type() == typeid(std::pair<std::any, std::any>)) {
      std::pair<std::any, std::any> tmp =
          std::any_cast<std::pair<std::any, std::any>>(val[i]);
      if (tmp.first.type() != typeid(std::pair<std::string, int>)) {
        CheckVariable(val[i]);
      }
    }
  }
  AddVariableStack();
  for (int i = 0; i < (int)val.size(); i++) {
    if (val[i].type() == typeid(std::pair<std::any, std::any>)) {
      // Resolve keyword argument situations "foo(a=1,b=2)"
      std::pair<std::any, std::any> tmp =
          std::any_cast<std::pair<std::any, std::any>>(val[i]);
      auto temp = std::any_cast<std::pair<std::string, int>>(tmp.first);
      std::string name = AnyToString(temp.first);
      vis[name] = 1;
      AddValue(name, tmp.second);
    } else {
      // Resolve positional argument situations "foo(1,2)"
      std::string variable_name = functions[name].parameter_list[i].name;
      vis[variable_name] = 1;
      CheckVariable(val[i]);
      AddValue(variable_name, val[i]);
    }
  }
  // Assign default values to variables
  for (int i = 0; i < (int)functions[name].parameter_list.size(); i++) {
    std::string variable_name = functions[name].parameter_list[i].name;
    std::any variable_val = functions[name].parameter_list[i].val;
    if (vis.find(variable_name) == vis.end()) {
      AddValue(variable_name, variable_val);
    }
  }
}

bool EvalVisitor::IsVariable(std::string name) {
  if (variables_stack.back().find(name) != variables_stack.back().end()) {
    return 1;
  } else if (variables_stack.front().find(name) !=
             variables_stack.front().end()) {
    return 1;
  }
  return 0;
}

std::any EvalVisitor::GetValue(std::any variable) {
  std::string name = std::any_cast<std::string>(variable);
  if (variables_stack.back().find(name) != variables_stack.back().end()) {
    return variables_stack.back()[name];
  } else {
    return variables_stack.front()[name];
  }
}

void EvalVisitor::SetValue(std::string name, std::any val) {
  // std::cout << "SetValue : name = " << name << '\n';
  if (variables_stack.back().find(name) != variables_stack.back().end()) {
    variables_stack.back()[name] = val;
  } else if (variables_stack.front().find(name) !=
             variables_stack.front().end()) {
    variables_stack.front()[name] = val;
  } else {
    variables_stack.back()[name] = val;
  }
}

void EvalVisitor::AddValue(std::string name, std::any val) {
  // std::cout << "AddValue : name = " << name << '\n';
  variables_stack.back()[name] = val;
}

// TODO : rename the function
void EvalVisitor::CheckVariable(std::any &tmp) {
  if (tmp.type() == typeid(std::pair<std::string, int>)) {
    // puts("this this");
    std::pair<std::string, int> temp =
        std::any_cast<std::pair<std::string, int>>(tmp);
    if (temp.second == 0) {
      return;
    }
    tmp = GetValue(temp.first);
  }
}

std::any EvalVisitor::visitFile_input(Parser::File_inputContext *ctx) {
  variables_stack.push_back(std::map<std::string, std::any>());
  for (auto stmt : ctx->stmt()) {
    visit(stmt);
  }
  return std::pair<std::string, int>("None", 0);
}

std::any EvalVisitor::visitFuncdef(Parser::FuncdefContext *ctx) {
  // build functions[name]
  // puts("enter : Funcdef");
  Function function;
  std::string name = ctx->NAME()->getText();
  // std::cout << "name is " << name << '\n';
  // puts("QwQ");
  function.parameter_list =
      std::any_cast<std::vector<Parameter>>(visit(ctx->parameters()));
  // int sz = function.parameter_list.size();
  // printf("sz = %d\n",sz);
  // puts("QwQ");
  function.suite = ctx->suite();
  // puts("QwQ");
  functions[name] = function;
  // puts("QwQ");
  return std::pair<std::string, int>("None", 0);
}

std::any EvalVisitor::visitParameters(Parser::ParametersContext *ctx) {
  std::vector<Parameter> res;
  if (ctx->typedargslist() != nullptr) {
    res = std::any_cast<std::vector<Parameter>>(visit(ctx->typedargslist()));
  }
  return res;
}

std::any EvalVisitor::visitTypedargslist(Parser::TypedargslistContext *ctx) {
  std::vector tfpdef_vector = ctx->tfpdef();
  std::vector test_vector = ctx->test();
  std::vector<Parameter> res;
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

std::any EvalVisitor::visitTfpdef(Parser::TfpdefContext *ctx) {
  return ctx->NAME()->getText();
}

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
    CheckVariable(tmp);
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
      CheckVariable(ans[i]);
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
      CheckVariable(tmp[i]);
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

std::any EvalVisitor::visitSuite(Parser::SuiteContext *ctx) {
  if (ctx->simple_stmt() != nullptr) {
    return visit(ctx->simple_stmt());
  } else {
    std::vector<Python3Parser::StmtContext *> stmt_vector = ctx->stmt();
    for (int i = 0; i < stmt_vector.size(); i++) {
      std::any tmp = visit(stmt_vector[i]);
      if (tmp.type() == typeid(Control)) {
        return tmp;
      }
    }
  }
  return std::pair<std::string, int>("None", 0);
}

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
    CheckVariable(res);
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
    CheckVariable(res);
    if (!AnyToBool(res)) {
      return (bool)false;
    }
  }
  return (bool)true;
}

std::any EvalVisitor::visitNot_test(Parser::Not_testContext *ctx) {
  if (ctx->not_test() != nullptr) {
    std::any tmp = visit(ctx->not_test());
    CheckVariable(tmp);
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
    CheckVariable(ans);
    CheckVariable(nxt);
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
    CheckVariable(ans);
    CheckVariable(tmp);
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
  // puts("enter : term");
  std::vector<Python3Parser::FactorContext *> factor_vector = ctx->factor();
  std::vector<Python3Parser::Muldivmod_opContext *> muldivmod_op_vector =
      ctx->muldivmod_op();
  std::any ans = visit(factor_vector[0]);
  int sz = factor_vector.size();
  for (int i = 1; i < sz; i++) {
    std::any tmp = visit(factor_vector[i]);
    CheckVariable(ans);
    CheckVariable(tmp);
    std::string op = AnyToString(visit(muldivmod_op_vector[i - 1]));
    if (op == "*") {
      if (ans.type() == typeid(std::string)) {
        std::string str = AnyToString(ans), res = "";
        int2048 len = AnyToInt(tmp);
        // while (len--) {
        while (len.CheckZero() == 0 && len.getsign() == 1) {
          len.minus1();
          res += str;
        }
        ans = res;
      } else if (tmp.type() == typeid(std::string)) {
        std::string str = AnyToString(tmp), res = "";
        int2048 len = AnyToInt(ans);
        // while (len--) {
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
      // puts("operator % ");
      // printf("val1 = %lld , val2 = %lld\n",AnyToInt(ans),AnyToInt(tmp));
      ans = AnyToInt(ans) % AnyToInt(tmp);
      // puts("ok?");
    }
  }
  // int2048 val = std::any_cast<int2048>(ans);
  // std::cout << "term : " << val << '\n';
  // puts("leave term");
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
  // puts("enter factor");
  if (ctx->atom_expr() != nullptr) {
    // std::any tmp = visit(ctx->atom_expr());
    // if (tmp.type() == typeid(std::string))  puts("ok factor");
    // else puts("failed");
    // return tmp;
    return visit(ctx->atom_expr());
  } else if (ctx->ADD() != nullptr) {
    std::any ans = visit(ctx->factor());
    CheckVariable(ans);
    return ans;
    // return visit(ctx->factor());
  } else if (ctx->MINUS() != nullptr) {
    std::any ans = visit(ctx->factor());
    CheckVariable(ans);
    // puts("this is a - factor");
    // if (ans.type() == typeid(int2048))  puts("ok"),printf("val =
    // %lld\n",AnyToInt(ans)); else puts("failed");
    if (ans.type() == typeid(double)) {
      if (AnyToDouble(ans)) {
        ans = -AnyToDouble(ans);
      }
    } else {
      ans = -AnyToInt(ans);
    }
    return ans;
  } else {
    return visit(ctx->factor());
  }
}

std::any EvalVisitor::visitAtom_expr(Parser::Atom_exprContext *ctx) {
  // puts("enter : Atom_expr");
  if (ctx->trailer() != nullptr) {
    std::pair<std::string, int> tmp =
        std::any_cast<std::pair<std::string, int>>(visit(ctx->atom()));
    std::string name = tmp.first;
    std::vector<std::any> trailer_vector =
        std::any_cast<std::vector<std::any>>(visit(ctx->trailer()));
    if (name == "int") {
      return AnyToInt(trailer_vector[0]);
    } else if (name == "str") {
      return AnyToString(trailer_vector[0]);
    } else if (name == "bool") {
      return AnyToBool(trailer_vector[0]);
    } else if (name == "float") {
      return AnyToDouble(trailer_vector[0]);
    } else if (name == "print") {
      // printf("try print\n");
      for (int i = 0; i < (int)trailer_vector.size(); i++) {
        std::any tmp = trailer_vector[i];
        CheckVariable(tmp);
        if (tmp.type() == typeid(std::pair<std::string, int>)) {
          printf("None");
        } else if (tmp.type() == typeid(double)) {
          // puts("type : double");
          printf("%.6lf", AnyToDouble(tmp));
        } else if (tmp.type() == typeid(bool)) {
          // puts("type : bool");
          if (AnyToBool(tmp)) {
            std::cout << "True";
          } else {
            std::cout << "False";
          }
        } else if (tmp.type() == typeid(int2048)) {
          // puts("type : int2048");
          std::cout << AnyToInt(tmp);
        } else if (tmp.type() == typeid(std::string)) {
          // puts("type : string");
          std::string str = AnyToString(tmp);
          int str_sz = str.size();
          for (int j = 0; j < str_sz; j++) {
            if (str[j] == '\\') {
              j++;
              if (str[j] == 'n') {
                std::cout << '\n';
              } else if (str[j] == 't') {
                std::cout << '\t';
              } else {
                std::cout << str[j];
              }
            } else {
              std::cout << str[j];
            }
          }
        }
        if (i != (int)trailer_vector.size() - 1) {
          std::cout << ' ';
        }
      }
      std::cout << '\n';
    } else {
      // puts("try build a function");
      InitFunction(name, trailer_vector);
      // puts("build a function");
      std::any ans = visit(functions[name].suite);
      // puts("visit a function");
      DelVariableStack();
      // puts("delete a function");
      if (ans.type() == typeid(Control)) {
        // puts("Switch Control to val");
        ans = std::any_cast<Control>(ans).return_val;
        // if (ans.type() == typeid(int2048))  puts("type int");
        // else if (ans.type() == typeid(std::pair<std::string,int>))
        // puts("ok"); else if (ans.type() == typeid(bool))  puts("type bool");
        // else if (ans.type() == typeid(std::string)) puts("type string");
        // else puts("failed");
      }
      return ans;
    }
  } else {
    // std::any tmp = visit(ctx->atom());
    // if (tmp.type() == typeid(int2048))  puts("ok");
    // int2048 val = std::any_cast<int2048>(tmp);
    // std::cout << "Atom_expr : " << val << '\n';
    // return tmp;
    return visit(ctx->atom());
  }
  return std::pair<std::string, int>("None", 0);
}

std::any EvalVisitor::visitTrailer(Parser::TrailerContext *ctx) {
  // puts("enter Trailer");
  if (ctx->arglist() != nullptr) {
    return visit(ctx->arglist());
  } else {
    return std::vector<std::any>();
  }
}

std::any EvalVisitor::visitAtom(Parser::AtomContext *ctx) {
  // puts("enter : atom");
  if (ctx->NAME() != nullptr) {
    // puts("this is a NAME");
    return std::pair<std::string, int>(ctx->NAME()->getText(), 1);
  } else if (ctx->NUMBER() != nullptr) {
    std::string str = ctx->NUMBER()->getText();
    // puts("try find number");
    // std::cout << str << '\n';
    bool flag = 0;
    int sz = str.size();
    for (int i = 0; i < sz; i++) {
      if (str[i] == '.') {
        flag = 1;
      }
    }
    if (flag == 1) {
      return StringToDouble(str);
    } else {
      // puts("type = int");
      // int2048 val = std::any_cast<int2048>(StringToInt(str));
      // std::cout << "Atom : " << val << '\n';
      return StringToInt(str);
    }
  } else if (ctx->STRING(0) != nullptr) {
    // puts("try : string");
    std::vector<antlr4::tree::TerminalNode *> string_vector = ctx->STRING();
    std::string ans = "";
    int sz = string_vector.size();
    for (int i = 0; i < sz; i++) {
      std::string str = string_vector[i]->getText();
      // std::cout << "find string : " << str << '\n';
      int str_sz = str.size();
      for (int j = 1; j + 1 < str_sz; j++) {
        ans += str[j];
      }
    }
    // std::cout << "ans : " << ans << '\n';
    return ans;
  } else if (ctx->NONE() != nullptr) {
    // puts("find None");
    // return (std::string)"None";
    return std::pair<std::string, int>("None", 0);
  } else if (ctx->TRUE() != nullptr) {
    return (bool)true;
  } else if (ctx->FALSE() != nullptr) {
    return (bool)false;
  } else if (ctx->test() != nullptr) {
    return visit(ctx->test());
  } else if (ctx->format_string() != nullptr) {
    return visit(ctx->format_string());
  }
  return std::pair<std::string, int>("None", 0);
}

std::any EvalVisitor::visitFormat_string(Parser::Format_stringContext *ctx) {
  int string_sz = (int)ctx->FORMAT_STRING_LITERAL().size();
  int testlist_sz = (int)ctx->testlist().size();
  int pos1 = 0, pos2 = 0;
  std::string ans = "";
  while (pos1 < string_sz && pos2 < testlist_sz) {
    auto index1 =
        ctx->FORMAT_STRING_LITERAL(pos1)->getSymbol()->getTokenIndex();
    auto index2 = ctx->CLOSE_BRACE(pos2)->getSymbol()->getTokenIndex();
    if (index1 < index2) {
      std::string str = ctx->FORMAT_STRING_LITERAL(pos1)->getText();
      int str_sz = (int)str.size();
      for (int i = 0; i < str_sz; i++) {
        if (str[i] == '{' || str[i] == '}') {
          i++;
        }
        ans += str[i];
      }
      pos1++;
    } else {
      std::any res =
          std::any_cast<std::vector<std::any>>(visit(ctx->testlist(pos2)))[0];
      CheckVariable(res);
      ans += AnyToString(res);
      pos2++;
    }
  }
  while (pos1 < string_sz) {
    std::string str = ctx->FORMAT_STRING_LITERAL(pos1)->getText();
    int str_sz = (int)str.size();
    for (int i = 0; i < str_sz; i++) {
      if (str[i] == '{' || str[i] == '}') {
        i++;
      }
      ans += str[i];
    }
    pos1++;
  }
  while (pos2 < testlist_sz) {
    std::any res =
        std::any_cast<std::vector<std::any>>(visit(ctx->testlist(pos2)))[0];
    CheckVariable(res);
    ans += AnyToString(res);
    pos2++;
  }
  return ans;
}

std::any EvalVisitor::visitTestlist(Parser::TestlistContext *ctx) {
  // puts("enter : Testlist");
  // std::cout << ctx->getText() << '\n';
  std::vector<Python3Parser::TestContext *> test_vector = ctx->test();
  std::vector<std::any> ans;
  // printf("sz = %d\n",(int)test_vector.size());
  for (int i = 0; i < test_vector.size(); i++) {
    // std::cout << ctx->test(i)->getText() << '\n';
    ans.push_back(visit(test_vector[i]));
  }
  // puts("leave Testlist");
  return ans;
}

std::any EvalVisitor::visitArglist(Parser::ArglistContext *ctx) {
  // puts("enter Arglist");
  std::vector<Python3Parser::ArgumentContext *> argument_vector =
      ctx->argument();
  int sz = argument_vector.size();
  std::vector<std::any> ans;
  // printf("sz = %d\n",sz);
  for (int i = 0; i < sz; i++) {
    std::any tmp = visit(argument_vector[i]);
    // if (tmp.type() == typeid(int2048))  puts("ok Arglist");
    // else puts("failed Arglist");
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
  // puts("enter Argument");
  std::vector<Python3Parser::TestContext *> test_vector = ctx->test();
  int sz = test_vector.size();
  if (sz == 1) {
    std::any ans = visit(test_vector[0]);
    // std::any tmp = ans;
    // if (tmp.type() == typeid(int2048))  puts("ok Argument");
    // else puts("failed Argument");
    // return tmp;
    CheckVariable(ans);
    // tmp = ans;
    // if (tmp.type() == typeid(int2048))  puts("ok Argument");
    // else puts("failed Argument");
    return ans;
  } else {
    // puts("enter Argument like (A = B)");
    std::any ans = visit(test_vector[1]);
    CheckVariable(ans);
    return std::pair(visit(test_vector[0]), ans);
  }
}