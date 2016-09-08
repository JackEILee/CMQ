#ifndef __CMQ_SDK_TOOL_H__
#define __CMQ_SDK_TOOL_H__

#include <string>
using namespace std;

namespace cmq
{
	
typedef unsigned char byte;

class CMQTool
{
	public:
	
		static string int2str(int n);

		static string url_encode(const string &src);

		static string base64_encode(const byte *src, int length);

		static void hmac_sha1(const byte *key, int key_len,const byte *data, int data_len, byte *digest);

		static string sign(const string &src, const string &key);
};

}

#endif

