#include "token_detector.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

lexer::lexer(){

    
}

lexer::lexer(char * file_name){

   file_buffer=nullptr;

   fill_buffer obj=fill_buffer(file_buffer,file_name,size);

   current_char=file_buffer;

   buffer_end=file_buffer+size-1;
}


token lexer::next_token(){

    // cout<<"from the lexer object "<<this<<endl;

    token result; 

    token_detector tok_dt(*this);

    tok_dt.start_lexing(result);

    // cout<<&current_char<<" here"<<endl;
    return result;

}

void lexer::advance_current_char(){
    

    current_char++;
    // cout<<"here we are "<<&current_char<<endl;
}


char * lexer::next_char(){

     if (current_char==buffer_end){

        return nullptr;
     }

     return current_char+1;
}


bool lexer::eof(){

    return current_char>buffer_end;
}


fill_buffer::fill_buffer(char * &filebuf,char *file_name,size_t &size){

ifstream file(file_name,ios::binary|ios::ate);


if (!file.is_open()){

cout<<"error";    
}

streamsize file_size=file.tellg();
size = static_cast<std::size_t>(file_size);
// cout<<size<<endl;
file.seekg(0,ios::beg);

filebuf=(char *)malloc(size);

if(filebuf==nullptr){

    cout<<"here"<<endl;
}
if (!file.read(filebuf,file_size)){

    cout<<"problem"<<endl;

}



}





int main(){

    lexer obj("temp");

    // cout<<"here we are from the main stack frame "<<&obj<<endl;

    // cout<<"here we are "<<&obj.current_char<<endl;


    token res=obj.next_token();

    cout<<"done"<<" "<<res.kind<<" "<<identifier<<endl;

    cout<<obj.current_char[1]<<endl;
    res=obj.next_token();
    cout<<"done"<<" "<<res.kind<<" "<<numeric_constant<<endl;
    cout<<obj.current_char[1]<<endl;
    res=obj.next_token();
    cout<<"done"<<" "<<res.kind<<" "<<string_literal<<endl;

}