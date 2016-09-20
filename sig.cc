#include "sig.h"
#include <string>
#include <stdio.h>
#include <string.h>
#include <openssl/hmac.h>
#include <apr-1/apr_base64.h>

Sig::Sig(const std::string &sk):m_sk(sk) {
}

Sig::~Sig(){
}

std::string Sig::sum(const SIG_PARAM &param) {
	std::string ret = "";
	std::string data = param.method + std::string("\n") + param.md5 + std::string("\n") + param.type + std::string("\n") + param.date + std::string("\n") + param.oss + std::string("\n") + param.resource;
	unsigned int len = 20;
	unsigned char *result = NULL;

	result = (unsigned char*)malloc(sizeof(char) * len);

	HMAC_CTX ctx;
	HMAC_CTX_init(&ctx);


	HMAC_Init_ex(&ctx, m_sk.c_str(), m_sk.length(), EVP_sha1(), NULL);
	HMAC_Update(&ctx, (unsigned char*)data.c_str(), data.length());
	HMAC_Final(&ctx, result, &len);
	HMAC_CTX_cleanup(&ctx);

	int enc_len = apr_base64_encode_len(len);
	char *out = new char[enc_len];
	memset((void *)out, 0, enc_len);

	apr_base64_encode(out, (const char *)result, len);

	ret = out;
	free(result);
	delete[] out;
	return ret;
}

/*
int main() {
	const char key[] = u8"OtxrzxIsfpFjA7SwPzILwy8Bw21TLhquhboDYROV";


	char data[] = u8"PUT\nODBGOERFMDMzQTczRUY3NUE3NzA5QzdFNUYzMDQxNEM=\ntext/html\nThu, 17 Nov 2005 18:49:58 GMT\nx-oss-magic:abracadabra\nx-oss-meta-author:foo@bar.com\n/oss-example/nelson";


	unsigned char* result;
	unsigned int len = 20;

	result = (unsigned char*)malloc(sizeof(char) * len);

	HMAC_CTX ctx;
	HMAC_CTX_init(&ctx);


	HMAC_Init_ex(&ctx, key, strlen(key), EVP_sha1(), NULL);
	HMAC_Update(&ctx, (unsigned char*)&data, strlen(data));
	HMAC_Final(&ctx, result, &len);
	HMAC_CTX_cleanup(&ctx);

	printf("HMAC digest: ");

	for (int i = 0; i != (int)len; i++)
		printf("%02x", (unsigned int)result[i]);

	printf("\n");
	printf("len: %d\n", len);

	int enc_len = apr_base64_encode_len(len);
	printf("enc_len: %d\n", enc_len);
	char *out = new char[enc_len];
	memset((void *)out, 0, enc_len);

	int out_len = apr_base64_encode(out, (const char *)result, len);
	printf("out_len: %d, base64(%s)\n", out_len, out);

	free(result);
	delete[] out;

	std::string real = "26NBxoKdsyly4EDv6inkoDft/yA=";

	return 0;
}
*/
