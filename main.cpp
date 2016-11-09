#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

using namespace std;

void tokenize(string);
int execute_set_command(vector<string>);
string execute_def_prompt(vector<string>);
void execute_cd(vector<string>);
void execute_listprocs();
void execute_run(vector<string>);
void execute_assignto(vector<string>);
void parse(vector<string>);
bool valid_variable(string);

vector<string> tokens;
vector<string> process_names;
vector<pid_t> procoess_ids;
vector<string> variable_names;
vector<string> variable_values;

int main(){

  string user_input, first_token, prompt = "sish > ";
  string initialPath = "/bin:/usr/bin";
  variable_names.push_back("PATH");
  variable_values.push_back(initialPath);
  setenv( "PATH", initialPath, 1 );

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
      if( tokens[1] == "PATH" ) {
        setenv( "PATH", tokens[2], 1 );
      }
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
  bool foundQuote = false;
  for (int i = 0; i < user_input.length(); i++){
    if ( !foundQuote && user_input[i] == '\"' ) {
      foundQuote = true; 
      // Quotes aren't part of the string, so just change the flag.
    }
    else if ( foundQuote && user_input[i] != '\"' ) {
      token += user_input[i];
    }
    else if ( foundQuote && user_input[i] == '\"' ) {
      foundQuote = false;
    }
    else if (leading_whitespace == false && user_input[i] != ' '){
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

void parse( vector<string> tokens ) {
  for( int i = 0; i < tokens.size(); i++ ) {
    if( tokens[i][0] == '$' ) {
      string lookup = "";
      for( int j = 1; j < tokens[i].length(); j++ ) { 
        lookup += tokens[i][j]; 
      }
      int valueLocation = find( 0, variable_names.size(), lookup );
      tokens[i] = variable_values[ valueLocation ];
    }
  }
}

int execute_set_command(vector<string> tokens){
  // Check to see if the variable has already been declared
  int index = find(variable_names.begin(), variable_names.end(), tokens[1]);
  bool valid = valid_variable( token[1] );
  if( valid ) {
    if( index != variable_names.end() ) {
      variable_values[index] = tokens[2]; 
    }
    else {
      variable_names.push_back(tokens[1]);
      variable_values.push_back(tokens[2]);
    }
  }
  else {
    cout << "Invalid variable name. Only letters or numbers may be used.\n";
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

bool valid_variable(string name) {
  bool valid = true;
  for( int i = 0; i < name.length(); i++ ) {
    if( name[i] < '0' | ( name[i] > '9' && name[i] < 'A' ) |
    ( name[i] > 'Z' && name[i] < 'a' ) | name[i] > 'z' ) {
      valid = false;
    }
  }
  return valid;
}