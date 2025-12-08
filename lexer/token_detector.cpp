#include "token_detector.h"

bool token_detector::is_newline(){
    char current=lexer_obj.current_char[0];

    return current=='\n';
}



bool token_detector::is_char(){

    char current=lexer_obj.current_char[0];

    if (current<='z'&&current>='A'){

        return 1 ;
    }
    return 0;
}


bool token_detector::is_number(){
    char current=lexer_obj.current_char[0];

    if (current>='0'&&current<='9'){

        return 1 ;
    }

    return 0;
}


bool token_detector::is_quote(){

    char current=lexer_obj.current_char[0];

    if (current=='"'){

        return 1 ;
    }
    return 0;
}


bool token_detector::is_ws(){

    char current=lexer_obj.current_char[0];

    if (current==' '){

        return 1;
    }
    
    return 0;
}

// consume white spaces or new lines 
void token_detector::consume_ws(){

    while (!lexer_obj.eof()&&(is_ws()||is_newline())){

           lexer_obj.advance_current_char();
    }

   
}


token_detector::token_detector(lexer &curr_lexer):lexer_obj(curr_lexer){
    // cout<<"from the token detector object "<<&curr_lexer<<endl;

    // lexer_obj=curr_lexer;

}


void token_detector::start_lexing(token &result ){

    consume_ws();//consume white spaces and new lines 
    if (!lexer_obj.eof()){



    identifier_tok(result);

    }
    else{
            eof_token(result);

    }
}



void token_detector::eof_token(token &result){

    if (lexer_obj.eof()){
        result.kind=eof;
        result.is_key=0;
        result.is_there_errors=0;
    }
}


// start with letter then any compination of letters and digits 
void token_detector::identifier_tok(token &result){

if (is_char()){
    lexer_obj.add_char_to_current_token_value();

    lexer_obj.advance_current_char();

    while (!lexer_obj.eof()&&(is_char()||is_number())){
        lexer_obj.add_char_to_current_token_value();
        lexer_obj.advance_current_char();
    }


    // here we need to check if the token is identifier or reserved keyword
    if (keyword().is_keyword(lexer_obj.current_token_value.data())) {
        result.is_there_errors=0;
        result.err=noerrors;
        result.is_key=1;
        result.keyword_type=lexer_obj.current_token_value;
        result.kind=keyword_tok;
    }

    else {
        result.err=noerrors;
        result.is_key=0;
        result.is_there_errors=0;
        result.kind=identifier; 
        result.value.string_value=lexer_obj.current_token_value;
    }
    
   
    lexer_obj.clear_current_token_value();
}

else {

    int_const(result);

}

}

void token_detector::int_const(token &result){


    if (is_number()){

    while (!lexer_obj.eof()&&is_number()){
        lexer_obj.add_char_to_current_token_value();

        lexer_obj.advance_current_char();
    }

    if (lexer_obj.eof()||is_ws()||is_newline()){
        result.is_there_errors=0;
        result.err=noerrors;
        result.kind=numeric_constant;
        result.value.string_value=lexer_obj.current_token_value;
        lexer_obj.clear_current_token_value();

    }


    // wrong number 123aa
    else{

        while (!lexer_obj.eof()&&!is_ws()){
            lexer_obj.add_char_to_current_token_value();
            lexer_obj.advance_current_char();
        }
        
        result.is_there_errors=1;
        result.err=wrong_int;
        result.value.string_value=lexer_obj.current_token_value;



        // cout<<"wrong number with value "<<lexer_obj.current_token_value<<endl;
        lexer_obj.clear_current_token_value();
    } 

    }

    // calls the next detector
    else {


        literal_string(result);

    }

}


void token_detector::literal_string(token &result){

 
    if (is_quote()){

        lexer_obj.advance_current_char();

        while (!lexer_obj.eof()&&!is_quote()&&!is_newline()){
            lexer_obj.add_char_to_current_token_value();

            lexer_obj.advance_current_char();
        }

        // not closed string literal like "abc
        if (!is_quote()){
            result.is_there_errors=1;
            result.err=unclosed_string;
            result.value.string_value=lexer_obj.current_token_value;
            // cout<<"not closing string "<<'"'<<lexer_obj.current_token_value<<endl;

        }

        else {
            lexer_obj.advance_current_char();
            result.err=noerrors;
            result.is_there_errors=0;
            result.kind=string_literal;
            result.value.string_value=lexer_obj.current_token_value;
        }

        lexer_obj.clear_current_token_value();
    }

    else {

        l_paren_tok(result);
    }

  

}

void token_detector::l_paren_tok(token &result){

    if (lexer_obj.current_char[0]=='('){
        lexer_obj.advance_current_char();

        result.kind=tk_l_paren;
    }
    else {

        r_paren_tok(result);
    }
}

void token_detector::r_paren_tok(token &result){
    if (lexer_obj.current_char[0]==')'){
        lexer_obj.advance_current_char();

        result.kind=tk_r_paren;
    }
    else {

        l_brace_tok(result);
    }

}


void token_detector::l_brace_tok(token &result){
    if (lexer_obj.current_char[0]=='{'){
        lexer_obj.advance_current_char();

        result.kind=tk_l_brace;
    }
    else {

        r_brace_tok(result);
    }

}


void token_detector::r_brace_tok(token &result){
    if (lexer_obj.current_char[0]=='}'){

        result.kind=tk_r_brace;
        lexer_obj.advance_current_char();

    }
    else {

        equal_tok(result);
    }

}


void token_detector::equal_tok(token &result){
    if (lexer_obj.current_char[0]=='='){

        result.kind=tk_equal;
        lexer_obj.advance_current_char();

    }
    else {

        equalequal(result);
    }

}


void token_detector::equalequal(token &result){
    if (lexer_obj.current_char[0]=='='&&lexer_obj.next_char()&&lexer_obj.next_char()[0]=='='){

        result.kind=tk_equalequal;

        lexer_obj.advance_current_char();
        lexer_obj.advance_current_char();

    }
    else {
            semi(result);
        }


    
}



void token_detector::semi(token &result){


    if (lexer_obj.current_char[0]==';'){

        result.kind=tk_semi;
        result.err=noerrors;
        result.is_there_errors=0;
        result.is_key=0;
        lexer_obj.advance_current_char();
        
    }

    else {

        comma(result);
    }

}


void token_detector::comma(token &result){

    if (lexer_obj.current_char[0]==','){

        result.kind=tk_comma;
        result.err=noerrors;
        result.is_there_errors=0;
        result.is_key=0;
        lexer_obj.advance_current_char();
        
    }

    else {

        undefined_tok(result);
    }

}


void token_detector::undefined_tok(token &result){

    while (!lexer_obj.eof()&&!is_ws()){
        lexer_obj.add_char_to_current_token_value();
        lexer_obj.advance_current_char();
    }

    result.is_there_errors=1;
    result.err=undefined;
    result.value.string_value=lexer_obj.current_token_value;
    lexer_obj.clear_current_token_value();

}