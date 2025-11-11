grammar Python3Demo;

tokens {
	INDENT,
	DEDENT
}

@lexer::members {

  // A queue where extra tokens are pushed on (see the NEWLINE lexer rule).
  private java.util.LinkedList<Token> tokens = new java.util.LinkedList<>();

  // The stack that keeps track of the indentation level.
  private java.util.Stack<Integer> indents = new java.util.Stack<>();

  // The amount of opened braces, brackets and parenthesis.
  private int opened = 0;

  // The most recently produced token.
  private Token lastToken = null;

  @Override
  public void emit(Token t) {
    super.setToken(t);
    tokens.offer(t);
  }

  @Override
  public Token nextToken() {

    // Check if the end-of-file is ahead and there are still some DEDENTS expected.
    if (_input.LA(1) == EOF && !this.indents.isEmpty()) {

      // Remove any trailing EOF tokens from our buffer.
      for (int i = tokens.size() - 1; i >= 0; i--) {
        if (tokens.get(i).getType() == EOF) {
          tokens.remove(i);
        }
      }

      // First emit an extra line break that serves as the end of the statement.
      this.emit(commonToken(Python3DemoParser.NEWLINE, "\n"));

      // Now emit as much DEDENT tokens as needed.
      while (!indents.isEmpty()) {
        this.emit(createDedent());
        indents.pop();
      }

      // Put the EOF back on the token stream.
      this.emit(commonToken(Python3DemoParser.EOF, "<EOF>"));
    }

    Token next = super.nextToken();

    if (next.getChannel() == Token.DEFAULT_CHANNEL) {
      // Keep track of the last token on the default channel.
      this.lastToken = next;
    }

    return tokens.isEmpty() ? next : tokens.poll();
  }

  private Token createDedent() {
    CommonToken dedent = commonToken(Python3DemoParser.DEDENT, "");
    dedent.setLine(this.lastToken.getLine() + 1);
    return dedent;
  }

  private CommonToken commonToken(int type, String text) {
    int stop = this.getCharIndex() - 1;
    int start = text.isEmpty() ? stop : stop - text.length() + 1;
    return new CommonToken(this._tokenFactorySourcePair, type, DEFAULT_TOKEN_CHANNEL, start, stop);
  }

  // Calculates the indentation of the provided spaces, taking the
  // following rules into account:
  //
  // "Tabs are replaced (from left to right) by one to eight spaces
  //  such that the total number of characters up to and including
  //  the replacement is a multiple of eight [...]"
  //
  //  -- https://docs.python.org/3.1/reference/lexical_analysis.html#indentation
  static int getIndentationCount(String spaces) {

    int count = 0;

    for (char ch : spaces.toCharArray()) {
      switch (ch) {
        case '\t':
          count += 8 - (count % 8);
          break;
        default:
          // A normal space char.
          count++;
      }
    }

    return count;
  }

  boolean atStartOfInput() {
    return super.getCharPositionInLine() == 0 && super.getLine() == 1;
  }
  int formatMode = 0;
  boolean exprMode = false;
}

file_input: (NEWLINE | stmt)* EOF;
funcdef: 'def' NAME parameters ':' suite;
parameters: '(' typedargslist? ')';
typedargslist: (tfpdef ('=' test)? (',' tfpdef ('=' test)?)*);
tfpdef: NAME ;

stmt: simple_stmt | compound_stmt;
simple_stmt: small_stmt  NEWLINE;
small_stmt: expr_stmt | flow_stmt;
expr_stmt: testlist ( (augassign testlist) |
                     ('=' testlist)*);//连等 加等/减等/...
augassign: ('+=' | '-=' | '*=' | '/=' | '//=' | '%=' );
flow_stmt: break_stmt | continue_stmt | return_stmt;
break_stmt: 'break';
continue_stmt: 'continue';
return_stmt: 'return' (testlist)?;
compound_stmt: if_stmt | while_stmt | funcdef ;
if_stmt: 'if' test ':' suite ('elif' test ':' suite)* ('else' ':' suite)?;
while_stmt: 'while' test ':' suite;
suite: simple_stmt | NEWLINE INDENT stmt+ DEDENT;
test: or_test ;
or_test: and_test ('or' and_test)*;
and_test: not_test ('and' not_test)*;
not_test: 'not' not_test | comparison;
comparison: arith_expr (comp_op arith_expr)*;
comp_op: '<'|'>'|'=='|'>='|'<=' | '!=';
arith_expr: term (addorsub_op term)*;
addorsub_op: '+'|'-';
term: factor (muldivmod_op factor)*;
muldivmod_op: '*'|'/'|'//'|'%';
factor: ('+'|'-') factor | atom_expr;
atom_expr: atom trailer?;
trailer: '(' (arglist)? ')' ;
atom: (NAME | NUMBER | STRING+| 'None' | 'True' | 'False' | ('(' test ')') | format_string);
format_string: FORMAT_QUOTATION (FORMAT_STRING_LITERAL | '{' testlist '}')* QUOTATION;
testlist: test (',' test)* (',')?;//算式  eg： a,b   a   a+b
arglist: argument (',' argument)*  (',')?;
argument: ( test |
            test '=' test );

