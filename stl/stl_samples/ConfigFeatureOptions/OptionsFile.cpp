#include "stdafx.h"
#include "OptionsFile.h"
#include <fstream>

OptionsFile::OptionsFile() {
}


OptionsFile::OptionsFile(const std::string &path) {
  m_optionFile = path;
  Read();
}

OptionsFile::~OptionsFile() {
}

void OptionsFile::Read() {
  std::ifstream in(m_optionFile);
  if (!in) {
    Init();
    return;
  }
  //read from file
  while (true) {
    std::string name;
    int value;
    in >> name >> value;
    if (in) {
      m_options[name] = value;
    } else break;
  }
}

void OptionsFile::Write() {
  std::ofstream os(m_optionFile);
  for (auto &item : m_options) {
    os << item.first << " " << item.second << std::endl;
  }
}

void OptionsFile::Init() {
  //set default values
  m_options["RemoveSoftTissue"] = 1;
}

void OptionsFile::SaveOption(const std::string &name, int value) {
  m_options[name] = value;
  Write();
}
