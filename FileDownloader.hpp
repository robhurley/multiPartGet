#include <mutex>
#include <fstream>

/* Download the file in parallel if the option supports it
 */

class SettingsHandler;

class FileDownloader {
public:
  void downloadFile(const SettingsHandler& settings);
private:
  void* outputBuffer;
  void downloadThread(unsigned int chunk, const SettingsHandler& settings);
  
  // Libcurl callback for data - user p holds pointer to buffer to write to
  static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
};