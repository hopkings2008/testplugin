#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <apr-1/apr_crypto.h>

class Crypto {
	public:
		Crypto();
		virtual ~Crypto();
		int init(const std::string &key, const std::string &salt, const std::string &iv);

	protected:
		apr_pool_t *m_pool;
		apr_crypto_driver_t *m_driver;
		apr_crypto_t *m_ctx;
		apr_crypto_key_t *m_key;
		std::string m_iv;
};

#endif
