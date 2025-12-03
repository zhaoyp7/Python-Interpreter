#include "Evalvisitor.h"

std::any EvalVisitor::visitFile_input(Parser::File_inputContext *ctx) {
  variables_stack.push_back(std::map<std::string, std::any>());
  for (auto stmt : ctx->stmt()) {
    visit(stmt);
  }
  return std::pair<std::string, int>("None", 0);
}