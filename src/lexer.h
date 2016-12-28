#ifndef STEELJSON_LEXER_H
#define STEELJSON_LEXER_H

#include "FlexLexer.h"
#include "parser.h"

#undef YY_DECL

#define	YY_DECL steeljson::parser::symbol_type steeljson::lexer::tokenize()

namespace steeljson {

	class lexer : public yyFlexLexer {
		public:
			lexer(FLEX_STD istream* arg_yyin = 0, FLEX_STD ostream* arg_yyout = 0) :
				yyFlexLexer(arg_yyin, arg_yyout) { }
			virtual ~lexer() = default;
			virtual steeljson::parser::symbol_type tokenize();
	};

}

#endif // STEELJSON_LEXER_H
