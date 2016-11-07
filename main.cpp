#include <iostream>
#include <string>

using namespace std;

int main(){

  string user_input, first_token;
  while (user_input != "done"){
    getline(cin, user_input);
    first_token = find_first_token(user_input);
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
