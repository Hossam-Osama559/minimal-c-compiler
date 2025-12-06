#include <iostream>
#include "token.h"
using namespace std;






token::token(){

    
}

token::token(tokenkinds tok_kind,position tok_position){

    this->kind=tok_kind;

    this->token_position=tok_position;
}

tokenkinds token::token_type(){

    return this->kind;
}


keyword::keyword(){

    keyword_numbers=13;

    #define KEYWORD(X,Y) "kw_"#X,

    char * arr[]={
        #include "keywords.def"
    };


    for (int i=0;i<keyword_numbers;i++){

           keywords_token[arr[i]]=1;
    }

}

bool keyword::is_keyword(char * type){

      return keywords_token[type];
}


// int main(){

//      keyword obj;

//      cout<<obj.is_keyword("kw_double");
// }