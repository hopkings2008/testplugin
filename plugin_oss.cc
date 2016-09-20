#include <plugin_factory.h>
#include <plugin_oss.h>

PluginOss::PluginOss(atscppapi::Transaction &txn, const OSS_PARAM &param): atscppapi::TransactionPlugin(txn), m_bucket(param.bucket),
	m_ak(param.ak), m_sk(param.sk) {
		registerHook(HOOK_SEND_REQUEST_HEADERS);
}

PluginOss::~PluginOss() {
}

void PluginOss::handleSendRequestHeaders(atscppapi::Transaction &txn) {
	txn.resume();
}


PluginOssFactory::PluginOssFactory(const OSS_PARAM &param): m_bucket(param.bucket), m_ak(param.ak), m_sk(param.sk)
{
}

PluginOssFactory::~PluginOssFactory() {
}

atscppapi::TransactionPlugin *PluginOssFactory::create(atscppapi::Transaction &txn) {
	OSS_PARAM param;
	param.bucket = m_bucket;
	param.ak = m_ak;
	param.sk = m_sk;
	return new PluginOss(txn, param);
}
