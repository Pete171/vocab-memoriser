#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <algorithm>

// TODO - words.txt is a bad format.  CSV would be better.
// TODO - make filenames/sleep time configurable - i.e. ini file.

class Word {
  std::string m_nativeLanguage;
  std::string m_targetLanguage;
  int m_secondsToWaitBeforeReview;
  std::time_t m_lastSeen;
public:
  Word(std::string nativeLanguage, std::string targetLanguage) :
    m_nativeLanguage(nativeLanguage),
    m_targetLanguage(targetLanguage),
    m_secondsToWaitBeforeReview(0),
    m_lastSeen(std::time(nullptr))
  {}
  
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

void clearScreen() {
  std::cout << "\033[2J\033[1;1H"; // note - OS specific.
}

void handleWord(Word* word) {
  char c;
  std::cout << "Entered correctly?  1 for yes, 0 for no." << std::endl;
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
    handleWord(word);
  }
}

int main() {
  std::ifstream file;
  std::string line;
  std::vector<Word*> allWords;
  Word* word;
  char sep = ' ';
  bool outputDistraction;

  file.open("words.txt");
  while(getline(file, line)) {
    word = split(line, sep);
    allWords.push_back(word);
  }
  file.close();

  file.open("distraction.txt");
  std::string distractionText(
    (std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>()
  );
  
  std::vector<Word*> randomWords;
  
  while (true) {
    for (Word* word : allWords) {
      if (word->canBeDisplayed()) {
	randomWords.push_back(word);
      }
    }

    if (randomWords.size() == 0) {
      if (!outputDistraction) {
	outputDistraction = true;
	clearScreen();
	std::cout << distractionText;
      }
    }
    else {
      std::random_shuffle ( randomWords.begin(), randomWords.end() );

      while (randomWords.size() > 0) {
	outputDistraction = false;

	clearScreen();
	
	word = randomWords.back();
	randomWords.pop_back();
	
	std::cout << word->getNativeLanguage() << std::endl;
	sleep(1);
	std::cout << word->getTargetLanguage() << std::endl;
	handleWord(word);
	std::cout << std::endl;
      }
    }
  }
}
