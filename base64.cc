#include <base64.h>
#include <apr-1/apr_base64.h>

Base64::Base64(){
}

Base64::~Base64(){
}

std::string Base64::encode(const std::vector<unsigned char> &input) const {
    int len = apr_base64_encode_len(input.size());
    std::string out;
    out.resize(len+1);
    apr_base64_encode(&out[0],(const char *)input.data(), input.size());
    out[out.length()] = 0;
    return out;
}

std::vector<unsigned char> Base64::decode(const std::string &input) const {
    int len = apr_base64_decode_len(input.c_str());
    std::vector<unsigned char> out;
    out.resize(len);
    int ret = apr_base64_decode((char *)out.data(), input.c_str());
    out.resize(ret);
   return out; 
}
