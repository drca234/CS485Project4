#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

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
vector<pid_t> procoess_ids;
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
    int ShowTokensIndex = find( 0, variable_names, "ShowTokens" );
    if( variable_values[ ShowTokensIndex ] == 1 ) {
      cout << "You entered the following tokens: \n"
      for( int i = 0; i < tokens.length(); i++ ) {
        cout << token[i] << endl;
      }
    }
    tokens.clear();
  }

}

void tokenize(string user_input){
  string token;
  bool leading_whitespace = false;
  bool 
  for (int i = 0; i < user_input.length(); i++){
    if ( )
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
  // Check to see if the variable has already been declared
  int index = find(variable_names.begin(), variable_names.end(), tokens[1]);
  
  if( index != variable_names.end() ) {
    variable_values[index] = tokens[2]; 
  }
  else {
    variable_names.push_back(tokens[1]);
    variable_values.push_back(tokens[2]);
  }
}

string execute_def_prompt(vector<string> tokens){
  return tokens[1];
}

void execute_cd(vector<string> tokens){
  char *path = &tokens[1][0];
  int retval = chdir(path);
  if (retval < 0){
    cout << "There was an error in changing directories\n";
  }
}

void execute_listprocs(){
  for (int i = 0; i < process_names.size(); i++){
    cout << "Process name:   " << process_names[i] << "   process ID:   " << procoess_ids[i] << endl;
  }
}

void execute_run(vector<string> tokens){
  vector<string> arguments;
  for (int i = 0; i < tokens.size(); i++){
    if (i > 0){
      arguments.push_back(tokens[i]);
    }
  }

  bool background_process = false;
  if (arguments.back() == "&"){
    arguments.pop_back();
    background_process = true;
  }

  const char *exec_command_name;
  char *exec_arguments[1024];
  exec_command_name = arguments[0].c_str();
  for (int i = 0; i < arguments.size(); i++){
    exec_arguments[i] = &arguments[i][0];
    if (i == arguments.size()-1){
      exec_arguments[i+1] = NULL;
    }
  }

  pid_t pid;
  int status;
  if ((pid = fork()) < 0){
    cout << "There was an error forking the process\n";
    exit(1);
  }
  else if (pid == 0){
    if (execvp(exec_command_name, exec_arguments) < 0){
      cout << "There was an error in executing your command\n";
      exit(1);
    }
  }
  else {
    if (!background_process){
      waitpid(pid, &status, 0);
    }
  }

  process_names.push_back(tokens[1]);
  procoess_ids.push_back(pid);
}

void execute_assignto(vector<string> tokens){

}
