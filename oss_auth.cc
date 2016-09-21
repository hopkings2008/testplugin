#include <oss_auth.h>
#include <hmac.h>
#include <base64.h>

OssAuth::OssAuth(const BucketInfo &bucket, const UserInfo &user): m_bucketInfo(bucket), m_userInfo(user) {
}

OssAuth::~OssAuth() {
}

std::string OssAuth::getAuth(const AuthHeader &header) {
    std::string sig = getAuthSig(header);
    std::string auth = " OSS " + m_bucketInfo.ak + ":" +sig;
    return auth;
}

std::string OssAuth::getAuthSig(const AuthHeader &header) {
    std::string data = header.method + std::string("\n") + header.contentMd5 + std::string("\n") + header.contentType + std::string("\n");
    if(header.ossHeaders.length() != 0)
        data += header.ossHeaders + std::string("\n");
    data += header.resource;
    Hmac hmac(m_bucketInfo.sk);
    std::vector<unsigned char> mac = hmac.sha1(data);
    Base64 b64;
    std::string sig = b64.encode(mac);
    return sig;
}