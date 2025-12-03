#include "Evalvisitor.h"

std::any EvalVisitor::visitFormat_string(Parser::Format_stringContext *ctx) {
  int string_sz = (int)ctx->FORMAT_STRING_LITERAL().size();
  int testlist_sz = (int)ctx->testlist().size();
  int pos1 = 0, pos2 = 0;
  std::string ans = "";
  while (pos1 < string_sz && pos2 < testlist_sz) {
    auto index1 =
        ctx->FORMAT_STRING_LITERAL(pos1)->getSymbol()->getTokenIndex();
    auto index2 = ctx->CLOSE_BRACE(pos2)->getSymbol()->getTokenIndex();
    if (index1 < index2) { // String case
      std::string str = ctx->FORMAT_STRING_LITERAL(pos1)->getText();
      int str_sz = (int)str.size();
      for (int i = 0; i < str_sz; i++) {
        if (str[i] == '{' || str[i] == '}') {
          i++;
        }
        ans += str[i];
      }
      pos1++;
    } else { // Testlist case
      std::any res =
          std::any_cast<std::vector<std::any>>(visit(ctx->testlist(pos2)))[0];
      VariableToVal(res);
      ans += AnyToString(res);
      pos2++;
    }
  }
  while (pos1 < string_sz) { // String case
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
  while (pos2 < testlist_sz) { // Testlist case
    std::any res =
        std::any_cast<std::vector<std::any>>(visit(ctx->testlist(pos2)))[0];
    VariableToVal(res);
    ans += AnyToString(res);
    pos2++;
  }
  return ans;
}