#ifndef __CMQ_SDK_CLIENT_H__
#define __CMQ_SDK_CLIENT_H__

#include "cmq_http.h"
#include <stdlib.h>      
#include <time.h> 
#include <string>
#include <map>
#include <iostream>

using namespace std;

namespace cmq
{

class CMQClient
{
	protected:
		string endpoint;
		string path;
		string secretId;
		string secretKey;
		string method;
		
		CMQHttp http;
		
	public:
		CMQClient();
		
		CMQClient(const string &endpoint, const string &path, const string &secretId, const string &secretKey, const string &method="POST");

		CMQClient& operator=(const CMQClient &r)
		{
			if(this != &r)
			{
				this->endpoint = r.endpoint;
				this->path = r.path;
				this->secretId = r.secretId;
				this->secretKey = r.secretKey;
				this->method = r.method;
			}
			return *this;
		} 
		void setEndpoint(string endpoint)
		{
			this->endpoint = endpoint;
		}
		void setPath(string path)
		{
			this->path = path;
		}
		
		void setSecretId(string secretId)
		{
			this->secretId = secretId;
		}
		
		void setSecretKey(string secretKey)
		{
			this->secretKey = secretKey;
		}
		
		void setMethod(string method)
		{
			this->method = method;
		}
		
		string call(const string &action, map<string,string> &param);
};

}

#endif

