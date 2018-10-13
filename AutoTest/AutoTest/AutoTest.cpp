#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <fstream>
#include <stdio.h>
using namespace std;

//#include <boost/filesystem.hpp>
//using namespace boost::filesystem;

#include <windows.h>

namespace report {
struct LevelMsg {
  string message;
  int level;
  enum { kFailure = 0, kError = 1 };

  LevelMsg() {}
  LevelMsg(const string &msg, int _level) : message(msg), level(_level) {}
  void print(int indent, FILE *fp) {
    const char *types[] = { "failure", "error" };
    fprintf(fp, "%*s", indent, "");
    fprintf(fp, "<%s message=\"%s\" type=\"%s\"/>\n", types[level], message.c_str(), types[level]);
  }
  static void test() {
    LevelMsg msg1;
    msg1.message = "1111";
    msg1.level = 0;
    msg1.print(2, stdout);

    LevelMsg msg2;
    msg2.message = "222";
    msg2.level = 1;
    msg2.print(2, stdout);
  }
};

struct SystemInfo {
  string message = "N/A";
  int level;
  enum { kSystemOut = 0, kSystemError = 1 };

  SystemInfo(int _level = 0) : level(_level) {}
  SystemInfo(const string &_msg, int _level) : level(_level), message(_msg) {}

  void print(int indent, FILE *fp) {
    const char *types[] = { "system-out", "system-err" };
    fprintf(fp, "%*s", indent, "");
    fprintf(fp, "<%s>%s</%s>\n", types[level], message.c_str(), types[level]);
  }
  static void test() {
    SystemInfo msg1;
    msg1.message = "1111";
    msg1.level = 0;
    msg1.print(4, stdout);

    SystemInfo msg2;
    msg2.message = "222";
    msg2.level = 1;
    msg2.print(4, stdout);
  }
};
struct TestCase {
  string classname = "TestCase";
  string name = "TestCase";
  string result;
  vector<LevelMsg> msgs;
  vector<SystemInfo> sys_infos;

  TestCase() {}
  TestCase(const string &name) : classname(name) {}
  void print(int indent, FILE *fp) {
    fprintf(fp, "%*s<testcase classname=\"%s\" name=\"%s\">\n", indent, "", classname.c_str(), name.c_str());
    if (result == "SKIPPED") {
      fprintf(fp, "%*s<skipped/>\n", indent + 4, "");
    }

    for (auto item : msgs) {
      item.print(indent + 4, fp);
    }
    for (auto item : sys_infos) {
      item.print(indent + 4, fp);
    }
    fprintf(fp, "%*s</testcase>\n", indent, "");
  }

  void update() {
    msgs.clear();
    if (result == "FAILED" || result == "UNKNOWN") {
      msgs.push_back(LevelMsg("failure", 0));
    }
  }
  static void test() {
    TestCase t;
    t.classname = "TestCapture";
    t.name = "TestCapture";
    t.sys_infos = { SystemInfo(0), SystemInfo(1) };
    t.print(0, stdout);
  }
};
struct TestSuit {
  int disabled = 0;
  int errors = 0;
  int failures = 0;
  int skipped = 0;
  int tests = 0;
  double time = 0;
  string name = "TestSuit";
  vector<TestCase> testcases;

  TestSuit() {}

  void update() {
    tests = testcases.size();
    for (auto i = 0; i < testcases.size(); i++) {
      std::string result = testcases[i].result;
      testcases[i].update();
      if (result == "PASSED") {}
      else if (result == "SKIPPED") {
        skipped++;
      } else if (result == "FAILED") {
        errors++;
      } else if (result == "UNKNOWN") {
        errors++;
      }
    }
  }
  void print(int indent, FILE *fp) {
    fprintf(fp, "%*s<testsuite disabled=\"%d\" errors=\"%d\" failures=\"%d\" name=\"%s\" skipped=\"%d\" tests=\"%d\" time=\"%f\">\n",
            indent, "", disabled, errors, failures, name.c_str(), skipped, tests, time);
    for (auto &item : testcases) {
      item.print(indent + 4, fp);
    }
    fprintf(fp, "%*s</testsuite>\n", indent, "");
  }

  static void test() {
    TestSuit suit;
    suit.testcases.push_back(TestCase());
    suit.testcases.push_back(TestCase("TestCase2"));
    suit.print(0, stdout);
  };
};

struct TestSuits {
  int disabled = 0;
  int errors = 0;
  int failures = 0;
  int tests = 0;
  double time = 0;

  vector<TestSuit> suits;

