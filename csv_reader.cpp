#include <string>
#include <sstream>
#include <fstream>
#include <list>

#include "csv_reader.h"

std::list< std::list<std::string> > CsvReader::parse(std::string filePath) {
  std::ifstream file;
  std::string line;
  std::list< std::list<std::string> > rows;
  
  file.open(filePath);
  
  while(getline(file, line, lineSep)) {
    std::stringstream ss;
    std::list<std::string> cols;
    std::string col;
    
    ss.str(line);
    
    while (getline(ss, col, colSep)) {
      cols.push_back(col);
    }
      
    rows.push_back(cols);
  }
  
  file.close();
  
  return rows;
}
