#ifndef HMAC_H
#define HMAC_H

#include <string>
#include <vector>

class Hmac {
    public:
        Hmac(const std::string &key);
        ~Hmac();
        std::vector<unsigned char> sha1(const std::string &input);

    protected:
        std::string m_key;
};

#endif
