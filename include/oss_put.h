#ifndef OSS_PUT_H
#define OSS_PUT_H
#include <plugin_oss.h>

class OssPut: public PluginOss {
	public:
		OssPut(atscppapi::Transaction &txn, const OSS_PARAM &param);
		virtual ~OssPut();
		virtual void handleSendRequestHeaders(atscppapi::Transaction &txn);
};

#endif
