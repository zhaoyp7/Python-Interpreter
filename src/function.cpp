#include "Evalvisitor.h"

void EvalVisitor::InitFunction(std::string name, std::vector<std::any> val) {
  std::map<std::string, int> vis;
  for (int i = 0; i < (int)val.size(); i++) {
    // Resolve situations where variables are used as parameters
    if (val[i].type() == typeid(std::pair<std::any, std::any>)) {
      std::pair<std::any, std::any> tmp =
          std::any_cast<std::pair<std::any, std::any>>(val[i]);
      if (tmp.first.type() != typeid(std::pair<std::string, int>)) {
        VariableToVal(val[i]);
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
      VariableToVal(val[i]);
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