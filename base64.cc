#include <base64.h>
#include <apr-1/apr_base64.h>

Base64::Base64(){
}

Base64::~Base64(){
}

std::string Base64::encode(const std::vector<unsigned char> &input) {
    int len = apr_base64_encode_len(input.size());
    std::string out;
    out.reserve(len);
    apr_base64_encode(&out[0],(const char *)input.data(), input.size());
    return out;
}

std::vector<unsigned char> decode(const std::string &input) {
    int len = apr_base64_decode_len(input.c_str());
    std::vector<unsigned char> out;
    out.reserve(len);
    apr_base64_encode((char *)out.data(), input.c_str(), input.length());
   return out; 
}
