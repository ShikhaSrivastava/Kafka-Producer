#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <errno.h>
#include <libkafka/ApiConstants.h>
#include <libkafka/Client.h>
#include <libkafka/Message.h>
#include <libkafka/MessageSet.h>
#include <libkafka/TopicNameBlock.h>
#include <libkafka/produce/ProduceMessageSet.h>
#include <libkafka/produce/ProduceRequest.h>
#include <libkafka/produce/ProduceResponsePartition.h>
#include <libkafka/produce/ProduceResponse.h>
#include <libkafka/metadata/MetadataRequest.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fcntl.h>  
#include <fstream>
#include <unistd.h> 
#include <assert.h>
#include <snappy.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <utility>
#include <sys/wait.h>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>   /* for sockaddr_in */
#include <time.h>
#include <algorithm>
#include <iterator>
#include <dirent.h>
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "sendMessage.h"


#define DEFAULT_BUFFER_SIZE 1024 * 64


using namespace google::protobuf::io;
using namespace std;
using namespace LibKafka;
using namespace boost;

boost::mutex m;
static std::vector<std::string> startedFiles;
static std::vector<std::string> allTopics;

std::time_t latestTime = 0;
std::time_t oldestTm = 0;
unsigned long int lastIntervalTime = std::time(0);
std::multimap<uint32_t, std::string> backlogFiles;

std::string getMachineName() {
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);

    addrinfo hints;
    addrinfo * info;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;

    int res = getaddrinfo(hostname, NULL, &hints, &info);
    if (res != 0) {
        throw std::runtime_error("Cannot determine own hostname!!");
    }

    const std::string hostName = info->ai_canonname;
    freeaddrinfo(info);

    return hostName;
}

const char* nextToSend(std::multimap<std::time_t , std::string> map1){
    std::multimap<std::time_t , std::string>::reverse_iterator endItr;
    std::multimap<std::time_t , std::string>::iterator beginItr;
    std:: string tempFileName;
    std::multimap<std::time_t , std::string> sortedFiles = map1;
   // boost::mutex::scoped_lock l;

    if(!sortedFiles.empty()){
        beginItr = sortedFiles.begin();
        endItr = sortedFiles.rbegin();
        if(latestTime <= (*endItr).first){
            boost::mutex::scoped_lock l1(m);
            latestTime = (*endItr).first;
            tempFileName = (*endItr).second;
            sortedFiles.erase(latestTime);
            return tempFileName.c_str();
        }else{
            boost::mutex::scoped_lock l2(m);
            oldestTm = (*beginItr).first;
            tempFileName = (*beginItr).second;
            sortedFiles.erase(oldestTm);
            return tempFileName.c_str();
        }
    }
}

std::string getTopicName(std::string fileN){
    std::string machineName = getMachineName();
    std::vector<string> token1;
    std::vector<string> token;
    boost::split( token1, machineName, boost::is_any_of( "-." ));
    boost::split( token, fileN, boost::is_any_of( "." ));
    std::ostringstream forTopicName;
    forTopicName << token1[2] << "-" << machineName << "-" << token[0];
    std::string topic_name(forTopicName.str());
    if(!(std::find(allTopics.begin(), allTopics.end(), topic_name) != allTopics.end())){
        allTopics.push_back(topic_name);
    }
    return topic_name;
}

