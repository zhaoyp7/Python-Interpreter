
#include <list>
#include <regex>


// Generated from Python3Lexer.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  Python3Lexer : public antlr4::Lexer {
public:
  enum {
    INDENT = 1, DEDENT = 2, STRING = 3, NUMBER = 4, INTEGER = 5, DEF = 6, 
    RETURN = 7, IF = 8, ELIF = 9, ELSE = 10, WHILE = 11, FOR = 12, IN = 13, 
    OR = 14, AND = 15, NOT = 16, NONE = 17, TRUE = 18, FALSE = 19, CONTINUE = 20, 
    BREAK = 21, NEWLINE = 22, NAME = 23, STRING_LITERAL = 24, FORMAT_STRING_LITERAL = 25, 
    BYTES_LITERAL = 26, DECIMAL_INTEGER = 27, OCT_INTEGER = 28, HEX_INTEGER = 29, 
    BIN_INTEGER = 30, FLOAT_NUMBER = 31, IMAG_NUMBER = 32, DOT = 33, ELLIPSIS = 34, 
    STAR = 35, OPEN_PAREN = 36, CLOSE_PAREN = 37, COMMA = 38, COLON = 39, 
    SEMI_COLON = 40, POWER = 41, ASSIGN = 42, OPEN_BRACK = 43, CLOSE_BRACK = 44, 
    OR_OP = 45, XOR = 46, AND_OP = 47, LEFT_SHIFT = 48, RIGHT_SHIFT = 49, 
    ADD = 50, MINUS = 51, DIV = 52, MOD = 53, IDIV = 54, NOT_OP = 55, OPEN_BRACE = 56, 
    CLOSE_BRACE = 57, LESS_THAN = 58, GREATER_THAN = 59, EQUALS = 60, GT_EQ = 61, 
    LT_EQ = 62, NOT_EQ_1 = 63, NOT_EQ_2 = 64, AT = 65, ARROW = 66, ADD_ASSIGN = 67, 
    SUB_ASSIGN = 68, MULT_ASSIGN = 69, AT_ASSIGN = 70, DIV_ASSIGN = 71, 
    MOD_ASSIGN = 72, AND_ASSIGN = 73, OR_ASSIGN = 74, XOR_ASSIGN = 75, LEFT_SHIFT_ASSIGN = 76, 
    RIGHT_SHIFT_ASSIGN = 77, POWER_ASSIGN = 78, IDIV_ASSIGN = 79, FORMAT_QUOTATION = 80, 
    QUOTATION = 81, SKIP_ = 82, UNKNOWN_CHAR = 83
  };

  explicit Python3Lexer(antlr4::CharStream *input);

  ~Python3Lexer() override;


  private:
  	// A queue where extra tokens are pushed on (see the NEWLINE lexer rule).
  	std::list<antlr4::Token *> tokens;
  private:
  	// The stack that keeps track of the indentation level.
  	std::stack<int> indents;
  private:
  	// The amount of opened braces, brackets and parenthesis.
  	int opened = 0;
  	int format_mode = 0;
  	bool expr_mode = false;
  public:
  	void emit(std::unique_ptr<antlr4::Token> t) override {
  		tokens.push_back(t.get());
  		token.release();
  		token = std::move(t);
  	}

  public:
  	std::unique_ptr<antlr4::Token> nextToken() override {
  		// Check if the end-of-file is ahead and there are still some DEDENTS expected.
  		if (_input->LA(1) == EOF && !this->indents.empty()) {
  			// Remove any trailing EOF tokens from our buffer.
  			for (auto i = tokens.rbegin(); i != tokens.rend();) {
  				auto tmp = i;
  				i++;
  				if ((*tmp)->getType() == EOF) {
  					tokens.erase(tmp.base());
  				}
  			}

  			// First emit an extra line break that serves as the end of the statement.
  			emit(make_CommonToken(Python3Lexer::NEWLINE, "\n"));

  			// Now emit as much DEDENT tokens as needed.
  			while (!indents.empty()) {
  				auto tmp = createDedent();
  				this->emit(std::move(tmp));
  				indents.pop();
  			}

  			// Put the EOF back on the token stream.
  			this->emit(make_CommonToken(static_cast<int>(Python3Lexer::EOF), "<EOF>"));
  		}
  		if (tokens.empty()) {
  			std::unique_ptr<antlr4::Token> next = Lexer::nextToken();
  			next.release();
  			// release it because it should be controlled by 'tokens' now
  		}
  		auto tmp = tokens.front();
  		tokens.pop_front();
  		return std::unique_ptr<antlr4::Token>(tmp);
  	}

  private:
  	std::unique_ptr<antlr4::Token> createDedent() {
  		auto dedent = make_CommonToken(Python3Lexer::DEDENT, "");
  		dedent->setText("DEDENT");
  		return std::move(dedent);
  	}

  private:
  	std::unique_ptr<antlr4::CommonToken> make_CommonToken(int type, std::string const &text) {
  		size_t stop = this->getCharIndex() - 1;
  		size_t start = text.empty() ? stop : stop - text.length() + 1;
  		return std::make_unique<antlr4::CommonToken>(std::make_pair(this, _input), type, DEFAULT_TOKEN_CHANNEL, start, stop);
  	}

  	// Calculates the indentation of the provided spaces, taking the
  	// following rules into account:
  	//
  	// "Tabs are replaced (from left to right) by one to eight spaces
  	//  such that the total number of characters up to and including
  	//  the replacement is a multiple of eight [...]"
  	//
  	//  -- https://docs.python.org/3.1/reference/lexical_analysis.html#indentation
  	static int getIndentationCount(std::string const &spaces) {
  		int count = 0;
  		for (auto ch : spaces)
  			if (ch == '\t') count += 8 - (count % 8);
  			else ++count; // normal space char
  		return count;
  	}
  	bool atStartOfInput() {
  		return Lexer::getCharPositionInLine() == 0 && Lexer::getLine() == 1;
  	}


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  void action(antlr4::RuleContext *context, size_t ruleIndex, size_t actionIndex) override;

  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.
  void NEWLINEAction(antlr4::RuleContext *context, size_t actionIndex);
  void OPEN_PARENAction(antlr4::RuleContext *context, size_t actionIndex);
  void CLOSE_PARENAction(antlr4::RuleContext *context, size_t actionIndex);
  void OPEN_BRACKAction(antlr4::RuleContext *context, size_t actionIndex);
  void CLOSE_BRACKAction(antlr4::RuleContext *context, size_t actionIndex);
  void OPEN_BRACEAction(antlr4::RuleContext *context, size_t actionIndex);
  void CLOSE_BRACEAction(antlr4::RuleContext *context, size_t actionIndex);
  void FORMAT_QUOTATIONAction(antlr4::RuleContext *context, size_t actionIndex);
  void QUOTATIONAction(antlr4::RuleContext *context, size_t actionIndex);

  // Individual semantic predicate functions triggered by sempred() above.
  bool NEWLINESempred(antlr4::RuleContext *_localctx, size_t predicateIndex);
  bool STRING_LITERALSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);
  bool FORMAT_STRING_LITERALSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);
  bool QUOTATIONSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);

};

