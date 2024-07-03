#include <iostream>

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
    else if (input.substr(0, 4) == "echo") echo(input.substr(5, input.length() - 5));
    else {
      std::cout << input << ": command not found\n";
      std::cout << std::unitbuf;
    }
  }
}

void echo(std::string input) {
  std::cout << input << std::endl;
  std::cout << std::unitbuf;
}
