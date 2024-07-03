#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

void invalidInput(std::string input, int flag);
void type(std::string input);
void echo(std::string input);

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    // Uncomment this block to pass the first stage
    std::cout << "$ ";
    
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit 0") return 0;
    else if (input.substr(0, 4) == "type") type(input.substr(5));
    else if (input.substr(0, 4) == "echo") echo(input.substr(5));
    else invalidInput(input, 'n');
  }
}

void invalidInput(std::string input, int flag = 1) {
  if (flag == 1) std::cout << input << ": command not found\n";
  else std::cout << input << ": not found\n";
  std::cout << std::unitbuf;
}

void type(std::string input) {
  if (input == "echo" || input == "exit" || input == "type") {
    std::cout << input << " is a shell builtin\n";
    std::cout << std::unitbuf;
    return;
  }

  const char *env {std::getenv("PATH")};

  if (env == nullptr) {
    invalidInput(input, 2);
    return;
  }
  
  std::string pathEnv {env};
  std::stringstream ss {pathEnv};
  std::string path {};

  while (std::getline(ss, path, ':')) {
    std::filesystem::path filePath {std::filesystem::path(path) / input};

    if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath)) {
      std::cout << input << " is " << filePath.string() << '\n';
      std::cout << std::unitbuf;
      return;
    } 
  }

  invalidInput(input, 2);  
}

void echo(std::string input) {
  std::cout << input << std::endl;
  std::cout << std::unitbuf;
}
