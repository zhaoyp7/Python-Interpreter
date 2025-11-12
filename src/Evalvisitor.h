#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3ParserBaseVisitor.h"
#include <any>

using int2048 = long long;

struct Parameter{
	std::string name;
	std::any val;
};
struct Function{
	Python3Parser::SuiteContext *suite;
	std::vector <Parameter> parameter_list;
};

bool AnyToBool (std::any) ;
double AnyToDouble (std::any) ;
int2048 AnyToInt (std::any) ;
std::string AnyToString (std::any) ;
double StringToDouble (std::string) ;
std::string DoubleToString(double) ;
std::string IntToString(int2048) ;
int2048 StringToInt(std::string) ;

class EvalVisitor : public Python3ParserBaseVisitor {
private:
  std::vector <std::map<std::string, std::any>> variables_stack;
  std::any GetValue(std::any);
  std::map<std::string, Function> functions;
public:
	// TODO: override all methods of Python3ParserBaseVisitor
	std::any visitFile_input(Python3Parser::File_inputContext *) override ;
	std::any visitFuncdef(Python3Parser::FuncdefContext *) override ;
	std::any visitParameters(Python3Parser::ParametersContext *) override ;
	std::any visitTypedargslist(Python3Parser::TypedargslistContext *) override ;
	std::any visitTfpdef(Python3Parser::TfpdefContext *) override ;
	std::any visitStmt(Python3Parser::StmtContext *) override ;
	std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *) override ;
	std::any visitSmall_stmt(Python3Parser::Small_stmtContext *) override ;
	std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *) override ;
	std::any visitAugassign(Python3Parser::AugassignContext *) override ;
	std::any visitFlow_stmt(Python3Parser::Flow_stmtContext *) override ;
	std::any visitBreak_stmt(Python3Parser::Break_stmtContext *) override ;
	std::any visitContinue_stmt(Python3Parser::Continue_stmtContext *) override ;
	std::any visitReturn_stmt(Python3Parser::Return_stmtContext *) override ;
	std::any visitCompound_stmt(Python3Parser::Compound_stmtContext *) override ;
	std::any visitIf_stmt(Python3Parser::If_stmtContext *) override ;
	std::any visitWhile_stmt(Python3Parser::While_stmtContext *) override ;
	std::any visitSuite(Python3Parser::SuiteContext *) override ;
	std::any visitTest(Python3Parser::TestContext *) override ;
	std::any visitOr_test(Python3Parser::Or_testContext *) override ;
	std::any visitAnd_test(Python3Parser::And_testContext *) override ;
	std::any visitNot_test(Python3Parser::Not_testContext *) override ;
	std::any visitComparison(Python3Parser::ComparisonContext *) override ;
	std::any visitComp_op(Python3Parser::Comp_opContext *) override ;
	std::any visitArith_expr(Python3Parser::Arith_exprContext *) override ;
	std::any visitAddorsub_op(Python3Parser::Addorsub_opContext *) override ;
	std::any visitTerm(Python3Parser::TermContext *) override ;
	std::any visitMuldivmod_op(Python3Parser::Muldivmod_opContext *) override ;
	std::any visitFactor(Python3Parser::FactorContext *) override ;
	std::any visitAtom_expr(Python3Parser::Atom_exprContext *) override ;
	std::any visitTrailer(Python3Parser::TrailerContext *) override ;
	std::any visitAtom(Python3Parser::AtomContext *) override ;
	std::any visitFormat_string(Python3Parser::Format_stringContext *) override ;
	std::any visitTestlist(Python3Parser::TestlistContext *) override ;
	std::any visitArglist(Python3Parser::ArglistContext *) override ;
	std::any visitArgument(Python3Parser::ArgumentContext *) override ;
};


#endif//PYTHON_INTERPRETER_EVALVISITOR_H
