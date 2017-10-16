#include "SettingsHandler.hpp"

#include <iostream>
#include <string>

#define FILEOUTPUTNAME "outputFile"
#define CHUNK_COUNT 4
// 4 MiB
#define TOTAL_SIZE 4194304

SettingsHandler::SettingsHandler(int argc, char **argv)
{
  // Check for valid options
  if(argc == 1)
    throw "Invalid Arguments";
  // output file specified?
  m_outputFileName = getArgumentValue("-o",argc,argv);
  if(m_outputFileName.empty())
    m_outputFileName = FILEOUTPUTNAME;
  m_url = argv[argc-1];
  m_totalSize = TOTAL_SIZE;
  std::string sizeString = getArgumentValue("-t",argc,argv);
  if(!sizeString.empty())
    m_totalSize = std::stoi(sizeString);
  m_chunkSize = m_totalSize/4;
  sizeString = getArgumentValue("-c",argc,argv);
  if(!sizeString.empty())
    m_chunkSize = std::stoi(sizeString);
  m_downloadInParralel = !getArgumentValue("-p",argc,argv).empty();
  
  // Should be validating that total size can be downloaded...
  std::cout << "Downloading: "<<m_url << " to: "<<m_outputFileName<<"\n";
}

std::string SettingsHandler::getArgumentValue(const std::string& argumentToFind, int argc, char** argv)
{
  for(uint i = 1; i < argc; ++i)
  {
    if(argumentToFind == argv[i] && i+1 < argc)
    {
      return argv[i+1];
    }
  }
  return std::string();
}


void SettingsHandler::printExampleCommand(std::string fileName)
{
  std::cout << "Usage: ./" << fileName << "[Options] URL\n";
  std::cout << "-o outfile file name\n";
  std::cout << "-p download in parallel\n";
  std::cout << "-c chunk size\n";
  std::cout << "-t total size\n";
}
