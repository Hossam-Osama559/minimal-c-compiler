#include "./../lexer/token_detector.h"
#include <vector>
// #include <format>

//enum that holds the type specifiers for functions and variables
enum type_specifier{
                 	char_type,
 	                char16_t_type ,    
 	                char32_t_type  ,   
 	                wchar_t_type,
 	                bool_type,
 	                short_type,
 	                int_type,
 	                long_type,
 	                signed_type,
 	                unsigned_type,
 	                float_type,
 	                double_type,
 	                void_type,

};


enum node_types:int{

	variable_declaration,
	translation_unit,
	function_decl


};



/*
this will the base ast_node class that all the node types will inherit from it 
*/
class ast_node{

public:
virtual ~ast_node(){}
ast_node();

node_types node_type;
bool err;
string err_msg;
};
/*
this will form the list of the var decl node 
like this int a , b , c=10;
list of identifiers and each identifier may get initializer or not 
*/
struct identifier_initializer{

    token  iden; // this must be identifier token 

    token init;  // this must be a literal , string or int 

    bool is_there_init;
	bool err;
	string err_msg;
};

class variable_declaration_node :public ast_node{
/*
the var dec will has type 
and then list of identifiers and optionaly their intializers 
*/
public:
variable_declaration_node();

string type; 

vector<identifier_initializer *> decl_list;

};

class translation_unit_node:public ast_node{
public:

/*
the trans unit may holds any types of declarations 
for now may be 
variables declarations 
function definition
*/
translation_unit_node();

vector<ast_node *> declarations;

};



class function_dec_node :public ast_node{
public:

function_dec_node();

string type;

string name;

vector<ast_node*> parameter_list;  //list of parameters 

vector<ast_node*>body;

};



/*
the parser class will construct with non paramatrized constructor and inside the constructor 
it will pull all the tokens from the lexer , so it should get the file name and pass it to the 
lexer and create object of it , and then pull all the tokens and stores in the parser object and 
just has a variable that handles in which token we are now,

the parser class will has a member function one for each grammer rule 
*/
class parser{

public:
    parser(char * filename);
    parser();

    int current; //the current token 

    vector<token>list_of_tokens;

    // the root ast node in c/c++ is the translation unit so the parsing begins from it 
	bool out_of_tokens();

	token next_x_token(int x,bool &suitable);

	void advance_current(int x);

	void decrement_current(int x);

	bool keyword_type(token);



    translation_unit_node* parse_translation_unit(); // the root ast node 

	void decalration_seq(vector<ast_node *> &decl_seq); // will return all declaratoins exist in the program

	ast_node* decalrations();  

	ast_node* var_decl();  // this will return a one var decl stmt to decalrations function 

	ast_node* fun_decl();

	string type_specifier();

	string declarator();

	bool parameter_list(vector<ast_node*> &para_list);

	bool compound_stmt(vector<ast_node*>&stmt_seq);

	bool stmt_seq(vector<ast_node*>&stmts);

	int return_stmt(bool x);// 0 for function of void type and 1 for other types 

	void init_decl_list(vector<identifier_initializer*> &init_list);  

	identifier_initializer* init_decl();  //expecting identifier and optional = then initializer



};


