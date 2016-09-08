#include <iostream>
#include <vector>
#include <fstream>
#include "cmq_exception.h"
#include "account.h"
#include "queue.h"
#include <thread>
#include <ctime>

using namespace std;
using namespace cmq;

void sendMsg(Queue queue, const string &s) {
	string MsId;
	queue.sendMessage(s,MsId);
	//cout <<"This ["<< s << "] is sent." << endl;

}

int SendMsg(void) {
	try {
		time_t start, end;
		start = clock();
		string secretId = "AKID2BeGz25vdVZpBjtA1zvx6saTSb9ndORc";
		string secretKey = "ZHCMfGAPsYSaxdVcztyQ4sojnzInUHGy";
		string endpoint = "http://cmq-queue-gz.api.qcloud.com";
		Account account(endpoint, secretId, secretKey);
		QueueMeta meta;
		meta.pollingWaitSeconds = 10;
		account.deleteQueue("JackEI-Lee-first-Queue");
		account.createQueue("JackEI-Lee-first-Queue", meta);
		vector<string> s;
		int totalCount;
		account.listQueue(totalCount, s);
		for (int i = 0; i < totalCount; i++) {
			cout << s[i] << " is ctreated." << endl;
		}
		Queue queue = account.getQueue("JackEI-Lee-first-Queue");
		string MsId;
		//time_t start, end;
		start = clock();
		string si = "This is the ";
		for (int i = 0; i < 100; i++) {
			string ss = si + to_string(i) + " meseage";
			thread mythread(sendMsg, queue, std::ref(ss));
			mythread.join();
			//this_thread::sleep_for(std::chrono::microseconds(10));
		}
		end = clock();
		cout << "*******����request�ĺ�ʱ*******" << (end - start)<<"����"<<endl;
	}
	catch(CMQServerException &e){
		cout << "Server Action failed,code:" << e.getCode() << ",message:" << e.getMessage() << ",requestId:" << e.getRequestId() << endl;
		return -2;
	}
	catch (CMQServerNetworkException &e){
		        cout<<"Server Network exception,http status: "<<e.getStatus()<< endl;
		        return -1;
	}
	catch (CMQClientException &e){
		        cout<<"CMQ Client Exceptrion:"<<e.what()<< endl;
		        return -2;
    }
	return 0;

}

int main() {
	int i=SendMsg();
	if (i == 0)
		cout << "create successful!"<<endl;
	else cout << "error!!"<<endl;
	return 0;
}

