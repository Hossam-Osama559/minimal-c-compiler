#include<string>
#include "./../token/token.h"
using namespace std;



class lexer{
public:
    string file_name;


    char *file_buffer; //start of the buffer
    char *buffer_end;
    char *current_char;
    size_t size;

    lexer(char * file_name);
    lexer();

    void advance_current_char();
    
    token next_token();

    bool eof();

    char * next_char();

     
};

class fill_buffer{
public:
fill_buffer(char * &file_buffer,char * file_name,size_t &size);

};



