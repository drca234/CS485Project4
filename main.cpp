#include <iostream>
#include <string>
#include <vector>

using namespace std;

void tokenize(string);
void execute_set_command(string);
void execute_def_prompt(string);
void execute_cd(string);
void execute_listprocs();
void execute_run(string);
void execute_assignto(string);

vector<string> process_names;
vector<int> procoess_ids;
vector<string> tokens;

vector<string> keys;
vector<string> 
int main(){

  string user_input, first_token;
  while (user_input != "done"){
    getline(cin, user_input);
    first_token = tokens[0];
    if (first_token == "%") {
      //Do nothing, as this is a comment
    }
    if (first_token == "set"){
      execute_set_command(user_input);
    }
    else if (first_token == "defprompt"){
      execute_def_prompt(user_input);
    }
    else if (first_token == "cd"){
      execute_cd(user_input);
    }
    else if (first_token == "listprocs"){
      execute_listprocs();
    }
    else if (first_token == "run"){
      execute_run(user_input);
    }
    else if (first_token == "assignto"){
      execute_assignto(user_input);
    }
    else {
      cout << "Command not recognized\n";
    }
  }

}

<<<<<<< HEAD
void tokenize(string user_input){
  string token;
  bool leading_whitespace = false;
  for (int i = 0; i < user_input.length(); i++){
    if (leading_whitespace == false && user_input[i] != ' '){
      leading_whitespace = true;
      token += user_input[i];
    }
    else if (leading_whitespace == true && user_input[i] == ' '){
      tokens.push_back(token);
      token = "";
    }
    else if (i == user_input.length()-1){
      token += user_input[i];
      tokens.push_back(token);
    }
    else {
      token += user_input[i];
    }
  }
}

void execute_set_command(string user_input){
=======
void execute_set_command(vector<sting> tokens){
>>>>>>> 6c5d551cff4bfdb815951358818903695c717c90

}

void execute_def_prompt(vector<sting> tokens){

}

void execute_cd(vector<sting> tokens){

}

void execute_listprocs(){

}

void execute_run(vector<sting> tokens){

}

void execute_assignto(vector<sting> tokens){

}
