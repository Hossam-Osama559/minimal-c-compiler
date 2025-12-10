#include "parser.h"


parser::parser(){



}


parser::parser(char * filename){
    current=0;


    lexer lexer_obj(filename);

    token result;


    while (result.kind!=eof){

        result=lexer_obj.next_token();
        if (result.kind!=eof){
        list_of_tokens.push_back(result);
        }
    }

}


bool parser::out_of_tokens(){

    if (current==list_of_tokens.size()){

        return 1;
    }
    return 0;
}






void parser::advance_current(int x){

     current+=x;
}


void parser::decrement_current(int x){

    current-=x;
}


token parser::next_x_token(int x,bool &suitable){

    if ((current+x)<list_of_tokens.size()){
        suitable=1;
        return list_of_tokens[current+x];
    }

    else {

        suitable=0;
        return token();
    }

    
}


bool parser::keyword_type(token tok){

    if (tok.kind==keyword_tok){

        if (tok.keyword_type=="int"||tok.keyword_type=="float"||tok.keyword_type=="char"){
            return 1 ;
        }
        
    }
    return 0;

}



variable_declaration_node::variable_declaration_node(){

    node_type=variable_declaration;
}

translation_unit_node::translation_unit_node(){

    node_type=translation_unit;
}


ast_node::ast_node(){


}


translation_unit_node* parser::parse_translation_unit(){

    cout<<"here"<<endl;
    translation_unit_node* root=new translation_unit_node();

    // vector<ast_node*>childs;

    // root->declarations=childs;

    decalration_seq(root->declarations);

    return root;

}

void parser::decalration_seq(vector<ast_node*> &decl_seq){


    // as long as there more declaration in the program go take it and if not return 

    if (!out_of_tokens()){

        ast_node* res=decalrations();
        decl_seq.push_back(res);
        if (res->err){
            return ;
        }
        decalration_seq(decl_seq);
    }


    return ;


}


ast_node* parser::decalrations(){
cout<<"here2"<<endl;
// there are two options now 
// 1--var decl   int x ,     int x ;      int x =
// 2--func decl  int x (

// will look for the current+2 token
    bool suitable;
    token var_or_fun=next_x_token(2,suitable);

    //there is enough tokens
    if (suitable){

        if (var_or_fun.kind==tk_comma||var_or_fun.kind==tk_semi||var_or_fun.kind==tk_equal){
            cout<<"here3"<<endl;
            // this is variable decl

            ast_node* variable_decl=var_decl();
            return variable_decl;

        }

        else if (var_or_fun.kind==tk_l_paren){
            //this is function decl

            ast_node* function_decl=fun_decl();
            return function_decl;

        }

        else {
            //may be another type of decl but for now will be error 

        }
    }

    //no enough tokens to make current+2
    else {
        // error
        cout<<"error"<<endl;

        ast_node*res=new ast_node();

        res->err=1;
        res->err_msg="decalaration statement must ends with semi colon";
        return res;


    }

}


ast_node* parser::var_decl(){

//consisting of type and then list of init decl 
    
    variable_declaration_node* res=new variable_declaration_node();
    
    bool dummy=1;

    token curr=next_x_token(0,dummy);// this must be a keyword type 

    if (keyword_type(curr)){
        cout<<"here4"<<endl;

        res->type=curr.keyword_type;
        advance_current(1);

    }

    else {
        //error 

        res->err_msg="declaration statments must start with a type specifier";
        res->err=1;
        return res;

    }



    init_decl_list(res->decl_list);
    cout<<"what"<<endl;
    if (res->decl_list.size()&&res->decl_list[res->decl_list.size()-1]->err==1){
        cout<<"what"<<endl;

        res->err=1;
        res->err_msg=res->decl_list[res->decl_list.size()-1]->err_msg;
    }



    return res;



}

ast_node* parser::fun_decl(){



}


void parser::init_decl_list(vector<identifier_initializer*> &init_list){


    bool is_suitable,is_suitable2;
    token curr=next_x_token(0,is_suitable);
    token nex=next_x_token(1,is_suitable2);
    if (is_suitable&&is_suitable2&&curr.kind==identifier&&(nex.kind==tk_semi||nex.kind==tk_comma||nex.kind==tk_equal)){
        identifier_initializer* res=init_decl();
        init_list.push_back(res);
        if (res->err){
            return;
        }
        cout<<"called"<<endl;
        init_decl_list(init_list);
    }

    else if (is_suitable&&!is_suitable2&&curr.kind!=tk_semi) {
        //error
        identifier_initializer* res;
        res->err=1;
        res->err_msg="the variable decleration statement must ends with semi colon";
        init_list.push_back(res);

    }

    else if (!is_suitable&&!is_suitable2) {
        //error
        identifier_initializer* res;
        res->err=1;
        res->err_msg="the variable decleration statement must ends with semi colon";
        init_list.push_back(res);

    }

    else if (is_suitable&&curr.kind==tk_semi){
        advance_current(1);
        return ;
    }

    else if (is_suitable&&is_suitable2&&curr.kind==identifier){
        cout<<"whatt "<<endl;
        identifier_initializer* res=new identifier_initializer;
        res->err=1;
        res->err_msg="the variable decleration statement must ends with semi colon";
        init_list.push_back(res);

    }

    else if (is_suitable&&curr.kind==tk_comma){

         if (is_suitable2&&nex.kind==identifier){
            advance_current(1);
            init_decl_list(init_list);
         }

         else {
            identifier_initializer *res=new identifier_initializer();
            res->err=1;
            res->err_msg="comma should be followed by an identifier";
            init_list.push_back(res);
            return;
         }
    }

    

    return ;

}


identifier_initializer* parser::init_decl(){

    identifier_initializer* res=new identifier_initializer();
    bool dummy;
    res->iden=next_x_token(0,dummy);

    advance_current(1);

    if (next_x_token(0,dummy).kind==tk_equal){
        advance_current(1);

        dummy=0;

        token literal=next_x_token(0,dummy);

        if (dummy&&(literal.kind==numeric_constant||literal.kind==string_literal)){
            res->init=literal;
            res->is_there_init=1;
        }

        else {

            res->err=1;
            res->err_msg="there must be a literal after the equal";
            return res;

        }
        advance_current(1);

        dummy=0;

        token curr=next_x_token(0,dummy);
        // if (dummy&&(curr.kind==tk_comma||curr.kind==tk_semi)){

        //     advance_current(1);
        // }
        // else if (!dummy) {
        //     //error
        //     res->err=1;
        //     res->err_msg="the declaration statement must end with semi colon";
        //     return res;
        // }
    }

        // else if (next_x_token(0,dummy).kind==tk_comma) {
        //     cout<<"here5"<<endl;

        //     advance_current(1);
        // }



    

    return res;

}

int main(){

    parser obj("test.txt");


    translation_unit_node* root=obj.parse_translation_unit();

    cout<<root->declarations.size()<<" "<<root->declarations[0]->node_type<<endl;

    ast_node* first=(root->declarations[0]);

    // for (int i=0;i<first->decl_list.size();i++){

        // cout<<first->decl_list[i]->iden.value.string_value<<" ";
        // cout<<first->decl_list[i]->init.value.string_value<<endl;
        cout<<first->err_msg<<endl;

    // }

    // first=dynamic_cast<variable_declaration_node*>(root->declarations[1]);


    // for (int i=0;i<first->decl_list.size();i++){

    //     cout<<first->decl_list[i]->iden.value.string_value<<" ";
    //     cout<<first->decl_list[i]->init.value.string_value<<endl;

    // }

}