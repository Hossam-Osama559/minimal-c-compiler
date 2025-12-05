#include "lexer.h"

class token_detector{
public:
char *current_char;

lexer &lexer_obj;

token_detector(lexer &curr_lexer);


/*

instead of calling each function and then checks the result token to
see whether its been filled to decide to go to the next detector or return 

i will make it as pipeling , each detector will call the next one if
it dosn't detect a token of the detector type

and the pipline will start with the start_lexing function
*/
void start_lexing(token &result);


/*
this function will detect identifiers 
and check if this identifier is a keyword or not 
by calling the functoin is_keyword 
*/
void identifier_tok(token &result); // x 

void int_const(token &result); // 12

void literal_string(token &result); //"here"

void l_paren_tok(token &result); //(

void r_paren_tok(token &result); //)

void l_brace_tok(token &result); //{

void r_brace_tok(token &result); //}


void equal_tok(token &result); // =

void equalequal(token &result); //==

void comma(token &result); //,

void semicolon(token &result);

void greaterequal(token &result); //>=

void greater(token &result); // >

void less(token &result); //<

void lessequal(token &result); //<=

void notequal(token &result); //!=

void slash(token &result); // /

void slashequal(token &result); // /=

void plus(token &result); //+

void minus(token &result);//-

void plusequal(token &result); //+=

void minusequal(token &result); //-=

void is_keyword(token &result);

void consume_ws();

bool is_char();

bool is_number();

bool is_ws();

bool is_quote(); // "

bool is_newline();

};