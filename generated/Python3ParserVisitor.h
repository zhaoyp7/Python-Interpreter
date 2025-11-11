
// Generated from Python3Parser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "Python3Parser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by Python3Parser.
 */
class  Python3ParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by Python3Parser.
   */
    virtual std::any visitFile_input(Python3Parser::File_inputContext *context) = 0;

    virtual std::any visitFuncdef(Python3Parser::FuncdefContext *context) = 0;

    virtual std::any visitParameters(Python3Parser::ParametersContext *context) = 0;

    virtual std::any visitTypedargslist(Python3Parser::TypedargslistContext *context) = 0;

    virtual std::any visitTfpdef(Python3Parser::TfpdefContext *context) = 0;

    virtual std::any visitStmt(Python3Parser::StmtContext *context) = 0;

    virtual std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *context) = 0;

    virtual std::any visitSmall_stmt(Python3Parser::Small_stmtContext *context) = 0;

    virtual std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *context) = 0;

    virtual std::any visitAugassign(Python3Parser::AugassignContext *context) = 0;

    virtual std::any visitFlow_stmt(Python3Parser::Flow_stmtContext *context) = 0;

    virtual std::any visitBreak_stmt(Python3Parser::Break_stmtContext *context) = 0;

    virtual std::any visitContinue_stmt(Python3Parser::Continue_stmtContext *context) = 0;

    virtual std::any visitReturn_stmt(Python3Parser::Return_stmtContext *context) = 0;

    virtual std::any visitCompound_stmt(Python3Parser::Compound_stmtContext *context) = 0;

    virtual std::any visitIf_stmt(Python3Parser::If_stmtContext *context) = 0;

    virtual std::any visitWhile_stmt(Python3Parser::While_stmtContext *context) = 0;

    virtual std::any visitSuite(Python3Parser::SuiteContext *context) = 0;

    virtual std::any visitTest(Python3Parser::TestContext *context) = 0;

    virtual std::any visitOr_test(Python3Parser::Or_testContext *context) = 0;

    virtual std::any visitAnd_test(Python3Parser::And_testContext *context) = 0;

    virtual std::any visitNot_test(Python3Parser::Not_testContext *context) = 0;

    virtual std::any visitComparison(Python3Parser::ComparisonContext *context) = 0;

    virtual std::any visitComp_op(Python3Parser::Comp_opContext *context) = 0;

    virtual std::any visitArith_expr(Python3Parser::Arith_exprContext *context) = 0;

    virtual std::any visitAddorsub_op(Python3Parser::Addorsub_opContext *context) = 0;

    virtual std::any visitTerm(Python3Parser::TermContext *context) = 0;

    virtual std::any visitMuldivmod_op(Python3Parser::Muldivmod_opContext *context) = 0;

    virtual std::any visitFactor(Python3Parser::FactorContext *context) = 0;

    virtual std::any visitAtom_expr(Python3Parser::Atom_exprContext *context) = 0;

    virtual std::any visitTrailer(Python3Parser::TrailerContext *context) = 0;

    virtual std::any visitAtom(Python3Parser::AtomContext *context) = 0;

    virtual std::any visitFormat_string(Python3Parser::Format_stringContext *context) = 0;

    virtual std::any visitTestlist(Python3Parser::TestlistContext *context) = 0;

    virtual std::any visitArglist(Python3Parser::ArglistContext *context) = 0;

    virtual std::any visitArgument(Python3Parser::ArgumentContext *context) = 0;


};

