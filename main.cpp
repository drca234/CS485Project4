#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

using namespace std;

void tokenize(string);
void execute_set_command(vector<char *>);
string execute_def_prompt(vector<char *>);
void execute_cd(vector<char *>);
void execute_listprocs();
void execute_run(vector<char *>);
void execute_assignto(vector<char *>);

vector<char *> tokens;
vector<char *> process_names;
vector<pid_t> procoess_ids;
vector<char *> variable_names;
vector<char *> variable_values;

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

void tokenize(const string user_input){
  char *token = NULL, *duplicate_token = NULL;
  bool leading_whitespace = false;
  int token_index = 0;
  for (int i = 0; i < user_input.length(); i++){
    if (leading_whitespace == false && user_input[i] != ' '){
      leading_whitespace = true;
      token[token_index] = user_input[i];
      token_index += 1;
    }
    else if (leading_whitespace == true && user_input[i] == ' '){
      token[token_index] = '\0';
      duplicate_token = strdup(token);
      tokens.push_back(duplicate_token);
      token_index = 0;
      leading_whitespace = false;
      // memset(token, 0 , strlen(token));
    }
    else if (i == user_input.length()-1){
      token[token_index] = user_input[i];
      tokens.push_back(token);
    }
    else {
      token[token_index] = user_input[i];
      token_index += 1;
    }
  }
}

void execute_set_command(vector<char *> tokens){
  // int index = find(variable_names.begin(), variable_names.end(), tokens[1]);
  //
  // if( index != variable_names.end() ) {
  //   variable_values[index] = tokens[2];
  // }
  variable_names.push_back(tokens[1]);
  variable_values.push_back(tokens[2]);
}

string execute_def_prompt(vector<char *> tokens){
  return tokens[1];
}

void execute_cd(vector<char *> tokens){
  // char path[tokens[1].size()];
  // // path = tokens[1].c_str();
  // strncpy(path, tokens[1].c_str(), tokens[1].size());
  // cout << tokens[1] << endl;
  // cout << path << endl;
  // int retval = chdir(path);
  // if (retval < 0){
  //   cout << "There was an error in changing directories\n";
  // }
}

void execute_listprocs(){}

void execute_run(vector<char *> tokens){
  vector<char *> arguments;
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

  // const char *exec_command_name;
  // int argc = arguments.size();
  // const char *exec_arguments[argc];

  const char *exec_command_name;
  exec_command_name = arguments[0];
  arguments.push_back(NULL);
  char **argv = &arguments[0];

  // for (int i = 0; i < arguments.size(); i++){
  //   for (int j = 0; j < arguments[i].length(); j++){
  //     exec_arguments[i][j] = arguments[i][j];
  //   }
    // exec_arguments[i] = arguments[i].c_str();
  // }


  pid_t pid;
  int status;
  if ((pid = fork()) < 0){
    cout << "There was an error forking the process\n";
    exit(1);
  }
  else if (pid == 0){
    if (execvp(exec_command_name, argv) < 0){
      cout << "There was an error in executing your command\n";
      exit(1);
    }
  }
  else {
    if (!background_process){
      waitpid(pid, &status, 0);
    }
  }

  process_names.push_back(tokens[0]);
  procoess_ids.push_back(pid);
}

void execute_assignto(vector<char *> tokens){

}
