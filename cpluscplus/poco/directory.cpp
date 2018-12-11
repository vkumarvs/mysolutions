#include <Poco/DirectoryIterator.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>
#include <iostream>
#include <string>
#include "Poco/Exception.h"
#include <stdio.h>
using namespace Poco;
using namespace std;
using Poco::SyntaxException;
void rec_dir(const string & path)
{
      DirectoryIterator end;
      DirectoryIterator it(path);
#if 0
    try 
    {
      DirectoryIterator it(path);
      printf("MSD-INTERNAL content dir not present (%d)--(Zu)--(%s) \n",
             it->isDirectory(), it->getSize(), path.c_str());
    }
    catch (Poco::Exception& exc)
    {
      cout << "No directory" <<endl;
    }
#else
  for (; it != end; ++it)
  {
    cout << (it->isDirectory() ? "true" : "false") <<endl; 
    cout << it->getSize() << endl;

#if 0
    LocalDateTime lastModified(it->getLastModified());
    cout << DateTimeFormatter::format(lastModified, "%Y-%m-%d %H:%M") << "\t";

    cout << it->path() << (it->isDirectory() ? "/" : it->canExecute() ? "*" : "") << endl;

    if (it->isDirectory())
    {
      rec_dir(it->path());
    }
#endif
  }
#endif
}

int main(int argc, char **argv)
{
  rec_dir(argc > 1 ? argv[1] : ".");
  return 0;
}
