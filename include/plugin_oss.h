#ifndef PLUGIN_OSS_H
#define PLUGIN_OSS_H

#include <string>
#include <atscppapi/TransactionPlugin.h>
#include <plugin_factory.h>

typedef struct _OSS_PARAM{
	std::string bucket;
	std::string ak;
	std::string sk;
}OSS_PARAM;

class PluginOss: public atscppapi::TransactionPlugin
{
	public:
		PluginOss(atscppapi::Transaction &txn, const OSS_PARAM &param);
		virtual ~PluginOss();

		virtual void handleSendRequestHeaders(atscppapi::Transaction &txn);

	protected:
		std::string m_bucket;
		std::string m_ak;
		std::string m_sk;
};

class PluginOssFactory : public PluginFactory {
	public:
		PluginOssFactory(const OSS_PARAM &param);
		virtual ~PluginOssFactory();
		virtual atscppapi::TransactionPlugin* create(atscppapi::Transaction &txn);

	protected:
		std::string m_bucket;
		std::string m_ak;
		std::string m_sk;
};

#endif