STRING: STRING_LITERAL | BYTES_LITERAL;

NUMBER: INTEGER | FLOAT_NUMBER | IMAG_NUMBER;

INTEGER:
	DECIMAL_INTEGER
	| OCT_INTEGER
	| HEX_INTEGER
	| BIN_INTEGER;

DEF: 'def';
RETURN: 'return';
RAISE: 'raise';
FROM: 'from';
IMPORT: 'import';
AS: 'as';
GLOBAL: 'global';
NONLOCAL: 'nonlocal';
ASSERT: 'assert';
IF: 'if';
ELIF: 'elif';
ELSE: 'else';
WHILE: 'while';
FOR: 'for';
IN: 'in';
TRY: 'try';
FINALLY: 'finally';
WITH: 'with';
EXCEPT: 'except';
LAMBDA: 'lambda';
OR: 'or';
AND: 'and';
NOT: 'not';
IS: 'is';
NONE: 'None';
TRUE: 'True';
FALSE: 'False';
CLASS: 'class';
YIELD: 'yield';
DEL: 'del';
PASS: 'pass';
CONTINUE: 'continue';
BREAK: 'break';
ASYNC: 'async';
AWAIT: 'await';

NEWLINE
 : ( {atStartOfInput()}?   SPACES
   | ( '\r'? '\n' | '\r' | '\f' ) SPACES?
   )
   {
     String newLine = getText().replaceAll("[^\r\n\f]+", "");
     String spaces = getText().replaceAll("[\r\n\f]+", "");
     int next = _input.LA(1);

     if (opened > 0 || next == '\r' || next == '\n' || next == '\f' || next == '#') {
       // If we're inside a list or on a blank line, ignore all indents,
       // dedents and line breaks.
       skip();
     }
     else {
       emit(commonToken(NEWLINE, newLine));

       int indent = getIndentationCount(spaces);
       int previous = indents.isEmpty() ? 0 : indents.peek();

       if (indent == previous) {
         // skip indents of the same size as the present indent-size
         skip();
       }
       else if (indent > previous) {
         indents.push(indent);
         emit(commonToken(Python3DemoParser.INDENT, spaces));
       }
       else {
         // Possibly emit more than 1 DEDENT token.
         while(!indents.isEmpty() && indents.peek() > indent) {
           this.emit(createDedent());
           indents.pop();
         }
       }
     }
   }
 ;


/// identifier   ::=  id_start id_continue*
NAME: ID_START ID_CONTINUE*;

/// stringliteral ::= [stringprefix](shortstring | longstring) / stringprefix ::= "r" | "u" | "R" |
// "U" | "f" | "F" / | "fr" | "Fr" | "fR" | "FR" | "rf" | "rF" | "Rf" | "RF"
STRING_LITERAL: {formatMode == 0 || exprMode}? ([rR] | [uU] | ( [fF] [rR]) | ( [rR] [fF]))? (
		SHORT_STRING
		| LONG_STRING
	);

