#include <crypto.h>
#include <log.h>

Crypto::Crypto():m_pool(NULL), m_driver(NULL),
	m_ctx(NULL), m_key(NULL) {
}

Crypto::~Crypto() {
}

int Crypto::init(const std::string &key, const std::string &salt, const std::string &iv) {
	apr_status_t rv;
	rv = apr_initialize();
	if(rv != APR_SUCCESS) {
		LOG_ERROR(log, "failed to perform apr initialize");
		return -1;
	}

	return 0;
}
