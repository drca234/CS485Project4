#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;

void tokenize(string);
int execute_set_command(vector<string>);
string execute_def_prompt(vector<string>);
void execute_cd(vector<string>);
void execute_listprocs();
void execute_run(vector<string>);
void execute_assignto(vector<string>);
void parse();
bool valid_variable(string);
int search(vector<string>, string);

vector<string> tokens;
vector<string> process_names;
vector<pid_t> procoess_ids;
vector<string> variable_names;
vector<string> variable_values;

int main(){

  //Setting up the initial values
  string user_input, first_token, prompt = "sish > ";
  string initialPath = "/bin:/usr/bin";
  variable_names.push_back("PATH");
  variable_values.push_back(initialPath);
  setenv( "PATH", initialPath.c_str(), 1 );
  //Showtokens is initially off.
  variable_names.push_back("ShowTokens");
  variable_values.push_back("0");

  while (user_input != "done"){
    cout << prompt;
    getline(cin, user_input);
    tokenize(user_input);
    parse();
    if(tokens.size() > 1) {
      first_token = tokens[0]; // If there was input, get the first token.
    }
    else {
      first_token = ""; // If no input was entered, we set it to empty.
    }
    if (first_token == "%") {
      //Do nothing, as this is a comment
      cout << "This is a comment\n";
    }
    else if (first_token == "set"){
      execute_set_command(tokens);
      if( tokens[1] == "PATH" ) { //If path was updated, 
        setenv( "PATH", tokens[2].c_str(), 1 ); //Update the environment as well
      }
    }
    else if (first_token == "defprompt"){
      prompt = execute_def_prompt(tokens) + " > ";
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
    else if (first_token == "done") {} //Prevents it from counting done as unrecognized.
    else {
      cout << "Command not recognized\n";
    }
    int ShowTokensIndex = search( variable_names, "ShowTokens" );
    if( variable_values[ ShowTokensIndex ] == "1" ) {
      // cout << "You entered the following tokens: \n";
      for( int i = 0; i < tokens.size(); i++ ) {
        cout << tokens[i] << endl;
      } // Output each token on its own line.
    }
    tokens.clear(); // Clear the buffer for the next input.
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
      // If we're in quotes, any input is valid.
    }
    else if ( foundQuote && user_input[i] == '\"' ) {
      foundQuote = false;
      tokens.push_back(token);
      token = "";
    } // Ending quote case
    else if (leading_whitespace == false && user_input[i] != ' '){
      leading_whitespace = true;
      token += user_input[i];
    }
    else if (leading_whitespace == true && user_input[i] == ' '){
      tokens.push_back(token);
      token = "";
    }
    else if (user_input[i] != ' ') {
      token += user_input[i];
    }
  }
  if(token != "") {
    tokens.push_back(token);
  }
}

void parse() {
  for( int i = 0; i < tokens.size(); i++ ) {
    if( tokens[i][0] == '$' ) { // If it starts with a $, it's a variable.
      string lookup = "";
      for( int j = 1; j < tokens[i].length(); j++ ) {
        lookup += tokens[i][j];
      }
      int valueLocation = search( variable_names, lookup );
	    if (valueLocation != -1) { // If the variable has been instantiated, replace the token.
		    tokens[i] = variable_values[valueLocation];
	    }
	    else { // If it hasn't, we treat it as void.
		   tokens[i] = "";
	    }
    }
  }
}

int execute_set_command(vector<string> tokens){
  // Check to see if the variable has already been declared
  int index = search( variable_names, tokens[1]);
  bool valid = valid_variable( tokens[1] ); // Check the validity of the name
  if( valid ) {
    if( index != -1 ) { // If it has, update it.
      variable_values[index] = tokens[2]; 
    }
    else { // If not, add the new variable and value.
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
  } // Building the argument list

  bool background_process = false;
  if (arguments.back() == "&"){
    arguments.pop_back();
    background_process = true;
  } // If it's a background process, don't pass the "&"

  const char *exec_command_name;
  char *exec_arguments[1024];
  exec_command_name = arguments[0].c_str();
  for (int i = 0; i < arguments.size(); i++){
    exec_arguments[i] = &arguments[i][0];
    if (i == arguments.size()-1){
      exec_arguments[i+1] = NULL;
    }
  } // Get the commands into the form for execvp

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
  // Add the names of the process to the process list
  process_names.push_back(tokens[1]);
  procoess_ids.push_back(pid);
}

void execute_assignto(vector<string> tokens){
  vector<string> arguments;
  for (int i = 2; i < tokens.size(); i++){
      arguments.push_back(tokens[i]);
  } // Prepare the arguments to be passed

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
  int status, file, defaultout;

  defaultout = dup(1); // Make a copy to restore stdout from
  close(1); // Close stdout
  if ((file = open("temp", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
	  cout << "There was an error in the setup to execute your command.\n";
  } // Make a temporary file to store the output.
  if ((pid = fork()) < 0){
	dup2(defaultout, 1); // Restore stdout
    cout << "There was an error forking the process\n";
    exit(1);
  }
  else if (pid == 0){
    if (execvp(exec_command_name, exec_arguments) < 0){
      dup2(defaultout, 1); // Restore stdout
      cout << "There was an error in executing your command\n";
      exit(1);
    }
  }
  else {
      waitpid(pid, &status, 0);
  }
  close(file); // Close the file.
  int temp = open("temp", O_RDWR,S_IRUSR | S_IWUSR); // Open the file for reading.
  char buf[1024];
  int bytesread = read(temp, buf, 1024);
  int index = search(variable_names, tokens[1]);
  bool valid = valid_variable( tokens[1] );
  close(file); 
  dup2(defaultout, 1); // Restore stdout
  close(defaultout);
  if( valid ) { // If it's valid, we update or add the variable and value.
    string contents(buf); // Convert the buffer to a string
    if(contents.size() > 0)
      contents.resize(contents.size() - 1); // Gets rid of eof
    if( index != -1 ) {
      variable_values[index] = contents;
    }
    else {
      variable_names.push_back(tokens[1]);
      variable_values.push_back(contents);
    }
  }
  else {
    cout << "Invalid variable name. Only letters or numbers may be used.\n";
  }
  unlink("temp");
}

bool valid_variable(string name) {
  bool valid = true;
  for( int i = 0; i < name.length(); i++ ) {
    if( name[i] < '0' | ( name[i] > '9' && name[i] < 'A' ) |
    ( name[i] > 'Z' && name[i] < 'a' ) | name[i] > 'z' ) {
      valid = false; //If it isn't alphanumeric, it isn't valid.
    }
  }
  return valid;
}

int search(vector<string> vec, string toFind) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == toFind) {
			return i; // Return the position if it's found.
		}
	}
	return -1; // If not found, return -1.
}