FORMAT_STRING_LITERAL
    : {formatMode > 0 && !exprMode}? (STRING_ESCAPE_SEQ | ~[\\\r\n\f"{}] | '{{' | '}}')+
    ;

/// bytesliteral ::= bytesprefix(shortbytes | longbytes) / bytesprefix ::= "b" | "B" | "br" | "Br" |
// "bR" | "BR" | "rb" | "rB" | "Rb" | "RB"
BYTES_LITERAL: ([bB] | ( [bB] [rR]) | ( [rR] [bB])) (
		SHORT_BYTES
		| LONG_BYTES
	);

/// decimalinteger ::=  nonzerodigit digit* | "0"+
DECIMAL_INTEGER: NON_ZERO_DIGIT DIGIT* | '0'+;

/// octinteger     ::=  "0" ("o" | "O") octdigit+
OCT_INTEGER: '0' [oO] OCT_DIGIT+;

/// hexinteger     ::=  "0" ("x" | "X") hexdigit+
HEX_INTEGER: '0' [xX] HEX_DIGIT+;

/// bininteger     ::=  "0" ("b" | "B") bindigit+
BIN_INTEGER: '0' [bB] BIN_DIGIT+;

/// floatnumber   ::=  pointfloat | exponentfloat
FLOAT_NUMBER: POINT_FLOAT | EXPONENT_FLOAT;

/// imagnumber ::=  (floatnumber | intpart) ("j" | "J")
IMAG_NUMBER: ( FLOAT_NUMBER | INT_PART) [jJ];

DOT: '.';
ELLIPSIS: '...';
STAR: '*';
OPEN_PAREN: '(' {opened++;};
CLOSE_PAREN: ')' {opened--;};
COMMA: ',';
COLON: ':';
SEMI_COLON: ';';
POWER: '**';
ASSIGN: '=';
OPEN_BRACK: '[' {opened++;};
CLOSE_BRACK: ']' {opened--;};
OR_OP: '|';
XOR: '^';
AND_OP: '&';
LEFT_SHIFT: '<<';
RIGHT_SHIFT: '>>';
ADD: '+';
MINUS: '-';
DIV: '/';
MOD: '%';
IDIV: '//';
NOT_OP: '~';
OPEN_BRACE: '{' {opened++; exprMode = true;};
CLOSE_BRACE: '}' {opened--; exprMode = false;};
LESS_THAN: '<';
GREATER_THAN: '>';
EQUALS: '==';
GT_EQ: '>=';
LT_EQ: '<=';
NOT_EQ_1: '<>';
NOT_EQ_2: '!=';
AT: '@';
ARROW: '->';
ADD_ASSIGN: '+=';
SUB_ASSIGN: '-=';
MULT_ASSIGN: '*=';
AT_ASSIGN: '@=';
DIV_ASSIGN: '/=';
MOD_ASSIGN: '%=';
AND_ASSIGN: '&=';
OR_ASSIGN: '|=';
XOR_ASSIGN: '^=';
LEFT_SHIFT_ASSIGN: '<<=';
RIGHT_SHIFT_ASSIGN: '>>=';
POWER_ASSIGN: '**=';
IDIV_ASSIGN: '//=';
FORMAT_QUOTATION
    : 'f"'
    {
        ++formatMode;
        exprMode = false;
    }
    ;
QUOTATION
    : {formatMode > 0  && !exprMode}? '"'
    {
        --formatMode;
        if (formatMode > 0) {
            exprMode = true;
        }
    }
    ;

SKIP_: ( SPACES | COMMENT | LINE_JOINING) -> skip;

UNKNOWN_CHAR: .;

/*
 * fragments
 */

/// shortstring ::= "'" shortstringitem* "'" | '"' shortstringitem* '"' / shortstringitem ::=
// shortstringchar | stringescapeseq / shortstringchar ::= <any source character except "\" or
// newline or the quote>
fragment SHORT_STRING:
	'\'' (STRING_ESCAPE_SEQ | ~[\\\r\n\f'])* '\''
	| '"' ( STRING_ESCAPE_SEQ | ~[\\\r\n\f"])* '"';
/// longstring      ::=  "'''" longstringitem* "'''" | '"""' longstringitem* '"""'
fragment LONG_STRING:
	'\'\'\'' LONG_STRING_ITEM*? '\'\'\''
	| '"""' LONG_STRING_ITEM*? '"""';

/// longstringitem  ::=  longstringchar | stringescapeseq
fragment LONG_STRING_ITEM: LONG_STRING_CHAR | STRING_ESCAPE_SEQ;

/// longstringchar  ::=  <any source character except "\">
fragment LONG_STRING_CHAR: ~'\\';

/// stringescapeseq ::=  "\" <any source character>
fragment STRING_ESCAPE_SEQ: '\\' . | '\\' NEWLINE;

/// nonzerodigit   ::=  "1"..."9"
fragment NON_ZERO_DIGIT: [1-9];

/// digit          ::=  "0"..."9"
fragment DIGIT: [0-9];

/// octdigit       ::=  "0"..."7"
fragment OCT_DIGIT: [0-7];

/// hexdigit       ::=  digit | "a"..."f" | "A"..."F"
fragment HEX_DIGIT: [0-9a-fA-F];

/// bindigit       ::=  "0" | "1"
fragment BIN_DIGIT: [01];

/// pointfloat    ::=  [intpart] fraction | intpart "."
fragment POINT_FLOAT: INT_PART? FRACTION | INT_PART '.';

/// exponentfloat ::=  (intpart | pointfloat) exponent
fragment EXPONENT_FLOAT: ( INT_PART | POINT_FLOAT) EXPONENT;

/// intpart       ::=  digit+
fragment INT_PART: DIGIT+;

/// fraction      ::=  "." digit+
fragment FRACTION: '.' DIGIT+;

/// exponent      ::=  ("e" | "E") ["+" | "-"] digit+
fragment EXPONENT: [eE] [+-]? DIGIT+;

/// shortbytes ::= "'" shortbytesitem* "'" | '"' shortbytesitem* '"' / shortbytesitem ::=
// shortbyteschar | bytesescapeseq
fragment SHORT_BYTES:
	'\'' (SHORT_BYTES_CHAR_NO_SINGLE_QUOTE | BYTES_ESCAPE_SEQ)* '\''
	| '"' (SHORT_BYTES_CHAR_NO_DOUBLE_QUOTE | BYTES_ESCAPE_SEQ)* '"';

/// longbytes      ::=  "'''" longbytesitem* "'''" | '"""' longbytesitem* '"""'
fragment LONG_BYTES:
	'\'\'\'' LONG_BYTES_ITEM*? '\'\'\''
	| '"""' LONG_BYTES_ITEM*? '"""';

/// longbytesitem  ::=  longbyteschar | bytesescapeseq
fragment LONG_BYTES_ITEM: LONG_BYTES_CHAR | BYTES_ESCAPE_SEQ;

/// shortbyteschar ::=  <any ASCII character except "\" or newline or the quote>
fragment SHORT_BYTES_CHAR_NO_SINGLE_QUOTE:
	[\u0000-\u0009]
	| [\u000B-\u000C]
	| [\u000E-\u0026]
	| [\u0028-\u005B]
	| [\u005D-\u007F];

fragment SHORT_BYTES_CHAR_NO_DOUBLE_QUOTE:
	[\u0000-\u0009]
	| [\u000B-\u000C]
	| [\u000E-\u0021]
	| [\u0023-\u005B]
	| [\u005D-\u007F];

/// longbyteschar  ::=  <any ASCII character except "\">
fragment LONG_BYTES_CHAR: [\u0000-\u005B] | [\u005D-\u007F];

/// bytesescapeseq ::=  "\" <any ASCII character>
fragment BYTES_ESCAPE_SEQ: '\\' [\u0000-\u007F];

fragment SPACES: [ \t]+;

fragment COMMENT: '#' ~[\r\n\f]*;

fragment LINE_JOINING: '\\' SPACES? ( '\r'? '\n' | '\r' | '\f');

// TODO: ANTLR seems lack of some Unicode property support... $ curl
// https://www.unicode.org/Public/13.0.0/ucd/PropList.txt | grep Other_ID_ 1885..1886 ;
// Other_ID_Start # Mn [2] MONGOLIAN LETTER ALI GALI BALUDA..MONGOLIAN LETTER ALI GALI THREE BALUDA
// 2118 ; Other_ID_Start # Sm SCRIPT CAPITAL P 212E ; Other_ID_Start # So ESTIMATED SYMBOL
// 309B..309C ; Other_ID_Start # Sk [2] KATAKANA-HIRAGANA VOICED SOUND MARK..KATAKANA-HIRAGANA
// SEMI-VOICED SOUND MARK 00B7 ; Other_ID_Continue # Po MIDDLE DOT 0387 ; Other_ID_Continue # Po
// GREEK ANO TELEIA 1369..1371 ; Other_ID_Continue # No [9] ETHIOPIC DIGIT ONE..ETHIOPIC DIGIT NINE
// 19DA ; Other_ID_Continue # No NEW TAI LUE THAM DIGIT ONE

fragment UNICODE_OIDS:
	'\u1885' ..'\u1886'
	| '\u2118'
	| '\u212e'
	| '\u309b' ..'\u309c';

fragment UNICODE_OIDC:
	'\u00b7'
	| '\u0387'
	| '\u1369' ..'\u1371'
	| '\u19da';

/// id_start     ::=  <all characters in general categories Lu, Ll, Lt, Lm, Lo, Nl, the underscore, and characters with the Other_ID_Start property>
fragment ID_START:
	'_'
	| [\p{L}]
	| [\p{Nl}]
	//| [\p{Other_ID_Start}]
	| UNICODE_OIDS;

/// id_continue  ::=  <all characters in id_start, plus characters in the categories Mn, Mc, Nd, Pc and others with the Other_ID_Continue property>
fragment ID_CONTINUE:
	ID_START
	| [\p{Mn}]
	| [\p{Mc}]
	| [\p{Nd}]
	| [\p{Pc}]
	//| [\p{Other_ID_Continue}]
	| UNICODE_OIDC;