//int main()
//{
//    //����Ѷ�ƹ�����ѯ����API��Կ��Ϣ
//	string secretId="AKID2BeGz25vdVZpBjtA1zvx6saTSb9ndORc";
//	string secretKey="ZHCMfGAPsYSaxdVcztyQ4sojnzInUHGy";
//	string endpoint = "http://cmq-queue-gz.api.qcloud.com";
//	
//    try
//    {
//		Account account(endpoint, secretId, secretKey);
//		
//		account.deleteQueue("queue-test10");
//		//��������
//		cout<<"---------------create queue ...---------------"<<endl;
//		QueueMeta meta;
//		meta.pollingWaitSeconds = 10;
//		meta.visibilityTimeout = 10;
//		meta.maxMsgSize = 65536;
//		meta.msgRetentionSeconds = 345600;
//		account.createQueue("queue-test10",meta);
//		cout<<"queue-test10 created"<<endl;
//		account.createQueue("queue-test11",meta);
//		cout<<"queue-test11 created"<<endl;
//		account.createQueue("queue-test12",meta);
//		cout<<"queue-test12 created"<<endl;
//		//account.createQueue("queue-test13",meta);
//		//cout<<"queue-test13 created"<<endl;
//		//account.createQueue("queue-test14",meta);
//		//cout<<"queue-test14 created"<<endl;
//		
//		//�г���ǰ�ʺ������ж�������
//		cout<<"---------------list queue ...---------------"<<endl;
//		vector<string> vtQueue;
//		int totalCount = 0;
//		account.listQueue(totalCount,vtQueue);
//		cout<<"totalCount:"<<totalCount<<endl;
//		for(size_t i=0;i<vtQueue.size();i++)
//		{
//			cout<<"queueName:"<<vtQueue[i]<<endl;
//		}
//		
//		//ɾ������
//		cout<<"---------------delete queue ...---------------"<<endl;
//		account.deleteQueue("queue-test11");
//		cout<<"queue-test11 deleted"<<endl;
//		account.deleteQueue("queue-test12");
//		cout<<"queue-test12 deleted"<<endl;
//
//		//��ö���ʵ��
//		cout<<"--------------- queue[queue-test10] ---------------"<<endl;
//		Queue queue = account.getQueue("queue-test10");
//		
//		//���ö�������
//		cout<<"---------------set queue attributes ...---------------"<<endl;
//		QueueMeta meta1;
//		meta1.pollingWaitSeconds = 20;
//		queue.setQueueAttributes(meta1);
//		cout<<"pollingWaitSeconds=20 set"<<endl;
//		
//		//��ȡ��������
//		cout<<"---------------get queue attributes ...---------------"<<endl;
//		QueueMeta meta2;
//		queue.getQueueAttributes(meta2);
//		cout<<"maxMsgHeapNum:"<<meta2.maxMsgHeapNum<<endl;
//		cout<<"pollingWaitSeconds:"<<meta2.pollingWaitSeconds<<endl;
//		cout<<"visibilityTimeout:"<<meta2.visibilityTimeout<<endl;
//		cout<<"maxMsgSize:"<<meta2.maxMsgSize<<endl;
//		cout<<"createTime:"<<meta2.createTime<<endl;
//		cout<<"lastModifyTime:"<<meta2.lastModifyTime<<endl;
//		cout<<"activeMsgNum:"<<meta2.activeMsgNum<<endl;
//		cout<<"inactiveMsgNum:"<<meta2.inactiveMsgNum<<endl;
//		
//		//������Ϣ
//		cout<<"---------------send message ...---------------"<<endl;
//		string msgId;
//		queue.sendMessage("hello world,this is cmq sdk for c++",msgId);
//		cout<<"[hello world,this is cmq sdk for c++] sent"<<endl;
//		
//		//������Ϣ
//		cout<<"---------------recv message ...---------------"<<endl;
//		Message msg;
//		queue.receiveMessage(10,msg);
//		cout<<"msgId:"<<msg.msgId<<endl;
//		cout<<"receiptHandle:"<<msg.receiptHandle<<endl;
//		cout<<"msgBody:"<<msg.msgBody<<endl;
//		cout<<"enqueueTime:"<<msg.enqueueTime<<endl;
//		cout<<"nextVisibleTime:"<<msg.nextVisibleTime<<endl;
//		cout<<"firstDequeueTime:"<<msg.firstDequeueTime<<endl;
//		cout<<"dequeueCount:"<<msg.dequeueCount<<endl;
//		
//		//ɾ����Ϣ
//		cout<<"---------------delete message ...---------------"<<endl;
//		queue.deleteMessage(msg.receiptHandle);
//		cout<<"receiptHandle:"<<msg.receiptHandle<<" deleted"<<endl;
//		
//		//��������
//		//����������Ϣ
//		cout<<"---------------batch send message ...---------------"<<endl;
//		vector<string> vtMsgId;
//		vector<string> vtMsgBody;
//		string msgBody = "hello world,this is cmq sdk for c++ 1";
//		vtMsgBody.push_back(msgBody);
//		msgBody = "hello world,this is cmq sdk for c++ 2";
//		vtMsgBody.push_back(msgBody);
//		msgBody = "hello world,this is cmq sdk for c++ 3";
//		vtMsgBody.push_back(msgBody);
//		queue.batchSendMessage(vtMsgBody,vtMsgId);
//		for(size_t i=0;i<vtMsgBody.size();i++)
//			cout<<"["<<vtMsgBody[i]<<"] sent"<<endl;	
//		for(size_t i=0;i<vtMsgId.size();i++)
//			cout<<"msgId:"<<vtMsgId[i]<<endl;
//		
//		vector<string> vtReceiptHandle;
//		//����������Ϣ
//		cout<<"---------------batch recv message ...---------------"<<endl;
//		vector<Message> msgList;
//		queue.batchReceiveMessage(10,10,msgList);
//		cout<<"recv msg count:"<<msgList.size()<<endl;
//		for(size_t i=0;i<msgList.size();i++)
//		{
//			Message &msg1 = msgList[i];
//			cout<<"msgId:"<<msg1.msgId<<endl;
//			cout<<"receiptHandle:"<<msg1.receiptHandle<<endl;
//			cout<<"msgBody:"<<msg1.msgBody<<endl;
//			cout<<"enqueueTime:"<<msg1.enqueueTime<<endl;
//			cout<<"nextVisibleTime:"<<msg.nextVisibleTime<<endl;
//			cout<<"firstDequeueTime:"<<msg.firstDequeueTime<<endl;
//			cout<<"dequeueCount:"<<msg.dequeueCount<<endl;
//			
//			vtReceiptHandle.push_back(msg1.receiptHandle);
//		}
//		
//		//����ɾ����Ϣ
//		queue.batchDeleteMessage(vtReceiptHandle);
//		cout<<"---------------batch delete message ...---------------"<<endl;
//		for(size_t i=0;i<vtReceiptHandle.size();i++)
//			cout<<"receiptHandle:"<<vtReceiptHandle[i]<<" deleted"<<endl;
//
//    }
//	catch(CMQServerNetworkException &e)
//    {
//        cout<<"Server Network exception,http status: "<<e.getStatus()<< endl;
//        return -1;
//    }
//    catch(CMQServerException &e)
//    {
//        cout<<"Server Action failed,code:"<< e.getCode()<<",message:"<<e.getMessage()<<",requestId:"<<e.getRequestId()<< endl;
//        return -1;
//    }
//    catch(CMQClientException &e)
//    {
//        cout<<"CMQ Client Exceptrion:"<<e.what()<< endl;
//        return -2;
//    }
//}
