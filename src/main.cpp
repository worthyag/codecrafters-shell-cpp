#include <iostream>
#include <string>
#include <array>

bool isValidCommand(std::string input);
std::string getResponse(std::string command);

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    // Uncomment this block to pass the first stage
    std::cout << "$ ";
    
    std::string input;
    std::getline(std::cin, input);

    // Simple solution.
    // std::cout << input << ": command not found";
    // std::cout << std::unitbuf;

    std::cout << getResponse(input) << std::endl;
    std::cout << std::unitbuf;
  }
}

bool isValidCommand(std::string input) {
  std::string commands[] {"echo", "cd"};
  std::string* end {commands + (sizeof(commands) / sizeof(commands[0]))};

  for (std::string* ptr = commands; ptr != end; ++ptr) {
    if (*ptr == input) return true;
  }

  return false;
}

std::string getResponse(std::string command) {
  if (!isValidCommand(command)) return (command + ": command not found");
  else return "processing";
}
