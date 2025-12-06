#include<iostream>
#include<map>
#include<string>
#include<cstring>
using namespace std;


struct cmp_str {
    bool operator()(const char* a, const char* b) const {
        return std::strcmp(a, b) < 0;
    }
};

enum tokenkinds:int{
#define TOK(X) X,
#define PUNCTUATOR(X,Y) TOK(tk_ ##X)
// #define KEYWORD(X,Y)    TOK(kw_ ## X)

#include "tokenkinds.def"

number_of_tokens
#undef TOK
#undef PUNCTUATOR
// #undef KEYWORD
};

struct position{

int row;
int col;

};



/*

there are some tokens that have values , and the value may be int or may be a string 

*/
struct token_value{

int int_value;

string string_value;

};

class token{
public:
tokenkinds kind;

token_value value;

bool is_key;

string keyword_type;

position token_position;

token(tokenkinds kind,position token_position);

token();

tokenkinds token_type();
};


class keyword{
public:
int keyword_numbers;
map<char *,bool,cmp_str> keywords_token;

bool is_keyword(char * type);

keyword();

};

