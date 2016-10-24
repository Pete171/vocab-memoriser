#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <exception> //TODO-used? check.
#include <ctime>
#include <unistd.h>

//TODO_ANKI - pow(), random stuff, <utility>'s std::get<0>(vectorObject), more cin and file stuff...

class Word {
  std::string m_nativeLanguage;
  std::string m_targetLanguage;
  int m_secondsToWaitBeforeReview;
  std::time_t m_lastSeen;
public: //TODO-these shouldnt be inline so move them out of here?
  Word(std::string nativeLanguage, std::string targetLanguage) : m_nativeLanguage(nativeLanguage), m_targetLanguage(targetLanguage), m_secondsToWaitBeforeReview(0), m_lastSeen(std::time(nullptr)) {}
  
  void updateSecondsToWait() {
    m_secondsToWaitBeforeReview = m_secondsToWaitBeforeReview == 0 ? 1 : (2 * m_secondsToWaitBeforeReview);
  }
  void resetSecondsToWait() {
    m_secondsToWaitBeforeReview = 1;
  }
  void updateSeenTime() {
    m_lastSeen = std::time(nullptr);
  }

  std::string getNativeLanguage() {
    return m_nativeLanguage;
  }
  std::string getTargetLanguage() {
    return m_targetLanguage;
  }
  int getSecondsToWait() {
    return m_secondsToWaitBeforeReview;
  }
  std::time_t getSeenTime() {
    return m_lastSeen;
  }
  bool canBeDisplayed() {
    return (std::time(nullptr) - getSeenTime()) >= getSecondsToWait();
  }
};
  
// http://www.cplusplus.com/reference/cstdlib/rand/
  
Word* split(const std::string &text, char sep) { // modified from http://stackoverflow.com/questions/236129/split-a-string-in-c
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
  return new Word(tokens[0], tokens[1]);
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
  std::vector<Word*> allWords;
  Word* word;
  //int randInt;//TODO-reintroduce randomness
  char sep = ' ';
  char c;

  srand(time(NULL)); //TODO-look into how this works

  file.open("words2.txt");
  while(getline(file, line)) {
    word = split(line, sep);
    allWords.push_back(word);
  }
  file.close();

  /*
  while (true) {
    randInt = (rand() % allWords.size());

    std::cout << ((std::time(nullptr) - allWords[randInt]->getSeenTime()) >= allWords[randInt]->getSecondsToWait()) << std::endl;//TODO-REMOVE
      
    std::cout << allWords[randInt]->getNativeLanguage() << " " << allWords[randInt]->getTargetLanguage() << std::endl;
    std::cin >> c;

    allWords[randInt]->updateSeenTime();
    allWords[randInt]->updateSecondsToWait();
  }
  */
  while (true) {
    for (Word* word : allWords) {
      if (word->canBeDisplayed()) {
	std::cout << word->getNativeLanguage() << std::endl;
	sleep(1);
	std::cout << word->getTargetLanguage() << std::endl;
	std::cin >> c;
	if (c == '0') {
	  word->updateSeenTime();
	  word->resetSecondsToWait();

	}
	else if (c == '1') {
	  word->updateSeenTime();
	  word->updateSecondsToWait();
	}
	else {
	  throw "Should have entered 0 or 1.";
	}
	std::cout << std::endl;
      }
    }
  }

  //TODO-explicitly delete all word pointers.
}
