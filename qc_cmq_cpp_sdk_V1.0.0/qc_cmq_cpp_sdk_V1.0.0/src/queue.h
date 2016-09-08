#ifndef __CMQ_SDK_QUEUE_H__
#define __CMQ_SDK_QUEUE_H__

#include "cmq_client.h"
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

using namespace std;
	
#define DEFAULT_POLLING_WAIT_SECONDS 0 			//ȱʡ��Ϣ���ճ���ѯ�ȴ�ʱ��
#define DEFAULT_VISIBILITY_TIMEOUT 30 			//ȱʡ��Ϣ�ɼ��Գ�ʱ
#define DEFAULT_MAX_MSG_SIZE 65536				//ȱʡ��Ϣ��󳤶�
#define DEFAULT_MSG_RETENTION_SECONDS 345600	//ȱʡ��Ϣ��������

namespace cmq
{
	
class QueueMeta
{
	public:
		QueueMeta();
		//���ѻ���Ϣ��
		int maxMsgHeapNum;
		//��Ϣ���ճ���ѯ�ȴ�ʱ��
		int pollingWaitSeconds;
		//��Ϣ�ɼ��Գ�ʱ
		int visibilityTimeout;
		//��Ϣ��󳤶�
		int maxMsgSize;
		//��Ϣ��������
		int msgRetentionSeconds;
		//���д���ʱ��
		int createTime;
		//������������޸�ʱ��
		int lastModifyTime;
		//���д���Active״̬����Ϣ����
		int activeMsgNum;
		//���д���Inactive״̬����Ϣ����
		int inactiveMsgNum;
};

class Message
{
	public:
		Message();
	
		//���������ص���ϢID
		string msgId;
		//ÿ������Ψһ����Ϣ���������ɾ���Ȳ���
		string receiptHandle;
		//��Ϣ��
		string msgBody;
		//��Ϣ���͵����е�ʱ�䣬�� 1970��1��1�� 00:00:00 000 ��ʼ�ĺ�����
        uint64_t enqueueTime;
		//��Ϣ�´οɼ���ʱ�䣬�� 1970��1��1�� 00:00:00 000 ��ʼ�ĺ�����
		uint64_t nextVisibleTime;
		//��Ϣ��һ�γ����е�ʱ�䣬�� 1970��1��1�� 00:00:00 000 ��ʼ�ĺ�����
		uint64_t firstDequeueTime;
		//�����д���
		int dequeueCount;
};

/**Queue�������̰߳�ȫ��������߳�ʹ�ã���Ҫÿ���̵߳�����ʼ��Queue�����*/
class Queue
{
	public:
		Queue(const string &queueName, const CMQClient &client);
	
		/**
		 * @brief             ���ö�������
		 * @param meta        �������Բ���
		**/
		void setQueueAttributes(const QueueMeta &meta);
		/**
		 * @brief             ��ȡ��������
		 * @param meta        ���صĶ������Բ���
		**/
		void getQueueAttributes(QueueMeta &meta);
		/**
		 * @brief             ������Ϣ
		 * @param msgBody     ��Ϣ����
		 * @param msgId       ���������ص���ϢΨһ��ʶ
		**/
		void sendMessage(const string &msgBody, string &msgId);
		/**
		 * @brief             ����������Ϣ
		 * @param vtMsgBody   ��Ϣ�б�
		 * @param vtMsgId     ���������ص���ϢΨһ��ʶ�б�
		**/
		void batchSendMessage(const vector<string> &vtMsgBody, vector<string> &vtMsgId);
		/**
		 * @brief             ��ȡ��Ϣ
		 * @param pollingWaitSeconds     �������Polling�ȴ�ʱ��
		 * @param msg         ������������Ϣ
		**/
		void receiveMessage(int pollingWaitSeconds, Message &msg);
		/**
		 * @brief             ������ȡ��Ϣ
		 * @param numOfMsg    ׼����ȡ��Ϣ��
		 * @param pollingWaitSeconds     �������Polling�ȴ�ʱ��
		 * @param msgList     ������������Ϣ�б�
		**/
		void batchReceiveMessage(int numOfMsg, int pollingWaitSeconds, vector<Message> &msgList);
		/**
		 * @brief             ɾ����Ϣ
		 * @param receiptHandle     ��Ϣ�������ȡ��Ϣʱ�ɷ���������
		**/
		void deleteMessage(const string &receiptHandle);
		/**
		 * @brief             ����ɾ����Ϣ
		 * @param receiptHandle     ��Ϣ����б���ȡ��Ϣʱ�ɷ���������
		**/
		void batchDeleteMessage(const vector<string> &vtReceiptHandle);
	protected:
		string queueName;
		CMQClient client;
};

}

#endif

