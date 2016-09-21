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
    _AuthHeader(std::string &method, std::string &md5, std::string &type, std::string &date,
		std::string &hdrs, std::string &r): method(method), contentMd5(md5),
	contentType(type), date(date), ossHeaders(hdrs), resource(r) {
    }
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
    _BucketInfo(const std::string &bucket, const std::string &ak, const std::string &sk):
	    bucket(bucket), ak(ak), sk(sk) {
    }
    _BucketInfo(const struct _BucketInfo &info):
	    bucket(info.bucket), ak(info.ak), sk(info.sk){
    }
}BucketInfo;

typedef struct _UserInfo{
    int uid;
    _UserInfo(int id):uid(id){
    }
    _UserInfo(const struct _UserInfo &info):uid(info.uid){
    }
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
