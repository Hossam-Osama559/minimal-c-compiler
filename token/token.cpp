
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

    #define KEYWORD(X,Y) #X,

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







token_name_string::token_name_string(){
    #define TOK(x) #x,
    #define PUNCTUATOR(X,Y) #X,
    #define KEYWORD(X,Y) #X,
    char *arr[]={

        #include "tokenkinds.def"
    };
    #undef TOK


    for (int i=0;i<number_of_tokens;i++){

        lookup[i]=arr[i];
    }
}



// int main(){

//     token tok;

//     tok.kind=numeric_constant;

//     token_name_string obj;

//     cout<<obj.lookup[tok.kind]<<endl;
// }