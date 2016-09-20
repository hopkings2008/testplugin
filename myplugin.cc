#include <myplugin.h>

StoragePlugin::StoragePlugin() {
	registerHook(atscppapi::Plugin::HOOK_READ_REQUEST_HEADERS_PRE_REMAP);
}

StoragePlugin::~StoragePlugin() {
}

void StoragePlugin::handleReadRequestHeadersPreRemap(atscppapi::Transaction &txn) {
}	
