#include "Evalvisitor.h"

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