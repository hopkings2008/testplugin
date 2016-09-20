#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <vector>

class Base64 {
	public:
		Base64();
		~Base64();
		std::string encode(const std::vector<char> &input);
		std::vector<char> decode(const std::string &input);
};

#endif
