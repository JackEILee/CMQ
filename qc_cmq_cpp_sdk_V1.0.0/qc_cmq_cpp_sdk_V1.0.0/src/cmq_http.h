#ifndef __CMQ_SDK_HTTP_H__
#define __CMQ_SDK_HTTP_H__

#include <curl/curl.h>
#include <string>
using namespace std;

namespace cmq
{
	
class CMQHttp
{	
	public:
		CMQHttp(int timout=120/*sec*/,bool isKeepAlive=true);
		~CMQHttp();
		
		void request(const string &method, const string &url, const string &req, string &rsp);
	
	protected:
		CURL *curl;
		int timeout;
		bool isKeepAlive;
};

}

#endif

