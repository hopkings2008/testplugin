#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <vector>
#include <apr-1/apr_crypto.h>

class Crypto {
	public:
		Crypto();
		virtual ~Crypto();
		int init(const std::string &key, const std::string &salt, const std::string &iv);
        int encrypt(const std::string &plain, std::vector<unsigned char> &cipher) const;
        int decrypt(const std::vector<unsigned char> &cipher, std::string &plain) const;

    protected:
        std::string getError()const;

	protected:
		apr_pool_t *m_pool;
		const apr_crypto_driver_t *m_driver;
		apr_crypto_t *m_ctx;
		apr_crypto_key_t *m_key;
		std::string m_iv;
};

#endif
