#include <crypto.h>
#include <log.h>

Crypto::Crypto():m_pool(NULL), m_driver(NULL),
	m_ctx(NULL), m_key(NULL) {
}

Crypto::~Crypto() {
    if (m_pool) {
        apr_pool_destroy(m_pool);
        m_pool = NULL;
    }
    apr_terminate();
}

int Crypto::init(const std::string &key, const std::string &salt, const std::string &iv) {
	apr_status_t rv;
    const apu_err_t *result = NULL;
	rv = apr_initialize();
	if(rv != APR_SUCCESS) {
		TSError(MODULE.c_str(), "failed to perform apr initialize, err: %s", getError().c_str());
		return -1;
	}
    rv = apr_pool_create(&m_pool, NULL);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to create pool, err: %s", getError().c_str());
        return -1;
    }
    rv = apr_crypto_init(m_pool);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to init pool, err: %s", getError().c_str());
        return -1;
    }
    rv = apr_crypto_get_driver(&m_driver, "openssl", NULL, &result, m_pool);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to get driver, err: %s", result->msg);
        return -1;
    }

    apr_crypto_make(&m_ctx, m_driver, "engine=openssl", m_pool);
    if (!m_ctx) {
        TSError(MODULE.c_str(), "failed to get context.");
        return -1;
    }

    rv = apr_crypto_passphrase(&m_key, NULL, key.c_str(), key.length(), (unsigned char *)salt.c_str(), salt.length(), APR_KEY_AES_256, APR_MODE_CBC, 1, 4096, m_ctx, m_pool);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to create key, err: %s", getError().c_str());
        return -1;
    }

    m_iv = iv;

	return 0;
}

int Crypto::encrypt(const std::string &plain, std::vector<unsigned char> &cipher) const {
    apr_status_t rv = 0;
    unsigned char *cipherText = NULL;
    apr_size_t cipherTextLen = 0;
    apr_crypto_block_t *block = NULL;
    apr_size_t blockSize = 0;
    apr_size_t len = 0;
    const unsigned char *iv = (const unsigned char *)m_iv.c_str();
    rv = apr_crypto_block_encrypt_init(&block, &iv, m_key, &blockSize, m_pool);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to init cipher, err: %s", getError().c_str());
        return -1;
    }
    if(!block || rv) {
        TSError(MODULE.c_str(), "failed to init cipher, block or rv is invalid.");
        return -1;
    }
    rv = apr_crypto_block_encrypt(&cipherText, &cipherTextLen, (const unsigned char*)plain.c_str(), plain.length(), block);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to encrypt, err: %s", getError().c_str());
        return -1;
    }
    rv = apr_crypto_block_encrypt_finish(cipherText + cipherTextLen, &len, block);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to finish encrypt, err: %s", getError().c_str());
        return -1;
    }
    cipherTextLen += len;
    cipher.resize(cipherTextLen);
    memcpy(cipher.data(), cipherText, cipherTextLen);
    apr_crypto_block_cleanup(block);
    block = NULL;
    return 0;
}

int Crypto::decrypt(const std::vector<unsigned char> &cipher, std::string &plain) const {
    apr_status_t rv = 0;
    unsigned char *plainText = NULL;
    apr_size_t plainTextLen = 0;
    apr_crypto_block_t *block = NULL;
    apr_size_t blockSize = 0;
    apr_size_t len = 0;
    unsigned char *iv = (unsigned char *)m_iv.c_str();
    rv = apr_crypto_block_decrypt_init(&block, &blockSize, iv, m_key, m_pool);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to init decrypt cipher, err: %s", getError().c_str());
        return -1;
    }
    if(!block || rv) {
        TSError(MODULE.c_str(), "failed to init decrypt cipher, block or rv is invalid.");
        return -1;
    }
    rv = apr_crypto_block_decrypt(&plainText, &plainTextLen, cipher.data(), cipher.size(), block);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to decrypt, err: %s", getError().c_str());
        return -1;
    }
    rv = apr_crypto_block_decrypt_finish(plainText+plainTextLen, &len, block);
    if (rv != APR_SUCCESS) {
        TSError(MODULE.c_str(), "failed to finish decrypt, err: %s", getError().c_str());
        return -1;
    }
    plainTextLen += len;
    plain.resize(plainTextLen+1);
    memcpy((char *)&plain[0], plainText, plainTextLen);
    plain[plain.length()] = 0;
    apr_crypto_block_cleanup(block);
    block = NULL;
    return 0;
}

std::string Crypto::getError() const {
    const apu_err_t *result = NULL;
    apr_crypto_error(&result, m_ctx);
    return std::string(result->reason ? result->reason:result->msg ? result->msg : "");
}
