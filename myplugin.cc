#include <myplugin.h>
#include <plugin_oss.h>
#include <log.h>
#include <crypto.h>

StoragePlugin::StoragePlugin() {
	registerHook(atscppapi::Plugin::HOOK_READ_REQUEST_HEADERS_PRE_REMAP);
}

StoragePlugin::~StoragePlugin() {
}

int StoragePlugin::init() {
	OSS_PARAM param = {
		"bucket",
		"ak",
		"sk",
	};
	m_router.addRoute("v2/img", new PluginOssFactory(param));
	return 0;
}

void StoragePlugin::handleReadRequestHeadersPreRemap(atscppapi::Transaction &txn) {
	atscppapi::Request &req = txn.getClientRequest();
	atscppapi::Url &url = req.getUrl();
	std::string path = url.getPath();
	if(path.length() == 0){
		txn.resume();
		return;
	}
	PluginFactory *factory = NULL;
	std::string root;
       	if (0 != m_router.match(path, root, factory)){
		txn.resume();
		return;
	}
	txn.addPlugin(factory->create(root, path, txn));
	txn.resume();
}

void TSPluginInit(int argc ATSCPPAPI_UNUSED, const char *argv[] ATSCPPAPI_UNUSED) {
    log.init("StoragePlugin", true, true, atscppapi::Logger::LOG_LEVEL_DEBUG, true, 300);
    log.setRollingEnabled(true);
    log.setRollingIntervalSeconds(300);
    g_crypto.init("#!shiqichuban123", "@salt", "!@shiqichuban456");
	StoragePlugin *plugin = new StoragePlugin();
	plugin->init();
}
