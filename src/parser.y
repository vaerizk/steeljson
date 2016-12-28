%require "3.0"
%skeleton "lalr1.cc"

%defines
%define api.namespace {steeljson}
%define api.value.type variant
%define api.token.constructor
%define parse.assert

%code requires
{
namespace steeljson { class lexer; }

#include "value.h"

#define YY_NULL nullptr
#define YY_NULLPTR nullptr
}

%code
{
#include <memory>
#include <string>
#include <utility>
#include "lexer.h"

#define yylex lexer->tokenize
}

%parse-param { steeljson::lexer* lexer } { steeljson::value* result }

%token NULL_LITERAL
%token TRUE_LITERAL
%token FALSE_LITERAL
%token <std::int64_t> INTEGER_LITERAL
%token <double> FLOATING_POINT_LITERAL
%token <std::string> STRING_LITERAL
%token OBJECT_BEGIN
%token OBJECT_END
%token ARRAY_BEGIN
%token ARRAY_END
%token NAME_SEPARATOR
%token VALUE_SEPARATOR
%token END_DOCUMENT 0

%type <steeljson::value> value
%type <std::pair<std::string,steeljson::value>> member
%type <steeljson::array> array values_list non_empty_values_list
%type <steeljson::object> object members_list non_empty_members_list

%start json

%%
json:
	value END_DOCUMENT { *(this->result) = $1; };

value:
	NULL_LITERAL { $$ = steeljson::value(steeljson::null); };
	| TRUE_LITERAL { $$ = steeljson::value(true); };
	| FALSE_LITERAL { $$ = steeljson::value(false); };
	| INTEGER_LITERAL { $$ = steeljson::value($1); };
	| FLOATING_POINT_LITERAL { $$ = steeljson::value($1); };
	| STRING_LITERAL { $$ = steeljson::value($1); };
	| array { $$ = steeljson::value($1); };
	| object { $$ = steeljson::value($1); };

array: ARRAY_BEGIN values_list ARRAY_END { $$ = $2; };

values_list:
	/* empty list */ { $$ = steeljson::array(); };
	| non_empty_values_list { $$ = $1; };

non_empty_values_list:
	value { $$ = steeljson::array{ $1 }; };
	| non_empty_values_list VALUE_SEPARATOR value {
			$1.push_back($3);
			$$ = $1;
		};

object: OBJECT_BEGIN members_list OBJECT_END { $$ = $2; };

members_list:
	/* empty list */ { $$ = steeljson::object(); };
	| non_empty_members_list { $$ = $1; }

non_empty_members_list:
	member { $$ = steeljson::object{ $1 }; };
	| non_empty_members_list VALUE_SEPARATOR member {
			std::pair<steeljson::object::iterator, bool> result = $1.insert($3);
			if (!result.second) {
				result.first->second = $3.second;
			}
			$$ = $1;
		};

member:
	STRING_LITERAL NAME_SEPARATOR value { $$ = std::make_pair($1, $3); };

%%

void steeljson::parser::error(const std::string& msg) {
	throw syntax_error(msg);
}
