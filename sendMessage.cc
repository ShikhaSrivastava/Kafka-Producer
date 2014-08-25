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
#include <sys/types.h>
#include <unistd.h>
#include <iterator>
#include <dirent.h>
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "proto/EventJournaller.pb.h"
#include "proto/event_bus.pb.h"
#include "sendMessage.h"


#define DEFAULT_BUFFER_SIZE 1024 * 64

using namespace google::protobuf::io;
using namespace std;
using namespace LibKafka;
using namespace fim::event::journaller;
using namespace fim::eventbus;
using namespace boost;

Message* createMessage(const char * value, uint32_t recLen, const char *key = "");
ProduceRequest* createProduceRequest(string topic_name, Message **messageArray, int messageArraySize);
MetadataRequest* createMetadataRequest(const char* topic_name);

const static int topicNameArraySize = 1;
uint32_t oldestTime = 0;
unsigned long int curTime;
boost::mutex mtx;

sendMessage::sendMessage(){
	cout<<"this is the dummy constructor"<<endl;
}

int analyze_print(const char *data, const uint32_t len){
	RecordedEvent recordEvent;
	static int count = 0;
	vector<string> token;
	std::ostringstream s;
	if( !recordEvent.ParseFromArray(data, len ) ) {
		return 0;
	}

	EventBusMessage ebMsg = recordEvent.event();
	if( !ebMsg.is_valid() ) {
		return 0;
	}
	return 1;
}



void sendMessage::sendMsg(const char *pathName, const char* fileN, std::string topic_name){
	const char *hostname = "localhost";
	const int port = 9092;
	const char *message_string = "This is a test message.";
	std::string fileName(fileN);
	std::ostringstream s;
	pid_t pid = getpid();
	Client *c = new Client(hostname, port);
	MetadataResponse *mresponse;
	int i = 0;
	stringstream fileSw;
	fileSw << pathName << "/" << fileName;
	std::string tmp(fileSw.str().c_str());
	const char *filename = tmp.c_str();
	int fd = open(filename, O_RDONLY);
	cout << "------------------------------------------------" << endl;
	cout << "topic name : " << topic_name << endl;
	cout << "-------------------------------------------------" << endl;

	do{
		MetadataRequest *mrequest = createMetadataRequest(topic_name.c_str());
		mresponse = c->sendMetadataRequest(mrequest);
	}while(mresponse->hasErrorCode());

	ZeroCopyInputStream * rawFileInput = new FileInputStream(fd);
	CodedInputStream * codedFileInput = new CodedInputStream(rawFileInput);

	uint64_t fileUncompressedSize = 0;
	struct stat fileStat;
	assert(0 == fstat(fd, &fileStat));
	codedFileInput->SetTotalBytesLimit(fileStat.st_size, -1);


	for(uint bytesRead = 0; bytesRead < fileStat.st_size;){
		uint32_t uncompressedDataLen = 0, compressedDataLen = 0;
		codedFileInput->ReadVarint32(&compressedDataLen);
		char *compressedData = new char[compressedDataLen];
		codedFileInput->ReadRaw((compressedData), compressedDataLen);
		string temp;
		temp.resize(1024);
		assert(snappy::Uncompress(compressedData,compressedDataLen,&temp));
		ZeroCopyInputStream * rawStringInput = new ArrayInputStream(temp.data(),temp.size());
		CodedInputStream * codedStringInput = new CodedInputStream(rawStringInput);
		bytesRead += CodedOutputStream::VarintSize32(compressedDataLen) + compressedDataLen;

		uint32_t uncompressedBytesRead = 0;
		uncompressedDataLen = temp.size();
		fileUncompressedSize += uncompressedDataLen;

		while(uncompressedBytesRead < uncompressedDataLen){
			uint32_t recLen = 0;
			codedStringInput->ReadVarint32(&recLen);
			char *data = new char[recLen]();
			codedStringInput->ReadRaw((void*) data, recLen);
			int retVal = analyze_print(data, recLen);
			if(retVal == 1){
				uncompressedBytesRead += CodedOutputStream::VarintSize32(recLen) + recLen;
			Message *m = createMessage(data, recLen);
			Message **messageArray = &m;

			ProduceRequest *request = createProduceRequest(string(topic_name), messageArray, 1);
			ProduceResponse *response;
			if( request != NULL ){
				response = c->sendProduceRequest(request); 
			} 

			if (response == NULL)
			{
				cerr << "an error ocurred while sending the produce request, errno = " << strerror(errno) << "\n";
			}
			else
			{
				if (response->hasErrorCode())
					cerr << "publish error detected here 1\n";
				else
					cout << "pid : " << pid << " : " << i << " : message successfully published to kafka\n";
				i++;
			}
			if (response != NULL){
				delete response;
			} 
			if (request != NULL) {
				delete request;
			}
			delete [] data;

		}else{
			cout<<"not a proto buf msg, not published." << endl;
		}
	}
	delete [] compressedData;
	}	
	int fd1 = open(filename, O_RDONLY);
	if(fd1 >= 0){
		std::stringstream fss;
		fss << fileSw.str().c_str() << ".read" ;
		std::string str=fss.str();
		std::string fp(fileSw.str().c_str());
		boost::filesystem::path pOld(fp);
		boost::filesystem::path pNew(str);
		rename(pOld.string().c_str(), pNew.string().c_str());
		cout << "-------------------------------------------------" << endl;
		cout << "filename changed to : " << str << endl;
		cout << "-------------------------------------------------" << endl;
	}else{
		cout << "File must have been moved or deleted. Exiting Safely. " << endl;
	}
	
_exit(1);
}

