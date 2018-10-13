// ZipExample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zipper.h"
#include "unzipper.h"
#include <fstream>
#include <omp.h>

using namespace std;
using namespace ziputils;

void zip() {
  const char *Filenames[] = {"unzipper.cpp", "unzipper.h", "zipper.cpp", "zipper.h"};
  unsigned int nCount = sizeof( Filenames ) / sizeof( char * );

  zipper zipFile;
  zipFile.open("test.zip");

  // add files to the zip file
  for ( unsigned int i = 0; i < nCount; i++ ) {
    ifstream file( Filenames[i], ios::in | ios::binary );
    if ( file.is_open() ) {
      zipFile.addEntry( Filenames[i] );
      zipFile << file;
    }
  }
  zipFile.close();
}

void zipFolder() {
  // append the existing file
  zipper zipFile;
  zipFile.open("test.zip", true);

  // add file into a folder
  ifstream file( "unzipper.cpp", ios::in | ios::binary );
  if ( file.is_open() ) {
    zipFile.addEntry( "/Folder/unzipper.cpp" );
    zipFile << file;
  }
  zipFile.close();
}

void unzip() {
  unzipper zipFile;
  zipFile.open("test.zip");
  auto filenames = zipFile.getFilenames();

  for ( auto it = filenames.begin(); it != filenames.end(); it++ ) {
    zipFile.openEntry( (*it).c_str() );
    zipFile >> std::cout;
  }
}
void myZip() {
  const char *Filenames[] = { "D3DCamera_20180516.txt" };
  unsigned int nCount = sizeof(Filenames) / sizeof(char *);

  zipper zipFile;
  zipFile.open("test.zip");

  // add files to the zip file
  for (unsigned int i = 0; i < nCount; i++) {
    ifstream file(Filenames[i], ios::in | ios::binary);
    if (file.is_open()) {
      zipFile.addEntry(Filenames[i]);
      zipFile << file;
    }
  }
  zipFile.close();
}
void myUnzip() {
  double t0 = omp_get_wtime();
  unzipper zipFile;
  zipFile.open("D:\\ziptest\\0524.zip");
  auto filenames = zipFile.getFilenames();

  for (auto it = filenames.begin(); it != filenames.end(); it++) {
    zipFile.openEntry((*it).c_str());
    ofstream file(*it, ios::binary);
    zipFile >> file;

    //zipFile >> std::cout;
  }
  double t1 = omp_get_wtime();
  printf("%s used time %f s", t1 - t0);
}
int main(int argc, char *argv[]) {
  cout << "sizeof(void*)=" << sizeof(void *) << endl;
  //myZip();
  //myUnzip();
  //return 0;
  zip();
  zipFolder();
  unzip();
  return 0;
}

