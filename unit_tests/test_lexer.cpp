#include "./../lexer/token_detector.h"



int test_identifier_tok(){


    lexer obj("source_files/test_identifier_tok.txt");
    bool pass=1;
    token result; 
    int iter=0;
    while (result.kind!=eof){

        result=obj.next_token();

        if (iter==0){

            pass&=(result.kind==identifier&&result.err==noerrors&&result.is_key==0&&result.is_there_errors==0&&result.value.string_value=="abc");
        }

    
        else if (iter==1){

            pass&=(result.kind==keyword_tok&&result.err==noerrors&&result.is_key==1&&result.is_there_errors==0&&result.keyword_type=="int");

        }

        else if (iter==2){



            pass&=(result.kind==eof&&result.is_key==0&&result.is_there_errors==0);

        }


        if (pass==0){
           

return iter;    
    }
                iter++;

    }

    return -1;


}




int test_int(){





    lexer obj("source_files/test_int.txt");
    bool pass=1;
    token result; 
    int iter=0;
    while (result.kind!=eof){

        result=obj.next_token();

        if (iter==0){

            pass&=(result.kind==numeric_constant&&result.err==noerrors&&result.is_there_errors==0&&result.value.string_value=="123");

            
            
        }

    
        else if (iter==1){

            pass&=(result.err==wrong_int&&result.is_there_errors==1&&result.value.string_value=="123a\n");

        //    cout<<result.is_there_errors<<" here"<<endl;
        }

        else if (iter==2){



            pass&=(result.kind==eof&&result.is_key==0&&result.is_there_errors==0);

        }


        if (pass==0){
            // cout<<iter<<endl;
           

return iter;    
    }
               iter++;
 
    }

    return -1;


}




int test_string(){





    lexer obj("source_files/test_string.txt");
    bool pass=1;
    token result; 
    int iter=0;
    while (result.kind!=eof){

        result=obj.next_token();

        if (iter==0){

            pass&=(result.kind==string_literal&&result.err==noerrors&&result.is_there_errors==0&&result.value.string_value=="abc");

            
            
        }

    
        else if (iter==1){

            pass&=(result.err==unclosed_string&&result.is_there_errors==1&&result.value.string_value=="abc");

        //    cout<<result.is_there_errors<<" here"<<endl;
        }

        else if (iter==2){



            pass&=(result.kind==string_literal&&result.is_there_errors==0);


        }

        else if (iter==3){

            pass&=(result.kind==string_literal&&result.is_there_errors==0);

        }

        else if (iter==4){
            pass&=(result.kind==eof&&result.is_key==0&&result.is_there_errors==0);

        }

        

        if (pass==0){
            return iter;
           

return 0;    
    }
       iter++; 
    }

    return -1;


}


void run_tests(){
    int res1=test_identifier_tok();
    if (res1==-1){

        cout<<"all tests passed in the test_identifier_tok"<<endl;
    }

    else {

        cout<<"the test "<<res1+1<<" in testing identifier didn't pass"<<endl;
    }


    res1=test_int();
    if (res1==-1){

        cout<<"all tests passed in the int test"<<endl;
    }

    else {

        cout<<"the test "<<res1+1<<" in testing int, didn't pass"<<endl;
    }


    res1=test_string();
    if (res1==-1){

        cout<<"all tests passed in the string testing"<<endl;
    }

    else {

        cout<<"the test "<<res1+1<<" in testing string didn't pass"<<endl;
    }

}

int main(){

    run_tests();
}

