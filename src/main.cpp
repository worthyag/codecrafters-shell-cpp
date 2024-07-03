#include <iostream>

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

    std::cout << input << ": command not found\n";
    std::cout << std::unitbuf;
  }
}
