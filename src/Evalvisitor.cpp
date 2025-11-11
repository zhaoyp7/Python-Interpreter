#include "Evalvisitor.h"

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
  for (auto stmt : ctx->stmt()) {
    visit(stmt);
  }
  return 0;
}
std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {}
std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {}
std::any
EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {}
std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {}
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
  if (ctx->break_stmt() != nullptr) {
    return visit(ctx->break_stmt());
  } else if (ctx->continue_stmt()) {
    return visit(ctx->continue_stmt());
  } else {
    return visit(ctx->return_stmt());
  }
}
std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {}
std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {}
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
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
std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {}
std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {}
std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {}
std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
  return visit(ctx->or_test());
}
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {}
std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {}
std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {}
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {}
std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {}
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