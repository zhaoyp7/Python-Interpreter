#include "Evalvisitor.h"
#include "int2048.h"
#include <any>

bool EvalVisitor::AnyToBool (std::any tmp) {
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
  return 0;
} 
double EvalVisitor::AnyToDouble (std::any tmp) {
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
int2048 EvalVisitor::AnyToInt (std::any tmp) {
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
std::string EvalVisitor::AnyToString (std::any tmp) {
  CheckVariable(tmp);
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
double EvalVisitor::StringToDouble (std::string str) {
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
  for (int i = 0, j = ans.size() - 1; i < j; i++, j--){
    std::swap(ans[i], ans[j]);
  }
  ans += '.';
  tmp = val * 1000000 + 0.5;
  for (int i = 1; i <= 6; i++) {
    ans += tmp % 10 + '0';
    tmp /= 10;
  }
  for (int j = ans.size() - 1, i = j - 5; i < j; i++, j--){
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
};
std::string EvalVisitor::IntToString(int2048 val) {
  return (std::string) val;
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
};
int2048 EvalVisitor::StringToInt (std::string str) {
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
  // puts("enter : InitFunction");
  // std::cout << "Function is " << name << '\n';
  std::map <std::string, int> vis;
  AddVariableStack();
  for (int i = 0; i < (int)val.size(); i++) {
    // CheckVariable(val[i]);
    if (val[i].type() == typeid(std::pair<std::string, int>)) {
      auto tmp = std::any_cast<std::pair<std::string, int>>(val[i]);
      vis[tmp.first] = 1;
      SetValue(tmp.first, tmp.second);
    } else {
      std::string variable_name = functions[name].parameter_list[i].name;
      vis[variable_name] = 1;
      SetValue(variable_name, val[i]);
    }
  }
  for (int i = 0; i < (int)functions[name].parameter_list.size(); i++) {
    std::string variable_name = functions[name].parameter_list[i].name;
    std::any variable_val = functions[name].parameter_list[i].val;
    if (vis.find(variable_name) == vis.end()) {
      SetValue(variable_name, variable_val);
    }
  }
}
bool EvalVisitor::IsVariable(std::string name) {
  if (variables_stack.back().find(name) != variables_stack.back().end()) {
    return 1;
  } else if (variables_stack.front().find(name) != variables_stack.back().end()) {
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
  } else if (variables_stack.front().find(name) != variables_stack.front().end()) {
    variables_stack.front()[name] = val;
  } else {
    variables_stack.back()[name] = val;
  }
}
void EvalVisitor::CheckVariable(std::any &tmp) {
  if (tmp.type() == typeid(std::pair<std::string, int>)) {
    std::pair<std::string, int> temp = std::any_cast<std::pair<std::string, int>>(tmp);
    tmp = GetValue(temp.first);
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
  // puts("enter : Funcdef");
  Function function;
  std::string name = ctx->NAME()->getText();
  // std::cout << "name is " << name << '\n';
  // puts("QwQ");
  function.parameter_list = std::any_cast<std::vector <Parameter>>(visit(ctx->parameters()));
  // int sz = function.parameter_list.size();
  // printf("sz = %d\n",sz);
  // puts("QwQ");
  function.suite = ctx->suite();
  // puts("QwQ");
  functions[name] = function;
  // puts("QwQ");
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
std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
  // puts("enter : Expr_stmt");
  std::vector<Python3Parser::TestlistContext *> testlist_vector = ctx->testlist();
  int sz = testlist_vector.size();
  // printf("sz = %d\n",sz);
  // printf("=sz = %d\n",(int)ctx->ASSIGN().size());
  if (sz == 1) {
    return visit(testlist_vector[0]);
  }
  if (ctx->augassign() != nullptr) {
    // puts("QwQ");
    std::string op = AnyToString(visit(ctx->augassign()));
    // puts("QwQ");
    std::vector <std::any> tmp_name = std::any_cast<std::vector<std::any>>(visit(testlist_vector[0]));
    std::string name = (std::any_cast <std::pair <std::string,int>>(tmp_name[0])).first;
    // std::string name = std::any_cast<std::vector <std::pair<std::string,int>>>(visit(testlist_vector[0]))[0].first;
    // puts("QwQ");
    std::any tmp = std::any_cast<std::vector <std::any>>(visit(testlist_vector[1]))[0];
    // puts("QwQ");
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
      // puts("operator *=");
      if (ans.type() == typeid(std::string)) {
        // puts("operator string *");
        std::string str = AnyToString(ans), res = "";
        int2048 len = AnyToInt(tmp);
        // std::cout << "len = " << len << '\n';
        // printf("%d\n",len.CheckZero());
        // while (len--) {
        while (len.CheckZero() == 0) {
          // puts("QwQ");
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
    // puts("QwQ");
    std::vector <std::any> ans = std::any_cast<std::vector <std::any>>(visit(testlist_vector[sz - 1]));
    // printf("ans.size() = %d\n",(int)ans.size());
    for (int i = 0; i < ans.size(); i++) {
      CheckVariable(ans[i]);
      // if (ans[i].type() == typeid(int2048)) puts("ok type"),printf("%lld\n",std::any_cast<int2048>(ans[i]));
      // else puts("wrong type");
    }
    for (int i = sz - 2; i >= 0; i--) {
      // puts("QwQ");
      std::vector <std::any> tmp = std::any_cast<std::vector<std::any>>(visit(testlist_vector[i]));
      // std::vector <std::pair<std::string,int>> tmp = std::any_cast<std::vector <std::pair<std::string,int>>>(ctx->visit(testlist_vector[i]));
      // puts("QwQ");
      for (int j = 0; j < ans.size(); j++) {
        std::pair <std::string,int> temp = std::any_cast <std::pair <std::string,int>>(tmp[j]);
        // std::cout << temp.first << ' ' << temp.second << '\n';
        SetValue(temp.first,ans[j]);
      }
    }
  }
  return 0 ;
}
std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
  if (ctx->ADD_ASSIGN() != nullptr) {
    return (std::string)"+=";
  } else if (ctx->SUB_ASSIGN() != nullptr) {
    return (std::string)"-=";
  } else if (ctx->MULT_ASSIGN() != nullptr) {
    return (std::string)"*=";
  } else if (ctx->DIV_ASSIGN() != nullptr) {
    return (std::string)"/=";
  } else if (ctx->IDIV_ASSIGN() != nullptr) {
    return (std::string)"//=";
  } else if (ctx->MOD_ASSIGN() != nullptr) {
    return (std::string)"%=";
  }
  return 0;
}
std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
  // puts("enter Flow_stmt");
  if (ctx->break_stmt() != nullptr) {
    return visit(ctx->break_stmt());
  } else if (ctx->continue_stmt()) {
    return visit(ctx->continue_stmt());
  } else if (ctx->return_stmt()) {
    return visit(ctx->return_stmt());
  }
  return 0;
}
std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
  Control ans(2);
  return ans;
}
std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
  Control ans(1);
  return ans;
}
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
  // puts("enter Return_stmt");

  Control ans(3);
  if (ctx->testlist() != nullptr) {
    std::vector <std::any> tmp = std::any_cast<std::vector<std::any>>(visit(ctx->testlist()));
    for (int i = 0; i < (int) tmp.size(); i++) {
      CheckVariable(tmp[i]);
    }
    if (tmp.size() == 1) {
      // if (tmp[0].type() == typeid(int2048)) {
      //   printf("return val = %lld\n",AnyToInt(tmp[0]));
      // }
      Control res(3);
      res.return_val = tmp[0];
      return res;
    }
    ans.return_val = tmp;
  }
  //TODO: 变量 -> 值
  // if (ans.return_val.size() == 1) {
  //   return ans[0];
  // } else {
  return ans;
  // }
}
std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
  if (ctx->if_stmt() != nullptr) {
    return visit(ctx->if_stmt());
  } else if (ctx->while_stmt()) {
    return visit(ctx->while_stmt());
  } else {
    return visit(ctx->funcdef());
  }
}
std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
  // puts("enter If_stmt");
  std::vector<Python3Parser::TestContext *> test_vector = ctx->test();
  std::vector<Python3Parser::SuiteContext *> suite_vector = ctx->suite();
  for (int i = 0; i < (int) test_vector.size(); i++) {
    std::any tmp = visit(test_vector[i]);
    if (AnyToBool(tmp)) {
      // puts("enter if_suite");
      return visit(suite_vector[i]);
    }
  }
  if (test_vector.size() != suite_vector.size()) {
    // puts("enter else");
    std::any res = visit(suite_vector.back());
    // if (res.type() == typeid(Control))  puts("type Control");
    // else if (res.type() == typeid(int2048)) puts("type int");
    // else puts("failed");
    return res;
    
    return visit(suite_vector.back());
  }
  return 0;
}
std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
  // puts("enter While_stmt");
  // std::any res = AnyToBool(visit(ctx->test()));
  // if (res.type() == typeid(bool)) puts("ok");
  // else puts("failed");
  // printf("%d\n",AnyToBool(res));
  while (AnyToBool(visit(ctx->test()))) {
    std::any ans = visit(ctx->suite());
    // puts("visit a suite");
    if (ans.type() == typeid(Control)) {
      // puts("this is a Control");
      Control res = std::any_cast<Control>(ans);
      if (res.op == 2) {
        return 0;
      } else if (res.op == 3) {
        return res;
      }
    }
  }
  // puts("leave While_stmt");
  return 0;
}
std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
  if (ctx->simple_stmt() != nullptr) {
    return visit(ctx->simple_stmt());
  } else {
    std::vector<Python3Parser::StmtContext *> stmt_vector = ctx->stmt();
    for (int i = 0; i < stmt_vector.size(); i++) {
      std::any tmp = visit(stmt_vector[i]);
      if (tmp.type() == typeid(Control)) {
        // puts("this is a Control");
        return tmp;
      }
    }
  }
  return 0;
}
std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
  // puts("enter test");
  // std::any tmp = visit(ctx->or_test());
  // if (tmp.type() == typeid(bool))  
  // {
  //   puts("ok Test");
  //   bool val = std::any_cast<bool>(tmp);
  //   std::cout << "test : " << val << '\n';
  // }
  // else puts("failed Test");
  // return tmp;  

  // std::any res = visit(ctx->or_test());
  // puts("leave test");
  // return res;
  return visit(ctx->or_test());
}
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
  std::vector<Python3Parser::And_testContext *> and_vector = ctx->and_test();
  int sz = and_vector.size();
  if (sz == 1) {
    return visit(and_vector[0]);
  }
  std::any res;
  for (int i = 0; i < sz; i++) {
    res = visit(and_vector[i]);
    if (AnyToBool(res)) {
      return (bool)true;
    }
  }
  // std::any tmp = res;
  // if (tmp.type() == typeid(std::string))  puts("ok Or_test");
  // else puts("failed Or_test");
  // return tmp;  
  return (bool)false;
}
std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
  std::vector<Python3Parser::Not_testContext *> not_vector = ctx->not_test();
  int sz = not_vector.size();
  if (sz == 1) {
    return visit(not_vector[0]);
  }
  std::any res = (bool) true;
  for (int i = 0; i < sz; i++) {
    res = visit(not_vector[i]);
    if (!AnyToBool(res)) {
      return (bool)false;
    }
  }
  // std::any tmp = res;
  // if (tmp.type() == typeid(std::string))  puts("ok And_test");
  // else puts("failed And_test");
  // return tmp;

  return (bool)true;
}
std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
  if (ctx->not_test() != nullptr) {
    std::any tmp = visit(ctx->not_test());
    // if (tmp.type() == typeid(bool)) puts("ok");
    // else if (tmp.type() == typeid(std::pair<std::string,int>)) puts("wrong");
    // else puts("failed");
    CheckVariable(tmp);
    bool res = AnyToBool(tmp);
    // printf("res = %d\n",res);
    return !res;
  } else {
    return visit(ctx->comparison());
  }
}
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
  std::vector<Python3Parser::Arith_exprContext *> arith_expr_vector = ctx->arith_expr();
  std::vector<Python3Parser::Comp_opContext *> comp_op_vector = ctx->comp_op();
  int sz = arith_expr_vector.size();
  std::any ans = visit(arith_expr_vector[0]);
  if (sz == 1) {
    return ans;
  }
  for (int i = 1; i < sz; i++) {
    std::any nxt = visit(arith_expr_vector[i]);
    CheckVariable(ans);
    CheckVariable(nxt);
    std::string op = AnyToString(visit(comp_op_vector[i - 1]));
    if (op == "<") {
      // puts("operator < ");
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) >= AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) >= AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) >= AnyToInt(nxt)) {
        // puts("return bool False");
        return (bool)false;
      }
    } else if (op == ">") {
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) <= AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
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
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) > AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) > AnyToInt(nxt)) {
        return (bool)false;
      }
    } else if (op == ">=") {
      // printf("operator >= ,ans.val = %lld,nxt.val = %lld,res = %d\n",AnyToInt(ans),AnyToInt(nxt),AnyToInt(ans) < AnyToInt(nxt));
      if (ans.type() == typeid(std::string)) {
        if (AnyToString(ans) < AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) < AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) < AnyToInt(nxt)) {
        return (bool)false;
      }
    } else if (op == "==") {
      if (ans.type() == typeid(std::string)) {
        if (nxt.type() != typeid(std::string)) {
          return (bool)false;
        }
        if (AnyToString(ans) != AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (nxt.type() == typeid(std::string)) {
        return (bool)false;
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) != AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) != AnyToInt(nxt)) {
        return (bool)false;
      }
    } else if (op == "!=") {
      if (ans.type() == typeid(std::string)||nxt.type() == typeid(std::string)) {
        if (ans.type() == typeid(std::string)&&nxt.type() == typeid(std::string)&&AnyToString(ans) == AnyToString(nxt)) {
          return (bool)false;
        }
      } else if (ans.type() == typeid(double)||nxt.type() == typeid(double)) {
        if (AnyToDouble(ans) == AnyToDouble(nxt)) {
          return (bool)false;
        }
      } else if (AnyToInt(ans) == AnyToInt(nxt)) {
        return (bool)false;
      }
    }
    ans = nxt;
  }
  // std::any tmp = ans;
  // if (tmp.type() == typeid(std::string))  puts("ok Comparison");
  // else puts("failed Comparison");
  // return tmp;
  // puts("leave Comparison with return val = true");
  return (bool)true;
}
std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
  if (ctx->LESS_THAN() != nullptr) {
    return (std::string)"<";
  } else if (ctx->GREATER_THAN() != nullptr) {
    return (std::string)">";
  } else if (ctx->EQUALS() != nullptr) {
    return (std::string)"==";
  } else if (ctx->GT_EQ() != nullptr) {
    return (std::string)">=";
  } else if (ctx->LT_EQ() != nullptr) {
    return (std::string)"<=";
  } else if (ctx->NOT_EQ_2() != nullptr) {
    return (std::string)"!=";
  } 
  return -1;
}
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
  // puts("enter : Arith_expr");
  std::vector<Python3Parser::TermContext *> term_vector = ctx->term();
  std::vector<Python3Parser::Addorsub_opContext *> addorsub_op_vector = ctx->addorsub_op();
  std::any ans = visit(term_vector[0]);
  // std::any tmp = ans;
  // if (tmp.type() == typeid(int2048))  puts("ok Arith_expr");
  // else puts("failed Arith_expr");
  // return tmp;

  int sz = term_vector.size();
  for (int i = 1; i < sz; i++) {
    std::any tmp = visit(term_vector[i]);
    std::string op = AnyToString(visit(addorsub_op_vector[i - 1]));
    CheckVariable(ans);
    CheckVariable(tmp);
    // puts("QwQ");
    // std::cout << op << '\n';
    if (op == "+") {
      // puts("operator + ");
      if (ans.type() == typeid(std::string) && tmp.type() == typeid(std::string)) {
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
std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {
  if (ctx->ADD() != nullptr) {
    return (std::string)"+";
  } else if (ctx->MINUS() != nullptr) {
    return (std::string)"-";
  }
  return 0;
}
std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
  // puts("enter : term");
  std::vector<Python3Parser::FactorContext *> factor_vector = ctx->factor();
  std::vector<Python3Parser::Muldivmod_opContext *> muldivmod_op_vector = ctx-> muldivmod_op();
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
        while (len.CheckZero() == 0){
          len.minus1();
          res += str;
        }
        ans = res;
      } else if (tmp.type() == typeid(std::string)) {
        std::string str = AnyToString(tmp), res = "";
        int2048 len = AnyToInt(ans);
        // while (len--) {
        while (len.CheckZero() == 0){
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
  return ans ;
}
std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) {
  if (ctx->STAR() != nullptr) {
    return (std::string)"*";
  } else if (ctx->DIV() != nullptr) {
    return (std::string)"/";
  } else if (ctx->IDIV() != nullptr) {
    return (std::string)"//";
  } else if (ctx->MOD() != nullptr) {
    return (std::string)"%";
  }
  return 0;
}
std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
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
    // if (ans.type() == typeid(int2048))  puts("ok"),printf("val = %lld\n",AnyToInt(ans));
    // else puts("failed");
    if (ans.type() == typeid(double)) {
      ans = -AnyToDouble(ans);
    } else {
      ans = -AnyToInt(ans);
    }
    return ans;
  } else {
    return visit(ctx->factor());
  }
}
std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
  // puts("enter : Atom_expr");
  if (ctx->trailer() != nullptr) {
    std::pair <std::string,int> tmp = std::any_cast<std::pair <std::string,int>>(visit(ctx->atom()));
    std::string name = tmp.first;
    std::vector <std::any> trailer_vector = std::any_cast<std::vector <std::any>>(visit(ctx->trailer()));
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
      for (int i = 0; i < (int) trailer_vector.size(); i++) {
        std::any tmp = trailer_vector[i];
        if (tmp.type() == typeid(double)) {
          // puts("type : double");
          printf("%.6lf",AnyToDouble(tmp));
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
        } else if (tmp.type() == typeid(std::string)){
          // puts("type : string");
          std::string str = AnyToString(tmp);
          int str_sz = str.size();
          for (int j = 0; j < str_sz; j++) {
            if (str[j] == '\\') {
              j++;
              if (str[j] == 'n') {
                std::cout << '\n';
              } else {
                std::cout << str[j];
              }
            } else {
              std::cout << str[j];
            }
          }
        }
        if (i != (int) trailer_vector.size() - 1) {
          std::cout << ' ';
        }
      }
      std::cout << '\n';
    } else {
      // puts("try build a function");
      InitFunction(name,trailer_vector);
      // puts("build a function");
      std::any ans = visit(functions[name].suite);
      // puts("visit a function");
      DelVariableStack();
      // puts("delete a function");
      if (ans.type() == typeid(Control)) {
        // puts("Switch Control to val");
        ans = std::any_cast<Control>(ans).return_val;
        // if (ans.type() == typeid(int2048))  puts("ok"),printf("val = %lld\n",AnyToInt(ans));
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
  return 0 ;
}
std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
  // puts("enter Trailer");
  if (ctx->arglist() != nullptr) {
    return visit(ctx->arglist());
  } else {
    return std::vector <std::any> ();
  }
}
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
  // puts("enter : atom");
  if (ctx->NAME() != nullptr) {
    // puts("this is a NAME");
    return std::pair<std::string,int>(ctx->NAME()->getText(),1);
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
    return (std::string)"None";
  } else if (ctx->TRUE() != nullptr) {
    return (bool)true;
  } else if (ctx->FALSE() != nullptr) {
    return (bool)false;
  }  else if (ctx->test() != nullptr) {
    return visit(ctx->test());
  } else if (ctx->format_string() != nullptr) {
    return visit(ctx->format_string());
  }
  return 0;
}
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
  int string_sz = (int) ctx->FORMAT_STRING_LITERAL().size();
  int testlist_sz = (int) ctx->testlist().size();
  int pos1 = 0, pos2 = 0;
  std::string ans = "";
  while (pos1 < string_sz && pos2 < testlist_sz) {
    auto index1 = ctx->FORMAT_STRING_LITERAL(pos1)->getSymbol()->getTokenIndex();
    auto index2 = ctx->CLOSE_BRACE(pos2)->getSymbol()->getTokenIndex();
    if (index1 < index2) {
      std::string str = ctx->FORMAT_STRING_LITERAL(pos1)->getText();
      int str_sz = (int) str.size();
      for (int i = 0; i < str_sz; i++) {
        if (str[i] == '{' || str[i] == '}') {
          i++;
        }
        ans += str[i];
      }
      pos1++;
    } else {
      std::any res = std::any_cast<std::vector <std::any>>(visit(ctx->testlist(pos2)))[0];
      CheckVariable(res);
      ans += AnyToString(res);
      pos2++;
    }
  }
  while (pos1 < string_sz) {
    std::string str = ctx->FORMAT_STRING_LITERAL(pos1)->getText();
    int str_sz = (int) str.size();
    for (int i = 0; i < str_sz; i++) {
      if (str[i] == '{' || str[i] == '}') {
        i++;
      }
      ans += str[i];
    }
    pos1++;
  }
  while (pos2 < testlist_sz) {
    std::any res = std::any_cast<std::vector <std::any>>(visit(ctx->testlist(pos2)))[0];
    CheckVariable(res);
    ans += AnyToString(res);
    pos2++;
  }
  return ans;
}
std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
  // puts("enter : Testlist");
  // std::cout << ctx->getText() << '\n';
  std::vector<Python3Parser::TestContext *> test_vector = ctx->test();
  std::vector <std::any> ans;
  // printf("sz = %d\n",(int)test_vector.size());
  for (int i = 0; i < test_vector.size(); i++) {
    // std::cout << ctx->test(i)->getText() << '\n';
    ans.push_back(visit(test_vector[i]));
  }
  // puts("leave Testlist");
  return ans;
}
std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
  // puts("enter Arglist");
  std::vector<Python3Parser::ArgumentContext *> argument_vector =  ctx->argument();
  int sz = argument_vector.size();
  std::vector <std::any> ans;
  // printf("sz = %d\n",sz);
  for (int i = 0; i < sz; i++) {
    std::any tmp = visit(argument_vector[i]);
    // if (tmp.type() == typeid(int2048))  puts("ok Arglist");
    // else puts("failed Arglist");
    ans.push_back(tmp);
  }
  return ans;
}
std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
  // puts("enter Argument");
  std::vector<Python3Parser::TestContext *> test_vector =  ctx->test();
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