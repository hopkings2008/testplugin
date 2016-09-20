#ifndef MY_PLUGIN_H
#define MY_PLUGIN_H

#include <string>
#include <map>
#include <atscppapi/GlobalPlugin.h>
#include <atscppapi/TransactionPlugin.h>
#include <atscppapi/PluginInit.h>
#include <plugin_factory.h>
#include <plugin_router.h>

class StoragePlugin: public atscppapi::GlobalPlugin
{
	public:
		StoragePlugin();
		virtual ~StoragePlugin();
		virtual void handleReadRequestHeadersPreRemap(atscppapi::Transaction &txn);
		int init();

	protected:
		std::map<std::string, PluginFactory *> m_factory;
		PluginRouter m_router;
};	

#endif
