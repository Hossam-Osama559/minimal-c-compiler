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

function_dec_node::function_dec_node(){

    node_type=function_decl;
}


ast_node::ast_node(){


}


translation_unit_node* parser::parse_translation_unit(){

    // cout<<"here"<<endl;
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
// cout<<"here2"<<endl;
// there are two options now 
// 1--var decl   int x ,     int x ;      int x =
// 2--func decl  int x (

// will look for the current+2 token
    bool suitable;
    token var_or_fun=next_x_token(2,suitable);

    //there is enough tokens
    if (suitable){

        if (var_or_fun.kind==tk_comma||var_or_fun.kind==tk_semi||var_or_fun.kind==tk_equal){
            // cout<<"here3"<<endl;
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
        // cout<<"error"<<endl;

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
        // cout<<"here4"<<endl;

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
    // cout<<"what"<<endl;
    if (res->decl_list.size()&&res->decl_list[res->decl_list.size()-1]->err==1){
        // cout<<"what"<<endl;

        res->err=1;
        res->err_msg=res->decl_list[res->decl_list.size()-1]->err_msg;
    }



    return res;



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
        // cout<<"called"<<endl;
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
        // cout<<"whatt "<<endl;
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


string parser::type_specifier(){
    bool dummy;
    token res=next_x_token(0,dummy);
    advance_current(1);
    return res.is_key?res.keyword_type:"";


}



string parser::declarator(){

    bool dummy;

    token res=next_x_token(0,dummy);

    advance_current(1);

    return res.kind==identifier?res.value.string_value:"";
}


bool parser::parameter_list(vector<ast_node*>&para_list){

    // (variables decl)

    // for now it will be empty para list ()
    advance_current(1);
    bool dummy;

    token curr=next_x_token(0,dummy);

    if (dummy&&curr.kind==tk_r_paren){
        advance_current(1);
        return true;
    }

    return false;


}


bool parser::stmt_seq(vector<ast_node*>&stmts){

    // got 2 options 
    // 1-starting with if so it will be a selection stmt
    // 2-starting with keyword type so it will be var

    bool dummy;

    token curr=next_x_token(0,dummy);

    if (dummy&&keyword_type(curr)){
        // variable
        // cout<<"var"<<endl;
        ast_node*ret=var_decl();
        // cout<<ret->node_type<<endl;
        stmts.push_back(ret);
        if (ret->err){
            
            return false;
        }


    }

    else if (dummy&&curr.keyword_type=="if"){
        // selection stmt
    }

    else if (dummy){
        return true;
    }

    else if (!dummy) {
        return false;
    }

    bool ret=stmt_seq(stmts);

    return ret&1;

}


bool parser::compound_stmt(vector<ast_node*>&body){

    //{seq_stmts}

    bool dummy;

    token curr=next_x_token(0,dummy);

    // starting with {
    if (dummy&&curr.kind==tk_l_brace){

        advance_current(1);
    }

    else {
        false;
    }

    // this will consume all stmts that either variable decl or if stmts 
    bool ret=stmt_seq(body);

    return ret;



}


int parser::return_stmt(bool x){
    bool dummy;
    //x =1 so the function has a not void type 
    if (x){
        //return returned_value_or_ident ;

        token curr=next_x_token(0,dummy);

        if (dummy&&curr.is_key&&curr.keyword_type=="return"){

            advance_current(1);
        }

        else {
            return 0;//not found the return stmt 
        }

        curr=next_x_token(0,dummy);

        if (dummy&&(curr.kind==identifier||curr.kind==numeric_constant||curr.kind==string_literal)){

            advance_current(1);
        }

        else {

            return 1;//the function must return something 
        }

        curr=next_x_token(0,dummy);

        if (dummy&&curr.kind==tk_semi){
            advance_current(1);
            return 2; //valid return stmt
        }

        else {
            

            return 3; //the return stmt must ends with semi colon 
        }


    }

    else {
        //the functoin of void type 
        //return ; or not at all 

    }

}

ast_node* parser::fun_decl(){

    // type-specifier declarator compound-statement

    function_dec_node* res=new function_dec_node();

    string type=type_specifier();

    res->type=type; 

    if (!type.size()){

        res->err=1;
        res->err_msg="the function decaration must starts with a type specifier";
        return res;
    }


    string name=declarator();

    res->name=name; 

    if (!name.size()){

        res->err=1;
        res->err_msg="the function name must be valid identifier";

        return res;
    }

    if (!parameter_list(res->parameter_list)){

        res->err=1;
        res->err_msg="not valid parameter list , the parameter list must starts with ( and ends with )";
        return res;
    }


    bool comp_stmt=compound_stmt(res->body);
    ast_node* last_stmt=new ast_node();

    if (res->body.size()){
        last_stmt=res->body.back();

        if (last_stmt->err){
            res->err=1;
            res->err_msg=last_stmt->err_msg;
            return res;
        }
        
    }

    if (!comp_stmt){

        res->err=1;
        res->err_msg="the function decalration must ends with }";
    }


    // checks for the return stmt



    if (res->type=="void"){

        // so it either empty return or no return at all

    }

    else {
        
        int ret=return_stmt(1);

        if (ret==0){
            res->err=1;
            res->err_msg="exptected the return stmt but found an unsuported stmt";
            return res;
        }

        else if (ret==1){
            res->err=1;
            res->err_msg="the return stmt must have something to return";
            return res;
        }

        else if (ret==3){

            res->err=1;
            res->err_msg="the return stmt must ends with semit colon";
        }

    }

    // checks for the final thing in the compound stmt which the closing brace }


    bool dummy;

    token curr=next_x_token(0,dummy);

    if (dummy&&curr.kind==tk_r_brace){

        return res;
    }

    else {

        res->err=1;

        res->err_msg="the compound stmt of the function must ends with closing brace";
        return res;
    }


    

}



int main(){

    parser obj("test.txt");


    translation_unit_node* root=obj.parse_translation_unit();

    // cout<<root->declarations.size()<<" "<<root->declarations[0]->node_type<<endl;

    function_dec_node* first=dynamic_cast<function_dec_node*>(root->declarations[0]);

    cout<<first->err<<endl<<first->err_msg<<endl;

    // cout<<first->name<<endl<<first->type<<endl<<first->body.size()<<endl;

    // variable_declaration_node* var_decl=dynamic_cast<variable_declaration_node*>(first->body[0]);

    // cout<<var_decl->decl_list[0]->iden.value.string_value<<endl<<var_decl->decl_list[0]->init.value.string_value<<endl;



    // for (int i=0;i<first->decl_list.size();i++){

        // cout<<first->decl_list[i]->iden.value.string_value<<" ";
        // cout<<first->decl_list[i]->init.value.string_value<<endl;
        // cout<<first->err_msg<<endl;

    // }

    // first=dynamic_cast<variable_declaration_node*>(root->declarations[1]);


    // for (int i=0;i<first->decl_list.size();i++){

    //     cout<<first->decl_list[i]->iden.value.string_value<<" ";
    //     cout<<first->decl_list[i]->init.value.string_value<<endl;

    // }

}