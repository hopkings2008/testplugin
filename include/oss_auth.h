#ifndef OSS_AUTH_H
#define OSS_AUTH_H

#include <string>

typedef struct _AuthHeader{
    std::string method;
    std::string contentMd5;
    std::string contentType;
    std::string date;
    std::string ossHeaders;
    std::string resource;
    _AuthHeader(const struct _AuthHeader &h) {
        this->method = h.method;
        this->contentMd5 = h.contentMd5;
        this->contentType = h.contentType;
        this->date = h.date;
        this->ossHeaders = h.ossHeaders;
        this->resource = h.resource;
    }
}AuthHeader;

typedef struct _BucketInfo{
    std::string bucket;
    std::string ak;
    std::string sk;
    _BucketInfo(const struct _BucketInfo &info) {
        this->bucket = info.bucket;
        this->ak = info.ak;
        this->sk = info.sk;
    }
}BucketInfo;

typedef struct _UserInfo{
    int uid;
}UserInfo;

class OssAuth {
    public:
        OssAuth(const BucketInfo &bucket, const UserInfo &user);
        ~OssAuth();
        std::string getAuth(const AuthHeader &header);

    protected:
        std::string getAuthSig(const AuthHeader &header);

    protected:
        BucketInfo m_bucketInfo;
        UserInfo m_userInfo;
};

#endif