int main(int argc, char *argv[])
{
    
    sendMessage o;
    vector<string> fileToken;
    
    std::multimap<std::time_t , std::string> map1;
    std::multimap<std::time_t , std::string> map2;
    std::multimap<std::time_t , std::string> map3;
    std::multimap<std::time_t , std::string> map4;
    std::time_t latestTime = 0;
    std::time_t oldestTm = 0;
    std::string tempFileName;
    int numOfFiles = 0;
    int found = 0;
    
    if ( argc != 2 ) // argc should be 2 for correct execution
        cout<<"usage: "<< argv[0] <<" <Directory Path>\n";
    else {
        cout<< "reset" << endl;
        const char* pathName = argv[1];
        string   str;
        DIR *pDIR;
        struct dirent *entry;
        int fileNameSize = 0;
        pid_t pid;
        pid_t retval;
        while(1){
            if( pDIR=opendir(pathName) ){
                map1.clear();
                map2.clear();
                map3.clear();
                map4.clear();
                backlogFiles.clear();
                numOfFiles = 0;
                while(entry = readdir(pDIR)){
                    if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
                        std::string tempFile(entry->d_name);
                        boost::split( fileToken, tempFile, boost::is_any_of( "." ));
                        fileNameSize = fileToken.size();
                        if(fileNameSize == 8){
                            stringstream sw;
                            sw << fileToken[7];
                            if(strcmp(((sw.str()).c_str()), "snappy") == 0){
                                found = 0;
                                stringstream filePath;
                                filePath << pathName << "/" << tempFile;
                                boost::filesystem::path p( filePath.str() ) ;
                                if ( boost::filesystem::exists( p ) ) {
                                    uint32_t timestamp = atoi(fileToken[6].c_str());
                                    boost::mutex::scoped_lock(m);
                                    backlogFiles.insert(make_pair(timestamp, tempFile));
                                    if(!(std::find(startedFiles.begin(), startedFiles.end(), tempFile) != startedFiles.end())){
                                        std::time_t t = boost::filesystem::last_write_time( p );
                                        ++numOfFiles;
                                        switch(numOfFiles%4){
                                            case 1:
                                            map1.insert(make_pair(t, tempFile));
                                            break;
                                            case 2:
                                            map2.insert(make_pair(t, tempFile));
                                            break;
                                            case 3:
                                            map3.insert(make_pair(t, tempFile));
                                            break;
                                            case 0:
                                            map4.insert(make_pair(t, tempFile));
                                            break;
                                            default:
                                            cout << "Invalid option" << endl;
                                            break;
                                        }
                                    }
                                }else {
                                    std::cout << "Could not find file " << filePath.str() << '\n' ;
                                    continue;
                                }
                            }
                        }
                    }
                } 
                cout<<"Im parent :" << getpid() << endl;

                if(!map1.empty()){
                    const char* f1 = nextToSend(map1);
                    std::string fileName1(f1);
                    startedFiles.push_back(fileName1);
                    boost::mutex::scoped_lock l1(m);
                    std::string topic1 = getTopicName(fileName1);
                    boost::this_thread::sleep_for( boost::chrono::seconds(2) );
                    pid = fork();
                    if(pid == 0){
                        cout<< "Im process : " << getpid() << endl;
                        o.sendMsg(pathName, f1, topic1);
                    }    
                }
                if(!map2.empty()){
                    
                    const char* f2 = nextToSend(map2);
                    std::string fileName2(f2);
                    startedFiles.push_back(fileName2);
                    boost::mutex::scoped_lock l1(m);
                    std::string topic2 = getTopicName(fileName2);
                    boost::this_thread::sleep_for( boost::chrono::seconds(2) );
                    
                    pid = fork();
                    if(pid == 0){
                        cout<< "Im process : " << getpid() << endl;
                        o.sendMsg(pathName, f2, topic2);
                    }
                }
                if(!map3.empty()){
                    
                    const char* f3 = nextToSend(map3);
                    std::string fileName3(f3);
                    startedFiles.push_back(fileName3);
                    boost::mutex::scoped_lock l1(m);
                    std::string topic3 = getTopicName(fileName3);
                    boost::this_thread::sleep_for( boost::chrono::seconds(2) );
                    
                    pid = fork();
                    if(pid == 0){
                        cout<< "Im process : " << getpid() << endl;
                        o.sendMsg(pathName, f3, topic3);
                    }  
                }
                if(!map4.empty()){
                    
                    const char* f4 = nextToSend(map4);
                    std::string fileName4(f4);
                    startedFiles.push_back(fileName4);
                    boost::mutex::scoped_lock l1(m);
                    std::string topic4 = getTopicName(fileName4);
                    boost::this_thread::sleep_for( boost::chrono::seconds(2) );
                    pid = fork();
                    if(pid == 0){
                        cout<< "Im process : " << getpid() << endl;
                        o.sendMsg(pathName, f4, topic4);
                    }
                }
               /* pid = fork();
                if(pid == 0){
                    cout<< "Im process : " << getpid() << endl;
                    o.sendRelayMessage(pathName, allTopics, lastIntervalTime, backlogFiles);  
                }*/
                retval = waitpid(-1, NULL, 0);              
            }
            boost::this_thread::sleep_for( boost::chrono::seconds(5) );
            closedir(pDIR);
        }
    }
    return 1;
}
