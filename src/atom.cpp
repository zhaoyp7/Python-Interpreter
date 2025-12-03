#include "Evalvisitor.h"

std::any EvalVisitor::visitAtom_expr(Parser::Atom_exprContext *ctx) {
  if (ctx->trailer() != nullptr) { // Function case
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
      for (int i = 0; i < (int)trailer_vector.size(); i++) {
        std::any tmp = trailer_vector[i];
        VariableToVal(tmp);
        if (tmp.type() == typeid(std::pair<std::string, int>)) {
          printf("None");
        } else if (tmp.type() == typeid(double)) {
          printf("%.6lf", AnyToDouble(tmp));
        } else if (tmp.type() == typeid(bool)) {
          if (AnyToBool(tmp)) {
            std::cout << "True";
          } else {
            std::cout << "False";
          }
        } else if (tmp.type() == typeid(int2048)) {
          std::cout << AnyToInt(tmp);
        } else if (tmp.type() == typeid(std::string)) {
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
    } else { // My functions
      InitFunction(name, trailer_vector);
      std::any ans = visit(functions[name].suite);
      DelVariableStack();
      if (ans.type() == typeid(Control)) {
        ans = std::any_cast<Control>(ans).return_val;
      }
      return ans;
    }
  } else {
    return visit(ctx->atom());
  }
  return std::pair<std::string, int>("None", 0);
}

std::any EvalVisitor::visitTrailer(Parser::TrailerContext *ctx) {
  if (ctx->arglist() != nullptr) {
    return visit(ctx->arglist());
  } else {
    return std::vector<std::any>();
  }
}

std::any EvalVisitor::visitAtom(Parser::AtomContext *ctx) {
  if (ctx->NAME() != nullptr) {
    // Using pair to distinguish from str
    return std::pair<std::string, int>(ctx->NAME()->getText(), 1);
  } else if (ctx->NUMBER() != nullptr) {
    std::string str = ctx->NUMBER()->getText();
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
      return int2048(str);
    }
  } else if (ctx->STRING(0) != nullptr) {
    std::vector<antlr4::tree::TerminalNode *> string_vector = ctx->STRING();
    std::string ans = "";
    int sz = string_vector.size();
    for (int i = 0; i < sz; i++) {
      std::string str = string_vector[i]->getText();
      int str_sz = str.size();
      for (int j = 1; j + 1 < str_sz; j++) {
        ans += str[j];
      }
    }
    return ans;
  } else if (ctx->NONE() != nullptr) {
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