#ifndef PLUGIN_OSS_H
#define PLUGIN_OSS_H

#include <string>
#include <atscppapi/TransactionPlugin.h>
#include <plugin_factory.h>

typedef struct _OSS_PARAM{
	std::string obj;
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
		int getUid(atscppapi::Transaction &txn);
		std::string getClientMethod(atscppapi::ClientRequest &req);
		std::string getClientContentMd5(atscppapi::ClientRequest &req);
		std::string getClientContentType(atscppapi::ClientRequest &req);
		std::string getClientDate(atscppapi::ClientRequest &req);

	protected:
		std::string m_obj;
		std::string m_bucket;
		std::string m_ak;
		std::string m_sk;
};

class PluginOssFactory : public PluginFactory {
	public:
		PluginOssFactory(const OSS_PARAM &param);
		virtual ~PluginOssFactory();
		virtual atscppapi::TransactionPlugin* create(const std::string &root, const std::string &path, atscppapi::Transaction &txn);

	protected:
		std::string getObjName(const std::string &root, const std::string &path);

	protected:
		std::string m_bucket;
		std::string m_ak;
		std::string m_sk;
};

#endif
