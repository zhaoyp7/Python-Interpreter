#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include "int2048.h"
#include <any>

// using int2048 = long long;
using Parser = Python3Parser;

struct Parameter {
  std::string name;
  std::any val;
};
struct Function {
  Parser::SuiteContext *suite;
  std::vector<Parameter> parameter_list;
};
struct Control {
  Control() {
    op = 0;
    return_val = std::any();
  }
  Control(int op_val) {
    op = op_val;
    return_val = std::any();
  }
  int op; // 1:continue 2:break 3:return 0:otherwise
  std::any return_val;
};

class EvalVisitor : public Python3ParserBaseVisitor {
private:
  std::vector<std::map<std::string, std::any>> variables_stack;
  std::any GetValue(std::any);
  bool AnyToBool(std::any);
  double AnyToDouble(std::any);
  int2048 AnyToInt(std::any);
  std::string AnyToString(std::any);
  double StringToDouble(const std::string &);
  std::string DoubleToString(double);
  void SetValue(const std::string &, std::any);
  void AddValue(const std::string &, std::any);
  void VariableToVal(std::any &);
  void AddVariableStack() {
    variables_stack.push_back(std::map<std::string, std::any>());
  }
  void DelVariableStack() { variables_stack.pop_back(); }
  void InitFunction(std::string, std::vector<std::any>);
  std::map<std::string, Function> functions;

public:
  std::any visitFile_input(Parser::File_inputContext *) override;
  std::any visitFuncdef(Parser::FuncdefContext *) override;
  std::any visitParameters(Parser::ParametersContext *) override;
  std::any visitTypedargslist(Parser::TypedargslistContext *) override;
  std::any visitTfpdef(Parser::TfpdefContext *) override;
  std::any visitStmt(Parser::StmtContext *) override;
  std::any visitSimple_stmt(Parser::Simple_stmtContext *) override;
  std::any visitSmall_stmt(Parser::Small_stmtContext *) override;
  std::any visitExpr_stmt(Parser::Expr_stmtContext *) override;
  std::any visitAugassign(Parser::AugassignContext *) override;
  std::any visitFlow_stmt(Parser::Flow_stmtContext *) override;
  std::any visitBreak_stmt(Parser::Break_stmtContext *) override;
  std::any visitContinue_stmt(Parser::Continue_stmtContext *) override;
  std::any visitReturn_stmt(Parser::Return_stmtContext *) override;
  std::any visitCompound_stmt(Parser::Compound_stmtContext *) override;
  std::any visitIf_stmt(Parser::If_stmtContext *) override;
  std::any visitWhile_stmt(Parser::While_stmtContext *) override;
  std::any visitSuite(Parser::SuiteContext *) override;
  std::any visitTest(Parser::TestContext *) override;
  std::any visitOr_test(Parser::Or_testContext *) override;
  std::any visitAnd_test(Parser::And_testContext *) override;
  std::any visitNot_test(Parser::Not_testContext *) override;
  std::any visitComparison(Parser::ComparisonContext *) override;
  std::any visitComp_op(Parser::Comp_opContext *) override;
  std::any visitArith_expr(Parser::Arith_exprContext *) override;
  std::any visitAddorsub_op(Parser::Addorsub_opContext *) override;
  std::any visitTerm(Parser::TermContext *) override;
  std::any visitMuldivmod_op(Parser::Muldivmod_opContext *) override;
  std::any visitFactor(Parser::FactorContext *) override;
  std::any visitAtom_expr(Parser::Atom_exprContext *) override;
  std::any visitTrailer(Parser::TrailerContext *) override;
  std::any visitAtom(Parser::AtomContext *) override;
  std::any visitFormat_string(Parser::Format_stringContext *) override;
  std::any visitTestlist(Parser::TestlistContext *) override;
  std::any visitArglist(Parser::ArglistContext *) override;
  std::any visitArgument(Parser::ArgumentContext *) override;
};

#endif // PYTHON_INTERPRETER_EVALVISITOR_H
