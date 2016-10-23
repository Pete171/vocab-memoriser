#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <exception>

// http://www.cplusplus.com/reference/cstdlib/rand/
std::pair<std::string, std::string> split(const std::string &text, char sep) { // modified from http://stackoverflow.com/questions/236129/split-a-string-in-c
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  if (tokens.size() != 2) {
    throw "Two words on a line expected.";
  }
  return std::pair<std::string, std::string>(tokens[0], tokens[1]);
}
/*
// TODO - rethink the splitting by char.  we want to be able to work with expressions too.  so probably CSV files would be better and still quick/user friendly to create.
// TODO - then look at implementing the real logic: we want to
//        1- loop through each word once in a random order.
          2 - for each word that was right set it to be shown again in X number of seconds.  for failed words set them to be shown immediately.
          3 - loop.  do SOMETHING if no words to show.  when words ready to be shown, show them.  if they are right show them set X = X*2 (so exponential increase - for now; may want ot play with the numbers) and continue looping.
*/

int main() {
  std::ifstream file;
  std::string line;
  std::vector<std::pair<std::string, std::string>> allWords;
  std::pair<std::string, std::string> pairOfWords;
  int randInt;
  char sep = ' ';
  char c;

  srand(time(NULL)); //TODO-look into how this works

  file.open("words2.txt");
  while(getline(file, line)) {
    pairOfWords = split(line, sep);
    allWords.push_back(pairOfWords);
  }
  file.close();

  while (true) {
    randInt = (rand() % allWords.size());
    std::cout << std::get<0>(allWords[randInt]) << " " << std::get<1>(allWords[randInt]) << std::endl;
    std::cin >> c;
  }
}
