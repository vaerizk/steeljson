%option c++
%option yyclass="steeljson::lexer"
%option nodefault
%option noyywrap

%{
#undef yyFlexLexer

#include <codecvt>
#include <string>
#include "parser.h"
#include "lexer.h"

std::string to_utf8(char32_t code_point);
%}

/* definitions */

/*
" - \x22
\ - \x5C
/ - \x2F
b - \x62
f - \x66
n - \x6E
r - \x72
t - \x74
u - \x75
*/

square_bracket_left \x5B
square_bracket_right \x5D
curly_bracket_left \x7B
curly_bracket_right \x7D
colon \x3A
comma \x2C

null \x6E\x75\x6C\x6C
true \x74\x72\x75\x65
false \x66\x61\x6C\x73\x65

digit [\x30-\x39]
non_zero_digit [\x31-\x39]
int_number \x30|\x2D?{non_zero_digit}+{digit}*
exp [\x45\x65][\x2B\x2D]?{digit}+
fraction \x2E{digit}+
fp_number {int_number}{fraction}?{exp}?

ws \x20|\x09|\x0A|\x0D
ws_sequence {ws}+

hex_digit {digit}|[\x41-\x46]|[\x61-\x66]
unescaped [\x20-\x21]|[\x23-\x5B]|[\x5D-\xFF]
unicode_escape_sequence \x5C\x75{hex_digit}{4}

%x STRING

%%
	/* rules */

%{
std::string result;
%}

<<EOF>> { return steeljson::parser::make_END_DOCUMENT(); }
{square_bracket_left} { return steeljson::parser::make_ARRAY_BEGIN(); }
{square_bracket_right} { return steeljson::parser::make_ARRAY_END(); }
{curly_bracket_left} { return steeljson::parser::make_OBJECT_BEGIN(); }
{curly_bracket_right} { return steeljson::parser::make_OBJECT_END(); }
{colon} { return steeljson::parser::make_NAME_SEPARATOR(); }
{comma} { return steeljson::parser::make_VALUE_SEPARATOR(); }
{ws_sequence} { }
{null} { return steeljson::parser::make_NULL_LITERAL(); }
{true} { return steeljson::parser::make_TRUE_LITERAL(); }
{false} { return steeljson::parser::make_FALSE_LITERAL(); }
{int_number} { return steeljson::parser::make_INTEGER_LITERAL(std::stoll(std::string(this->YYText()))); }
{fp_number} { return steeljson::parser::make_FLOATING_POINT_LITERAL(std::stod(std::string(this->YYText()))); }
\x22 {
		BEGIN(STRING);
		result = std::string();
}
<STRING>\x5C\x22 {
		result.push_back('\x22');
}
<STRING>\x5C\x5C {
		result.push_back('\x5C');
}
<STRING>\x5C\x2F {
		result.push_back('\x2F');
}
<STRING>\x5C\x62 {
		result.push_back('\x08');
}
<STRING>\x5C\x66 {
		result.push_back('\x0C');
}
<STRING>\x5C\x6E {
		result.push_back('\x0A');
}
<STRING>\x5C\x72 {
		result.push_back('\x0D');
}
<STRING>\x5C\x74 {
		result.push_back('\x09');
}
<STRING>{unicode_escape_sequence} {
		const std::string hex_code = std::string(this->YYText()).substr(2);
		result.append(to_utf8(static_cast<char32_t>(std::stoi(hex_code, nullptr, 16))));
}
<STRING>{unescaped} {
		result.append(this->YYText());
}
<STRING>\x22 {
		BEGIN(INITIAL);
		return steeljson::parser::make_STRING_LITERAL(result);
}
<STRING>.|\x0A { throw steeljson::parser::syntax_error("invalid string"); }
. { throw steeljson::parser::syntax_error("input cannot be matched to any rule"); }

%%

std::string to_utf8(char32_t code_point) {
	#if defined _MSC_VER && _MSC_VER <= 2000
	std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> ucs4conv;
	auto p = reinterpret_cast<const int32_t*>(&code_point);
	return ucs4conv.to_bytes(p, p + 1);
	#else
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> ucs4conv;
	return ucs4conv.to_bytes(code_point);
	#endif
}
