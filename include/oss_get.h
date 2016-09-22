#ifndef OSS_GET_H
#define OSS_GET_H

#include <plugin_oss.h>

class OssGet: public PluginOss {
	public:
		OssGet(atscppapi::Transaction &txn, const OSS_PARAM &param);
		virtual ~OssGet();
		virtual void handleSendRequestHeaders(atscppapi::Transaction &txn);
};

#endif
