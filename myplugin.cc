#include <myplugin.h>
#include <plugin_oss.h>

StoragePlugin::StoragePlugin() {
	registerHook(atscppapi::Plugin::HOOK_READ_REQUEST_HEADERS_PRE_REMAP);
}

StoragePlugin::~StoragePlugin() {
	std::map<std::string, PluginFactory *>::iterator itr = m_factory.begin();
	for(; itr != m_factory.end(); itr++) {
		delete itr->second;
	}
}

int StoragePlugin::init() {
	OSS_PARAM param = {
		"bucket",
		"ak",
		"sk",
	};
	m_factory.insert(std::pair<std::string, PluginFactory *>("/v2/img/{id}", new PluginOssFactory(param)));
	for(std::map<std::string, PluginFactory *>::iterator itr = m_factory.begin(); itr != m_factory.end(); itr++) {
		m_router.addRoute(itr->first, itr->second);
	}
	return m_router.create();
}

void StoragePlugin::handleReadRequestHeadersPreRemap(atscppapi::Transaction &txn) {
	atscppapi::Request &req = txn.getClientRequest();
	atscppapi::Url &url = req.getUrl();
	std::string path = url.getPath();
	if(path.length() == 0){
		txn.resume();
		return;
	}
	if(path[0] != '/') {
		path = std::string("/") + path;
	}
	PluginFactory *factory = m_router.match(path);
	if(!factory) {
		txn.resume();
		return;
	}
	txn.addPlugin(factory->create(txn));
	txn.resume();
}

void TSPluginInit(int argc ATSCPPAPI_UNUSED, const char *argv[] ATSCPPAPI_UNUSED) {
	StoragePlugin *plugin = new StoragePlugin();
	plugin->init();
}
