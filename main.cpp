#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

void tokenize(string);
void execute_set_command(vector<string>);
string execute_def_prompt(vector<string>);
void execute_cd(vector<string>);
void execute_listprocs();
void execute_run(vector<string>);
void execute_assignto(vector<string>);

vector<string> tokens;
vector<string> process_names;
vector<int> procoess_ids;
vector<string> variable_names;
vector<string> variable_values;

int main(){

  string user_input, first_token, prompt = "sish > ";
  while (user_input != "done"){
    cout << prompt;
    getline(cin, user_input);
    tokenize(user_input);
    first_token = tokens[0];
    if (first_token == "%") {
      //Do nothing, as this is a comment
      cout << "This is a comment\n";
    }
    else if (first_token == "set"){
      execute_set_command(tokens);
    }
    else if (first_token == "defprompt"){
      prompt = execute_def_prompt(tokens);
    }
    else if (first_token == "cd"){
      execute_cd(tokens);
    }
    else if (first_token == "listprocs"){
      execute_listprocs();
    }
    else if (first_token == "run"){
      execute_run(tokens);
    }
    else if (first_token == "assignto"){
      execute_assignto(tokens);
    }
    else {
      cout << "Command not recognized\n";
    }
  }

}

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

void execute_set_command(vector<string> tokens){
  variable_names.push_back(tokens[1]);
  variable_values.push_back(tokens[2]);
}

string execute_def_prompt(vector<string> tokens){
  return tokens[1];
}

void execute_cd(vector<string> tokens){
  char path[tokens[1].size()];
  // path = tokens[1].c_str();
  strncpy(path, tokens[1].c_str(), tokens[1].size());
  cout << tokens[1] << endl;
  cout << path << endl;
  int retval = chdir(path);
  if (retval < 0){
    cout << "There was an error in changing directories\n";
  }
}

void execute_listprocs(){

}

void execute_run(vector<string> tokens){

}

void execute_assignto(vector<string> tokens){

}
