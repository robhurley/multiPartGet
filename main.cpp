#include <iostream>

#include "SettingsHandler.hpp"
#include "FileDownloader.hpp"


int main(int argc, char **argv) {
  try{
    SettingsHandler settings(argc,argv);
    FileDownloader temp;
    temp.downloadFile(settings);
    std::cout << "Download complete\n";
  }
  catch(...)
  {
    SettingsHandler::printExampleCommand(argv[0]);
  }
  return 0;
}
