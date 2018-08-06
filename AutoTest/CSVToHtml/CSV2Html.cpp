#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void split(const std::string &s, char delimiter, std::vector<std::string> &tokens) {
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    if (token.length() > 1) {
      tokens.push_back(token);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: CSV2Html.exe csvPath htmlPath.\n");
    return -1;
  }

  std::ifstream in(argv[1]);
  std::ofstream out(argv[2]);
  char line[2048];
  in.getline(line, 2048, '\n');

  std::vector<string> header;
  split(line, ',', header);
  std::vector<string> cells;
  while (in.getline(line, 2048, '\n')) {
    split(line, ',', cells);
  }

  int lines = cells.size() / header.size();

  out << "<html>\n";
  out << "<body>\n";
  out << "<h1>Test report</h1>\n";
  out << "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">" << endl;
  out << "<table class=\"imagetable\" border=\"1\">\n";
  out << "<tr>" << endl;
  for (int i = 0; i < header.size(); i++) {
    out << "<th>" << header[i] << "</th>" << endl;
  }
  out << "</tr>" << endl;

  for (int i = 0; i < lines; i++) {
    out << "<tr>" << endl;
    for (int j = 0; j < header.size(); j++) {
      out << "<td>" << cells[i * header.size() + j] << "</td>" << endl;
    }
    out << "</tr>" << endl;
  }
  out << "</table>" << endl;

  //tail
  out << "</body>" << endl;
  out << "</html>" << endl;


  /*
  <html>
  <body>

  <h1>Title</h1>

  <table border="1">
  <tr>
  <th>Heading</th>
  <th>Another Heading</th>
  </tr>
  <tr>
  <td>row 1, cell 1</td>
  <td>row 1, cell 2</td>
  </tr>
  <tr>
  <td>row 2, cell 1</td>
  <td>row 2, cell 2</td>
  </tr>
  </table>

  </body>
  </html>
  */
  //header
  //line

  //tail
  return 0;
}