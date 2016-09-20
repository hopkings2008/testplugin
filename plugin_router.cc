#include <plugin_router.h>

PluginRouter::PluginRouter(const int nodeNum) {
}

PluginRouter::~PluginRouter() {
	radix_tree<std::string, PluginFactory *>::iterator itr = m_router.begin();
	for(; itr != m_router.end(); itr++) {
		delete itr->second;
	}
}

void PluginRouter::addRoute(const std::string &path, PluginFactory *factory) {
	m_router[path] = factory;
}

PluginFactory *PluginRouter::match(const std::string &path) {
	radix_tree<std::string, PluginFactory *>::iterator itr = m_router.longest_match(path);
	if(itr != m_router.end())
		return itr->second;
	return NULL;
}
