#include "cmq_tool.h"
#include "sha1.h"
//#include <openssl/hmac.h>
#include <string>
#include <sstream>

using namespace std;
using namespace cmq;

string CMQTool::int2str(int n)
{
	stringstream ss;
	ss<<n;
	return ss.str();
}

string CMQTool::url_encode(const string &src)
{
	static const char *hex = "0123456789ABCDEF";

	string dst;
	for(size_t i=0; i<src.size(); i++)
	{
		byte c = (byte)src[i];
		if(isalnum(c) ||(c == '-') || (c == '_') || (c == '.') || (c == '!') || 
           (c == '~') || (c == '*') || (c == '\'') || (c == '(') ||
           (c == ')') || (c == '/'))
		{
            dst += c;
        }
		else if(c == ' ') 
		{
            dst += "%20";
        }
		else 
		{
            dst += '%';
            dst += hex[c >> 4];
            dst += hex[c & 15];
        }
	}
    
	return dst;
}

string CMQTool::base64_encode(const byte *src, int length)
{
	static const char *b64c = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	string dst;
	const byte *p = src;
	
    while(length) 
	{
        dst += b64c[*p >> 2];
        if(!--length)
		{
            dst += b64c[(*p & 0x3) << 4];
            dst += "==";
            break;
        }
        dst += b64c[((*p & 0x3) << 4) | (*(p + 1) >> 4)];
        p++;
        if(!--length)
		{
            dst += b64c[(*p & 0xF) << 2];
            dst += '=';
            break;
        }
        dst += b64c[((*p & 0xF) << 2) | (*(p + 1) >> 6)];
        p++;
		
        dst	+= b64c[*p & 0x3F];
        p++, length--;
    }

    return dst;
}

//HMAC-SHA-1:
/*void CMQTool::hmac_sha1(const byte *key, int key_len, const byte *data, int data_len, byte *digest)
{
	unsigned int digest_len = 0;
    HMAC(EVP_sha1(), key, key_len, (unsigned char*)data, data_len, digest, &digest_len);
}*/

void CMQTool::hmac_sha1(const byte *key, int key_len, const byte *data, int data_len, byte *digest)
{
    byte kopad[64], kipad[64];
    int i;
    byte tmp[20];
    SHA_INFO si;
    
	if(key_len > 64) 
		key_len = 64;

    for(i = 0; i < key_len; i++) 
	{
        kopad[i] = key[i] ^ 0x5c;
        kipad[i] = key[i] ^ 0x36;
    }

    for( ; i < 64; i++) 
	{
        kopad[i] = 0 ^ 0x5c;
        kipad[i] = 0 ^ 0x36;
    }

    sha_init(&si);
    sha_update(&si, (byte *)kipad, 64);
    sha_update(&si, (byte *)data, data_len);
    sha_final(tmp, &si);

    sha_init(&si);
    sha_update(&si, (byte *)kopad, 64);
    sha_update(&si, (byte *)tmp, 20);
    sha_final(digest, &si);
}

string CMQTool::sign(const string &src, const string &key)
{
	byte digest[20]={0};
	hmac_sha1((const byte *)key.data(),key.size(),(const byte *)src.data(),src.size(),digest);

	return base64_encode(digest,20);
}
