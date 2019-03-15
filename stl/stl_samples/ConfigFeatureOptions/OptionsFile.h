#pragma once

#include <map>
#include <string>

class OptionsFile {
 public:
  OptionsFile();
  OptionsFile(const std::string &path);
  ~OptionsFile();
  void Read();
  void Write();
  void Init();
  void SaveOption(const std::string &name, int value);

  std::string m_optionFile;
  std::map<std::string, int> m_options;
};

