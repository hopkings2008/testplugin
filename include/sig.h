#ifndef SIG_H
#define SIG_H

#include <string>

typedef struct  _SIG_PARAM{
	std::string method;
	std::string md5;
	std::string type;
	std::string date;
	std::string oss;
	std::string resource;
}SIG_PARAM;

class Sig {
	public:
		Sig(const std::string &sk);
		~Sig();

		std::string sum(const SIG_PARAM &param);

	protected:
		std::string m_sk;
};

#endif
