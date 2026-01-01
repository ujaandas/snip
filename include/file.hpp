#include <fstream>
#include <ios>
#include <string>

/*
Currently open file buffer - handles all IO operations thereof.
*/

#ifndef FILE_H
#define FILE_H

class File {
  std::string line;
  std::fstream stream;

public:
  File(std::string fp) {
    stream.open(fp, std::ios::in | std::ios::app);
    while (stream.is_open() && !stream.eof()) {
      std::getline(stream, line);
    };
    stream.close();
  };

  std::string getText() { return line; }
};

#endif // FILE_H