#include "queue.h"
#include "cmq_exception.h"
#include "json/json.h"
#include "cmq_tool.h"

using namespace cmq;

QueueMeta::QueueMeta()
:maxMsgHeapNum(-1)
,pollingWaitSeconds(DEFAULT_POLLING_WAIT_SECONDS)
,visibilityTimeout(DEFAULT_VISIBILITY_TIMEOUT)
,maxMsgSize(DEFAULT_MAX_MSG_SIZE)
,msgRetentionSeconds(DEFAULT_MSG_RETENTION_SECONDS)
,createTime(-1),lastModifyTime(-1),activeMsgNum(-1),inactiveMsgNum(-1)
{
}

Message::Message()
:msgId("")
,receiptHandle("")
,msgBody("")
,enqueueTime(0)
,nextVisibleTime(0)
,firstDequeueTime(0)
,dequeueCount(0)
{
}

Queue::Queue(const string &queueName, const CMQClient &client)
{
	this->queueName = queueName;
	this->client = client;
}
	
void Queue::setQueueAttributes(const QueueMeta &meta)
{
	map<string, string> param;
	param["queueName"] = this->queueName;
	
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
	
	string result = this->client.call("SetQueueAttributes", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
		throw CMQClientException("Json parse failed");
	
	int code = value["code"].asInt();
	if(code != 0)
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
}
		
void Queue::getQueueAttributes(QueueMeta &meta)
{
	map<string,string> param;
	param["queueName"] = this->queueName;
	string result = this->client.call("GetQueueAttributes", param);
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

	meta.maxMsgHeapNum = value["maxMsgHeapNum"].asInt();
	meta.pollingWaitSeconds = value["pollingWaitSeconds"].asInt();
	meta.visibilityTimeout = value["visibilityTimeout"].asInt();
	meta.maxMsgSize = value["maxMsgSize"].asInt();
	meta.msgRetentionSeconds = value["msgRetentionSeconds"].asInt();
	meta.createTime = value["createTime"].asInt();
	meta.lastModifyTime = value["lastModifyTime"].asInt();
	meta.activeMsgNum = value["activeMsgNum"].asInt();
	meta.inactiveMsgNum = value["inactiveMsgNum"].asInt();
}
		
void Queue::sendMessage(const string &msgBody, string &msgId)
{
	map<string,string> param;
	param["queueName"] = this->queueName;
	param["msgBody"] = msgBody;
	string result = this->client.call("SendMessage", param);

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
		
	msgId = value["msgId"].asString();
}
		
void Queue::batchSendMessage(const vector<string> &vtMsgBody, vector<string> &vtMsgId)
{
	if(vtMsgBody.empty())
		return;
	map<string,string> param;
	param["queueName"] = this->queueName;
	for(size_t i=0;i<vtMsgBody.size();i++)
	{
		string k = "msgBody." + CMQTool::int2str(i+1);
		param[k] = vtMsgBody[i];
	}
	string result = this->client.call("BatchSendMessage", param);
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

	Json::Value valMsgList = value["msgList"];
	for(unsigned int i=0;i<valMsgList.size();i++)
	{
		string msgId = valMsgList[i]["msgId"].asString();
		vtMsgId.push_back(msgId);
	}
}
		
void Queue::receiveMessage(int pollingWaitSeconds, Message &msg)
{
	map<string,string> param;
	param["queueName"] = this->queueName;
	param["pollingWaitSeconds"] = CMQTool::int2str(pollingWaitSeconds);
	string result = this->client.call("ReceiveMessage", param);

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

	msg.msgId = value["msgId"].asString();
	msg.receiptHandle = value["receiptHandle"].asString();
	msg.msgBody = value["msgBody"].asString();
	msg.enqueueTime = value["enqueueTime"].asUInt64();
	msg.nextVisibleTime = value["nextVisibleTime"].asUInt64();
	msg.firstDequeueTime = value["firstDequeueTime"].asUInt64();
	msg.dequeueCount = value["dequeueCount"].asInt();
}
		
void Queue::batchReceiveMessage(int numOfMsg, int pollingWaitSeconds, vector<Message> &msgList)
{
	map<string,string> param;
	param["queueName"] = this->queueName;
	param["numOfMsg"] = CMQTool::int2str(numOfMsg);
	param["pollingWaitSeconds"] = CMQTool::int2str(pollingWaitSeconds);
	string result = this->client.call("BatchReceiveMessage", param);
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
	
	Json::Value valMsgList = value["msgInfoList"];
	for(unsigned int i=0;i<valMsgList.size();i++)
	{
		Json::Value &valMsg = valMsgList[i];
		Message msg;
		msg.msgId = valMsg["msgId"].asString();
		msg.receiptHandle = valMsg["receiptHandle"].asString();
		msg.msgBody = valMsg["msgBody"].asString();
		msg.enqueueTime = valMsg["enqueueTime"].asUInt64();
		msg.nextVisibleTime = value["nextVisibleTime"].asUInt64();
		msg.firstDequeueTime = value["firstDequeueTime"].asUInt64();
		msg.dequeueCount = value["dequeueCount"].asInt();

		msgList.push_back(msg);
	}
}
		
void Queue::deleteMessage(const string &receiptHandle)
{
	map<string,string> param;
	param["queueName"] = this->queueName;
	param["receiptHandle"] = receiptHandle;
	string result = this->client.call("DeleteMessage", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
		throw CMQClientException("Json parse failed");
	
	int code = value["code"].asInt();
	if(code != 0)
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
}
		
void Queue::batchDeleteMessage(const vector<string> &vtReceiptHandle)
{
	if(vtReceiptHandle.empty())
		return;
	map<string,string> param;
	param["queueName"] = this->queueName;
	for(size_t i=0;i<vtReceiptHandle.size();i++)
	{
		string k = "receiptHandle." + CMQTool::int2str(i+1);
		param[k] = vtReceiptHandle[i];
	}
	string result = this->client.call("BatchDeleteMessage", param);
	Json::Reader reader;
	Json::Value value;
	if(!reader.parse(result, value))
		throw CMQClientException("Json parse failed");
	
	int code = value["code"].asInt();
	if(code != 0)
		throw CMQServerException(code,value["message"].asString(),value["requestId"].asString());
}
