/* Reads all command line arguemnts and makes it available to
 * user
 */
#include <stdint.h>
#include <string>

class SettingsHandler{
public:
  /* Constuctor throws if the arguments doesnt include required */
  SettingsHandler(int argc, char **argv);
  
  static void printExampleCommand(std::string fileName);
  
  std::string getURL() const{
    return m_url;
  }
  std::string getFileName() const {
    return m_outputFileName;
  }
  std::size_t getTotalSize() const {
    return m_totalSize;
  }
  std::size_t getChunkSize() const {
    return m_chunkSize;
  }
  bool getParallel() const {
    return m_downloadInParralel;
  }
private:
  std::string m_url;
  std::string m_outputFileName;
  bool m_downloadInParralel;
  uint64_t m_chunkSize;
  uint64_t m_totalSize;
  
  // Returns an empty string if the argument doesnt exist
  std::string getArgumentValue(const std::string &argumentToFind, int argc, char **argv);
};