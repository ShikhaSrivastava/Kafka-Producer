#include <string>

class sendMessage
{
  public:
  	sendMessage();
    void sendMsg(const char *pathName, const char* fileN, std::string topic_name);
    void sendRelayMessage(const char* pathName, std::vector<std::string> allTopics, std::time_t lastIntervalTime, std::multimap<uint32_t, std::string> backlogFiles);

};
