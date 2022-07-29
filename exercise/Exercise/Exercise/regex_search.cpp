// regex_search example
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
using namespace std;
void sample() {
  std::string s("this subject has a submarine as a subsequence");
  std::smatch m;
  std::regex e("\\b(sub)([^ ]*)");   // matches words beginning by "sub"

  std::cout << "Target sequence: " << s << std::endl;
  std::cout << "Regular expression: /\\b(sub)([^ ]*)/" << std::endl;
  std::cout << "The following matches and submatches were found:" << std::endl;

  while (std::regex_search(s, m, e)) {
    for (auto x : m) std::cout << x << " ";
    std::cout << std::endl;
    s = m.suffix().str();
  }
}

void find_name1() {
  std::regex reg("<(.*)>(.*)</(\\1)>");
  std::cmatch m;
  auto ret = std::regex_search("123<xml>value</xml>456", m, reg);
  if (ret) {
    for (auto &elem : m)
      std::cout << elem << std::endl;
  }

  std::cout << "prefix:" << m.prefix() << std::endl;
  std::cout << "suffix:" << m.suffix() << std::endl;
}

string find_name() {
  std::regex reg("<name key=\"patient\" (.*) />");
  std::cmatch m;
  auto ret = std::regex_search(R"(<name key="patient" family="JENK" given="0002" middle="" prefix="" suffix="" />)", m, reg);
  //if (ret) {
  //  for (auto &elem : m)
  //    std::cout << elem << std::endl;
  //}
  cout << "ret=" << ret << endl;
  cout << m.size() << endl;
  cout << "[" << m[1] << "]" << endl;
  return m[1];
}

string split() {
  string s = R"(family="JENK" given="0002" middle="" prefix="" suffix="")";
  std::regex reg("family=\"(.*?)\"");//shortest matching
  std::cmatch m;
  auto ret = std::regex_search(s.c_str(), m, reg);
  cout << "ret=" << ret << endl;
  cout << m.size() << endl;
  for (int i = 1; i < m.size(); i++) {
    cout << "[" << m[i] << "]" << endl;

  }

  std::regex reg2("given=\"(.*?)\"");//shortest matching
  std::regex_search(s.c_str(), m, reg2);
  cout << m.size() << endl;
  for (int i = 1; i < m.size(); i++) {
    cout << "[" << m[i] << "]" << endl;

  }

  return m[1];
}


struct ParseXML {
  static std::string ReadFileContent(const std::string &filename) {
    std::ifstream file_config(filename);
    std::string str((std::istreambuf_iterator<char>(file_config)), std::istreambuf_iterator<char>());
    return str;
  }

  static string GetElement(const string &s) {
    std::regex reg("<name key=\"patient\" (.*) />");
    std::cmatch m;
    auto ret = std::regex_search(s.c_str(), m, reg);
    if (ret && m.size() > 1) {
      return m[1];
    }
    return "";
  }

  static string GetAttribute(const string &s, const string &tag) {
    std::regex reg(tag + "=\"(.*?)\""); //shortest matching
    std::cmatch m;
    auto ret = std::regex_search(s.c_str(), m, reg);
    if (ret && m.size() > 1) {
      return m[1];
    }
    return "";
  }

  // FirstName_LastName
  static string GetPatientName(const string &s) {
    return  GetAttribute(s, "given") + "_" + GetAttribute(s, "family");
  }

  static string GetPatientNameFromFile(const string &filename) {
    string file_content = ReadFileContent(filename);
    string line = GetElement(file_content);
    cout << line << endl;
    string name = GetPatientName(line);
    return name;
  }

  static void test() {
    string s = R"(family="JENK" given="0002" middle="mm" prefix="" suffix="")";
    cout << GetPatientName(s) << endl;
    cout << GetPatientNameFromFile("C:/ProgramData/TW/AcqAltair/inputdata.xml") << endl;
  }
};

int main() {
  ParseXML::test();
  return 0;
}