#ifndef __CMQ_SDK_ACCOUNT_H__
#define __CMQ_SDK_ACCOUNT_H__

#include "cmq_client.h"
#include "queue.h"
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

namespace cmq
{
	
/**Account�������̰߳�ȫ��������߳�ʹ�ã���Ҫÿ���̵߳�����ʼ��Account�����*/
class Account
{
	public:
		/**
		 * @brief             
		 * @param secretId    ����Ѷ�ƹ����鿴��api����ԿID
		 * @param secretKey   ����Ѷ�ƹ����鿴��api����Կ
		 * @param endpoint    ��Ϣ����api��ַ�����磺https://cmq-queue-gz.api.qcloud.com
		 * @param method	  ���󷽷�����ʹ��GET��POST����
		**/
		Account(const string &endpoint,const string &secretId, const string &secretKey, const string &method="POST");
		Account(const string &endpoint,const string &secretId, const string &secretKey, const string &path, const string &method);
		
		/**
		 * @brief             ��������
		 * @param queueName   ��������
		 * @param meta        �������Բ���
		**/
		void createQueue(const string &queueName,const QueueMeta &meta);
		/**
		 * @brief             ɾ������
		 * @param queueName   ��������
		**/
		void deleteQueue(const string &queueName);
		/**
		 * @brief             �г�Account�Ķ����б�
		 * @param totalCount  �����û��ʺ����ܶ�����
		 * @param vtQueue     ���ض����б�
		 * @param searchWord  ��ѯ�ؼ���
		 * @param offset      
		 * @param limit       
		**/
		void listQueue(int &totalCount,vector<string> &vtQueue, const string &searchWord="", int offset=-1, int limit=-1);
		/**
		 * @brief             ��ȡAccount��һ��Queue����    
		 * @return 		      
		**/
		Queue getQueue(const string &queueName);
		
	protected:
		CMQClient client;
};

}

#endif

