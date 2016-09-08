#include "account.h"
#include "cmq_exception.h"
#include "json/json.h"
#include "cmq_tool.h"

using namespace cmq;
	
Account::Account(const string &endpoint, const string &secretId, const string &secretKey, const string &method)
:client(endpoint, "/v2/index.php", secretId, secretKey, method)
{
}
		
Account::Account(const string &endpoint, const string &secretId, const string &secretKey, const string &path, const string &method)
:client(endpoint, path, secretId, secretKey, method)
{
}
		
void Account::createQueue(const string &queueName,const QueueMeta &meta)
{
	map<string, string> param;
	if(queueName == "")
	{
		throw CMQClientException("Invalid parameter:queueName is empty");
		return;
	}
	else
		param["queueName"] = queueName;
	
	if(meta.maxMsgHeapNum > 0)
		param["maxMsgHeapNum"] = CMQTool::int2str(meta.maxMsgHeapNum);
	if(meta.pollingWaitSeconds > 0)
		param["pollingWaitSeconds"] = CMQTool::int2str(meta.pollingWaitSeconds);
	if(meta.visibilityTimeout > 0)
		param["visibilityTimeout"] = CMQTool::int2str(meta.visibilityTimeout);
	if(meta.maxMsgSize > 0)
		param["maxMsgSize"] = CMQTool::int2str(meta.maxMsgSize);
	if(meta.msgRetentionSeconds > 0)
		param["msgRetentionSeconds"] = CMQTool::int2str(meta.msgRetentionSeconds);
	
	string result = this->client.call("CreateQueue", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
		throw CMQClientException("Json parse failed");
	
	int code = value["code"].asInt();
	if(code != 0)
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
}
		
void Account::deleteQueue(const string &queueName)
{
	map<string,string> param;
	param["queueName"] = queueName;
	string result = this->client.call("DeleteQueue", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
		throw CMQClientException("Json parse failed");
	
	int code = value["code"].asInt();
	if(code != 0)
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
}
		
void Account::listQueue(int &totalCount, vector<string> &vtQueue, const string &searchWord, int offset, int limit)
{
	map<string,string> param;
	if(searchWord !="")
		param["searchWord"] = searchWord;
	if(offset >=0 )
		param["offset"] = offset;
	if(limit > 0 )
		param["limit"] = limit;

	string result = this->client.call("ListQueue", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
	{
		throw CMQClientException("Json parse failed");
		return;
	}
	
	int code = value["code"].asInt();
	if(code != 0)
	{
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
		return;
	}
	
	totalCount = value["totalCount"].asInt();
	Json::Value valQueueList = value["queueList"];
	for(unsigned int i=0;i<valQueueList.size();i++)
	{		
		vtQueue.push_back(valQueueList[i]["queueName"].asString());
	}
}

Queue Account::getQueue(const string &queueName)
{
	return Queue(queueName,this->client);
}