void sendMessage::sendRelayMessage(const char* pathName, std::vector<std::string> topics, std::time_t lastIntervalTime, std::multimap<uint32_t, std::string> logFiles){
	cout << "I reached here" << endl;
	const char *hostname = "localhost";
	const int port = 9092;
	Client *c = new Client(hostname, port);
	std::multimap<uint32_t , std::string>::iterator beginItr;
	std::multimap<uint32_t, std::string> backlogFiles = logFiles;
	std::vector<std::string> allTopics = topics;
	vector<string> fileToken;
	int fileNameSize = 0;
	mtx.lock();
	if(!backlogFiles.empty()){
		beginItr = backlogFiles.begin();
		oldestTime = (*beginItr).first;
	}
	mtx.unlock();
	curTime = std::time(0);
	cout << "Im checking this condition " << endl;
	if((lastIntervalTime + 900) <= curTime) /*&& ((lastIntervalTime + 90) < oldestTime))*/{

	cout << "condition is true " << endl;
	stringstream s;
	s << "All messages in the time interval  " << lastIntervalTime << " to " << (lastIntervalTime+900) << " has been sent. " ; 

	std::string msg(s.str());
	mtx.lock();
	for(std::vector<std::string>::const_iterator i = allTopics.begin(); i != allTopics.end(); ++i) {
		const char *m_string = msg.c_str();
		Message *mm = createMessage(m_string, strlen(m_string));
		Message **mArray = &mm;
		cout << "going to send the message " << endl;
		//cout << *i << " "; // this will print all the contents of *features*
		ProduceRequest *Mrequest = createProduceRequest(*i, mArray, 1);
		ProduceResponse *Mresponse = c->sendProduceRequest(Mrequest);
		if (Mresponse == NULL){
			cerr << "an error ocurred while sending the produce request, errno = " << strerror(errno) << "\n";
		}
		else{
			if (Mresponse->hasErrorCode())
				cerr << "metadata publish error detected\n";
			else
				cout << "metadata message successfully published to kafka\n";
		}
		if (Mrequest != NULL) {
			delete Mrequest;
		}
		if (Mresponse != NULL){
			delete Mresponse;
		}
	}

	mtx.unlock();
	lastIntervalTime = lastIntervalTime+900;
	}
	_exit(1);
}


