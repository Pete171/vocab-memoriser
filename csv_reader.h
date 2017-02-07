#include <list>
#include <string>

class CsvReader {
private:
  char colSep;
  char lineSep;
public:
  CsvReader(char colSep, char lineSep) : colSep(colSep), lineSep(lineSep) {}
  CsvReader() : colSep(','), lineSep('\n') {}
  std::list< std::list<std::string> >  parse(std::string filePath);
};
