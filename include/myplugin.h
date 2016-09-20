#ifndef MY_PLUGIN_H
#define MY_PLUGIN_H
#include <atscppapi/GlobalPlugin.h>
#include <atscppapi/TransactionPlugin.h>
#include <atscppapi/PluginInit.h>

class StoragePlugin: public atscppapi::GlobalPlugin
{
	public:
		StoragePlugin();
		virtual ~StoragePlugin();
		virtual void handleReadRequestHeadersPreRemap(atscppapi::Transaction &txn);
};	

#endif