vector<Message*> messageVector;
MessageSet* messageSet;
ProduceMessageSet** produceMessageSetArray;
TopicNameBlock<ProduceMessageSet>** produceTopicArray;


Message* createMessage(const char * value, uint32_t recLen, const char *key)
{
// these will be updated as the message is prepared for production
   	const static int crc = 1001;
    const static signed char magicByte = -1;
	const static signed char attributes = 0; // last three bits must be zero to disable gzip compression
	int keyLength;

	unsigned char *v = new unsigned char[recLen];
	memcpy(v, value, recLen);

	unsigned char *k = new unsigned char[strlen(key)];
	memcpy(k, key, strlen(key));
	keyLength = strlen(key);

	Message * newmsg =  new Message(crc, magicByte, attributes, strlen(key), (unsigned char *)k, recLen, (unsigned char *)v, 0, true);
	return newmsg;
}


MessageSet* createMessageSet(Message **messageArray, int messageArraySize)
{
	int messageSetSize = 0;
	messageVector.clear();

	for (int i = 0 ; i < messageArraySize ; i++)
	{
		messageVector.push_back(messageArray[i]);
// sizeof(offset) + sizeof(messageSize) + messageSize
		messageSetSize += sizeof(long int) + sizeof(int) + messageArray[i]->getWireFormatSize(false);
	}

	return new MessageSet(messageSetSize, messageVector, true);
}

ProduceMessageSet* createProduceMessageSet(Message **messageArray, int messageArraySize)
{
	messageSet = createMessageSet(messageArray, messageArraySize);
	int messageSetSize = messageSet->getWireFormatSize(false);
// using partition = 0
	return new ProduceMessageSet(0, messageSetSize, messageSet, true);
}

TopicNameBlock<ProduceMessageSet>* createProduceRequestTopicNameBlock(string topic_name, Message **messageArray, int messageArraySize)
{
	const int produceMessageSetArraySize = 1;

	produceMessageSetArray = new ProduceMessageSet*[produceMessageSetArraySize];
	for (int i=0; i<produceMessageSetArraySize; i++) {
		produceMessageSetArray[i] = createProduceMessageSet(messageArray, messageArraySize);
	}
	return new TopicNameBlock<ProduceMessageSet>(topic_name, produceMessageSetArraySize, produceMessageSetArray, true);
}

ProduceRequest* createProduceRequest(string topic_name, Message **messageArray, int messageArraySize)
{
	const int correlationId = 212121;
	const string clientId = string("libkafka-test");
	const static int requiredAcks = 1;
	const static int timeout = 20;
	int bufferSize;

	int produceTopicArraySize = 1;
	produceTopicArray = new TopicNameBlock<ProduceMessageSet>*[produceTopicArraySize];
	for (int i=0; i<produceTopicArraySize; i++) {
		produceTopicArray[i] = createProduceRequestTopicNameBlock(topic_name, messageArray, messageArraySize);
	}

// changes for dynamic buffer allocation - starts

bufferSize += sizeof(short int) + sizeof(int);
bufferSize += sizeof(int);
for (int i=0; i<produceTopicArraySize; i++) {
bufferSize += produceTopicArray[i]->getWireFormatSize(false);
}

return new ProduceRequest(correlationId, clientId, requiredAcks, timeout, produceTopicArraySize, produceTopicArray, true, bufferSize);

// changes for dynamic buffer allocation - ends
}

//tests if the topic exists, if it does returns its metadata info, if not creates the topic

MetadataRequest* createMetadataRequest(const char* tempTopic)
{
	const int correlationId = 212121;
	const string clientId = string("libkafka-test");

	std::string topic_name(tempTopic);
	string *topicNameArray = new string[topicNameArraySize];
	topicNameArray[0] = topic_name;
	return new MetadataRequest(correlationId, clientId, topicNameArraySize, topicNameArray, true);

}
