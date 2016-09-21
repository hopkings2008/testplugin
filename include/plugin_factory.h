#ifndef PLUGIN_FACTORY_H
#define PLUGIN_FACTORY_H

#include <string>
#include <atscppapi/Transaction.h>
#include <atscppapi/TransactionPlugin.h>

class PluginFactory {
	public:
		PluginFactory(){};
		virtual ~PluginFactory(){};
		virtual atscppapi::TransactionPlugin* create(const std::string &root, const std::string &path, atscppapi::Transaction &txn) = 0;
};

#endif
