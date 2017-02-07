#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <algorithm>

#include "csv_reader.h"

// TODO - words.txt is a bad format.  CSV would be better.
// TODO - make filenames/sleep time configurable - i.e. ini file.
// TODO - keep track of words that have 'finished' past a max number of iterations and exit if all words are 'finished'.  if we timeout keep track of the words that were still failing and print them for further study.

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
    m_secondsToWaitBeforeReview = m_secondsToWaitBeforeReview == 0 ? 1 : (3 * m_secondsToWaitBeforeReview);
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
  
Word* split(const std::string &text, char sep, bool reverse) { // modified from http://stackoverflow.com/questions/236129/split-a-string-in-c
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
  if (!reverse) {
    return new Word(tokens[0], tokens[1]);
  }
  else {
    return new Word(tokens[1], tokens[0]);
  }
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

static int TIMEOUT = 5 * 60;

int main(int argc, char** argv) {
  bool reverse = argc > 1 && atoi(argv[1]) != 0;
  std::ifstream file;
  std::string line;
  std::vector<Word*> allWords;
  Word* word;
  bool outputDistraction;
  std::time_t startTime = std::time(nullptr);
  std::time_t currentTime;
  CsvReader csvReader;
  int sleepSeconds;
  
  std::list<std::list<std::string>> rows = csvReader.parse("words.txt");
  
  for (auto row : rows) {
    if (row.size() != 2) {
      throw "Invalid size.";//TODO-improve
    }
    if (reverse) {
      allWords.push_back(new Word(row.back(), row.front()));
    }
    else {
      allWords.push_back(new Word(row.front(), row.back()));
    }
  }
  
  
  file.open("distraction.txt");
  std::string distractionText(
    (std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>()
  );
  
  std::vector<Word*> randomWords;
  
  while (true) {
    currentTime = std::time(nullptr);
    if ((currentTime - startTime) >= TIMEOUT) {
      std::cout << "Timeout" << std::endl;
      exit(0);
    }
    
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
      //TODO-a small 0.1s sleep might be good here to give you time to look back to the top when its about to go back from the distraction
      std::random_shuffle ( randomWords.begin(), randomWords.end() );

      while (randomWords.size() > 0) {
	outputDistraction = false;

	clearScreen();
	
	word = randomWords.back();
	randomWords.pop_back();
	
	std::cout << word->getNativeLanguage() << std::endl;

	if (word->getSecondsToWait() == 1) {
	  sleepSeconds = 3;
	}
	else if (word->getSecondsToWait() == 3) {
	  sleepSeconds = 2;
	}
	else {
	  sleepSeconds = 1;
	}
	
	sleep(sleepSeconds);
	std::cout << word->getTargetLanguage() << std::endl;
	handleWord(word);
	std::cout << std::endl;
      }
    }
  }
}