  void update() {
    for (auto i = 0; i < suits.size(); i++) {
      suits[i].update();
      disabled += suits[i].disabled;
      errors += suits[i].errors;
      failures + suits[i].errors;
      tests += suits[i].tests;
      time += suits[i].time;
    }
  }
  void print(int indent, FILE *fp) {
    fprintf(fp, "<?xml version=\"1.0\"?>\n");
    fprintf(fp, "%*s<testsuites disabled=\"%d\" errors=\"%d\" failures=\"%d\" tests=\"%d\" time=\"%d\">\n",
            indent, "", disabled, errors, failures, tests, time);
    for (auto &item : suits) {
      item.print(indent + 4, fp);
    }
    fprintf(fp, "%*s</testsuites>\n", indent, "");
  }

  static void test() {
    printf("\n\n%s\n", __FUNCTION__);
    TestSuits t;
    t.suits.push_back(TestSuit());
    t.suits[0].testcases.push_back(TestCase());
    t.suits[0].testcases[0].classname = "ClassName";
    t.suits[0].testcases[0].msgs.push_back(LevelMsg("Failed", LevelMsg::kFailure));
    t.suits[0].testcases[0].sys_infos.push_back(SystemInfo("Failed", SystemInfo::kSystemOut));
    t.print(8, stdout);
  }
};

void test() {
  LevelMsg::test();
  SystemInfo::test();
  TestCase::test();
  TestSuit::test();
  TestSuits::test();
}
}

vector<string> get_all_files_names_within_folder(string folder, const string &prefix = "") {
  vector<string> names;
  string search_path = folder + "/*.*";
  WIN32_FIND_DATAA fd;
  HANDLE hFind = ::FindFirstFileA(search_path.c_str(), &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      // read all (real) files in current folder
      // , delete '!' read other 2 default folder . and ..
      if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        //string name = fd.cFileName;
        //if (name.find(Filter) == 0) {
        //  names.push_back(name);
        //}
        if (strstr(fd.cFileName, prefix.c_str())) {
          names.push_back(fd.cFileName);
        }
      }
    } while (::FindNextFileA(hFind, &fd));
    ::FindClose(hFind);
  }
  return names;
}

string ReadResult(const string &filename) {
  std::ifstream in(filename);
  string result;
  in >> result;
  return result;
}
struct TestItem {
  string name;
  string result;
  TestItem(const string &_name, const string &_result) : name(_name), result(_result) {}
};
vector<TestItem>  ParseResult(const string &folder_path, string prefix = "Test") {
  //string folder_path = "D:\\git_clone\\D3DVideo_2\\D3DVideo\\Console";
  //string prefix = "Test";

  vector<TestItem> test_results;
  vector<string> Folders = get_all_files_names_within_folder(folder_path, prefix);
  for (size_t i = 0; i < Folders.size(); i++) {
    string result = ReadResult(folder_path + "\\" + Folders[i] + "\\status.txt");
    test_results.push_back(TestItem(Folders[i], result));
  }

  return test_results;
}
void printResult(vector<TestItem> &results) {
  cout << "Test Results:\n";
  for (size_t i = 0; i < results.size(); i++) {
    cout << i << "," << results[i].name << "," << results[i].result << endl;
  }
}
using namespace report;
void SaveResultToXmlFile(const vector<TestItem> &results, const std::string &filename) {
  TestSuits suits;
  TestSuit suit;
  for (size_t i = 0; i < results.size(); i++) {
    TestCase testcase;
    const TestItem &item = results[i];
    testcase.classname = item.name;
    testcase.name = item.name;
    testcase.result = item.result;


    suit.testcases.push_back(testcase);
  }
  suits.suits.push_back(suit);

  suits.update();
  suits.print(0, stdout);

  FILE *fp = fopen(filename.c_str(), "w");
  if (fp) {
    suits.print(0, fp);
    fclose(fp);
  }
}

void testParseResult() {
  string folder = "D:\\git_clone\\D3DVideo_2\\D3DVideo\\Console";
  auto results = ParseResult(folder);
  printResult(results);
  SaveResultToXmlFile(results, folder + "\\report.xml");
}

int main(int argc, char *argv[]) {
  //report::test();
  //testParseResult();

  if (argc < 2) {
    printf("Usage: GenTestReport.exe folder [reportPath]\n");
    printf("	Build time:%s,%s\n", __DATE__, __TIME__);
    printf("	sample: GenTestReport.exe D:\\git_clone\\D3DVideo_2\\D3DVideo\\Console d:\\report.xml");
    printf("	folder: Console\Test* folder");
    printf("	reportPath: default is folder\\report.xml");
    return -1;
  }

  string folder = argv[1];
  string xmlFileName = argc > 2 ? argv[2] : folder + "\\report.xml";

  argv[2];
  string filter = "Test";
  auto results = ParseResult(folder, filter);
  printResult(results);
  SaveResultToXmlFile(results, xmlFileName);

  return 0;
}