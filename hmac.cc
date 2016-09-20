#include <hmac.h>
#include <openssl/hmac.h>

Hmac::Hmac(const std::string &key):m_key(key){
}

Hmac::~Hmac() {
}

std::vector<unsigned char> Hmac::sha1(const std::string &input) {
    std::vector<unsigned char> out;
    unsigned int len = 20;
    out.reserve(len);
    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, m_key.c_str(), m_key.length(), EVP_sha1(), NULL);
    HMAC_Update(&ctx, (unsigned char *)input.c_str(), input.length());
    HMAC_Final(&ctx, out.data(), &len);
    HMAC_CTX_cleanup(&ctx);
    return out;
}

