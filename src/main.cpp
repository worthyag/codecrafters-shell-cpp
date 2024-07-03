#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

void invalidInput(std::string input, int flag);
std::string getExecutablePath(std::string input, int flag);
void type(std::string input);
void echo(std::string input);
void pwd();
void cd(std::string input);
void externalProgram(std::string input);

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";
    
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit 0") return 0;
    else if (input.substr(0, 4) == "type") type(input.substr(5));
    else if (input.substr(0, 4) == "echo") echo(input.substr(5));
    else if (input.substr(0, 3) == "pwd") pwd();
    else if (input.substr(0, 2) == "cd") cd(input.substr(3));
    else externalProgram(input);
  }
}

void invalidInput(std::string input, int flag = 1) {
  if (flag == 1) std::cout << input << ": command not found\n";
  else std::cout << input << ": not found\n";
  std::cout << std::unitbuf;
}

std::string getExecutablePath(std::string input, int flag) {
  const char *env {std::getenv("PATH")};

  if (env == nullptr) {
    invalidInput(input, flag);
    return "";
  }
  
  std::string pathEnv {env};
  std::stringstream ss {pathEnv};
  std::string path {};

  while (std::getline(ss, path, ':')) {
    std::filesystem::path filePath {std::filesystem::path(path) / input};

    if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath)) {
      return filePath.string();
    } 
  }

  invalidInput(input, flag);
  return "";
}

void type(std::string input) {
  if (input == "echo" || input == "exit" || input == "type" || input == "pwd") {
    std::cout << input << " is a shell builtin\n";
    std::cout << std::unitbuf;
    return;
  }

  std::string path {getExecutablePath(input, 2)};

  if (path == "") return;
  else {
    std::cout << input << " is " << path << '\n';
    std::cout << std::unitbuf;
  } 
}

void echo(std::string input) {
  std::cout << input << std::endl;
  std::cout << std::unitbuf;
}

void pwd() {
  std::cout << std::filesystem::current_path().string() << std::endl;
  std::cout << std::unitbuf;
}

void cd(std::string input) {
  std::filesystem::path path {};

  if (input[0] == '~') {
    std::string home {std::getenv("HOME")};
    path += std::filesystem::path{home};
    input = input.substr(1);
  }

  path += std::filesystem::path{input};

  try {
    std::filesystem::current_path(path);
  } catch(std::filesystem::filesystem_error) {
    std::cout << "cd: " << input << ": No such file or directory\n";
    std::cout << std::unitbuf;
  }
}

void externalProgram(std::string input) {
  std::string name {};
  std::string argument {};
  std::vector<std::string> arguments {};
  std::stringstream ss {input};
  bool first {true};

  while (std::getline(ss, argument, ' ')) {
    if (first) {
      name = argument;
      first = false;
    } else arguments.push_back(argument);
  }

  std::string path {getExecutablePath(name, 1)};

  if (path.empty()) return;

  // Forking a child process
  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Child Process.
    // Preparing arguments for execvp.
    std::vector<char*> args {arguments.size() + 2};
    // The first argument is the program path.
    args[0] = const_cast<char*>(path.c_str());

    for (size_t i = 0; i < arguments.size(); ++i) {
      args[i + 1] = const_cast<char*>(arguments[i].c_str());
    }

    // Null terminator for execvp.
    args[arguments.size() + 1] = nullptr;

    if ((execvp(path.c_str(), args.data())) == -1) {
      perror("execvp");
      exit(EXIT_FAILURE);
    }
  } else {
    // Parent process.
    int status {};
    // Waiting for the child process to complete.
    waitpid(pid, &status, 0);
  }
}
