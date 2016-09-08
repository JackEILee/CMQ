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
	
/**Account类对象非线程安全，如果多线程使用，需要每个线程单独初始化Account类对象*/
class Account
{
	public:
		/**
		 * @brief             
		 * @param secretId    从腾讯云官网查看云api的密钥ID
		 * @param secretKey   从腾讯云官网查看云api的密钥
		 * @param endpoint    消息服务api地址，例如：https://cmq-queue-gz.api.qcloud.com
		 * @param method	  请求方法，可使用GET或POST方法
		**/
		Account(const string &endpoint,const string &secretId, const string &secretKey, const string &method="POST");
		Account(const string &endpoint,const string &secretId, const string &secretKey, const string &path, const string &method);
		
		/**
		 * @brief             创建队列
		 * @param queueName   队列名字
		 * @param meta        队列属性参数
		**/
		void createQueue(const string &queueName,const QueueMeta &meta);
		/**
		 * @brief             删除队列
		 * @param queueName   队列名字
		**/
		void deleteQueue(const string &queueName);
		/**
		 * @brief             列出Account的队列列表
		 * @param totalCount  返回用户帐号下总队列数
		 * @param vtQueue     返回队列列表
		 * @param searchWord  查询关键字
		 * @param offset      
		 * @param limit       
		**/
		void listQueue(int &totalCount,vector<string> &vtQueue, const string &searchWord="", int offset=-1, int limit=-1);
		/**
		 * @brief             获取Account的一个Queue对象    
		 * @return 		      
		**/
		Queue getQueue(const string &queueName);
		
	protected:
		CMQClient client;
};

}

#endif

