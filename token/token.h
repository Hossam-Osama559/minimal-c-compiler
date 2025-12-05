#include <iostream>
#include<map>
using namespace std;




enum tokenkinds :int;

struct position{

int row;
int col;

};

class token{
public:
tokenkinds kind;

position token_position;

token(tokenkinds kind,position token_position);

tokenkinds token_type();
};


class keyword{
public:
int keyword_numbers;
map<char *,bool> keywords_token;

bool is_keyword(char * type);

keyword();

};

