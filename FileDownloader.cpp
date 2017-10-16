#include "FileDownloader.hpp"
#include "SettingsHandler.hpp"
#include <curl/curl.h>
#include <thread>
#include <vector>
#include <math.h>
#include <cstring>
#include <stdio.h>

#include <iostream>


void FileDownloader::downloadFile(const SettingsHandler &settings)
{
  // Currently buffer to memory object, should write directly to disk
  outputBuffer = new char[settings.getTotalSize()];
  
  // Currently code doesnt get the file size before, so if total size>actual file size, it will duplicate
  std::size_t chunkCount = ceil((float)settings.getTotalSize()/(float)settings.getChunkSize());
  // If parallel we make each chunk a thread, including a remainder
  if(settings.getParallel())
  {
    // ToDo implement a real theadpool here so threads can be < size/chunk
    std::vector<std::thread> threadPool;
    for(std::size_t i = 0; i < chunkCount; ++i)
    {
      threadPool.push_back(std::thread(&FileDownloader::downloadThread,this,i,settings));
    }
    for(std::size_t i = 0; i < threadPool.size(); ++i)
    {
      threadPool[i].join();
    }
  }
  else
  {
    for(std::size_t i = 0; i < chunkCount; ++i)
    {
      downloadThread(i,settings);
    }
  }
  // Download is complete
  FILE* fOut = fopen(settings.getFileName().c_str(),"wb");
  fwrite(outputBuffer,1,settings.getTotalSize(),fOut);
  fclose(fOut);
  
}

// Keep a record for each chunk to know its progress in the buffer
struct CurlChunkHeader{
  void* bufferPtr;
  std::size_t bufferIndex = 0;
};

void FileDownloader::downloadThread(unsigned int chunk, const SettingsHandler& settings)
{
  CurlChunkHeader thisChunkHeader;
  auto easyhandle = curl_easy_init();
  std::size_t rangeLow = chunk*settings.getChunkSize();
  std::size_t rangeHigh = std::min(rangeLow+settings.getChunkSize(),settings.getTotalSize());
  std::string curlRangeStr;
  // Curl requires string arguments
  curlRangeStr = std::to_string(rangeLow) + "-" + std::to_string(rangeHigh);
  curl_easy_setopt(easyhandle, CURLOPT_RANGE, curlRangeStr.c_str());
  curl_easy_setopt(easyhandle, CURLOPT_URL, settings.getURL().c_str());
  curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, FileDownloader::write_data);
  thisChunkHeader.bufferIndex = rangeLow;
  thisChunkHeader.bufferPtr = outputBuffer;
  curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &thisChunkHeader);
  curl_easy_perform(easyhandle);
}

size_t FileDownloader::write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
  CurlChunkHeader* header = (CurlChunkHeader*)userp;
  memcpy(header->bufferPtr+header->bufferIndex,buffer,nmemb);
  header->bufferIndex+=nmemb;
  return nmemb;
}

