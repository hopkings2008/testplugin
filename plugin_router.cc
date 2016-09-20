#include <plugin_router.h>

PluginRouter::PluginRouter(const int nodeNum): m_router(r3_tree_create(nodeNum)) {
}

PluginRouter::~PluginRouter() {
	if(m_router)
		r3_tree_free(m_router);
}

void PluginRouter::addRoute(const std::string &path, PluginFactory *factory) {
	r3_tree_insert_path(m_router, path.c_str(), factory);
}

int PluginRouter::create() {
	char *errstr = NULL;
	int ret = r3_tree_compile(m_router, &errstr);
	if(ret != 0) 
		free(errstr);
	return ret;
}

PluginFactory *PluginRouter::match(const std::string &path) {
	R3Node *matched_node = r3_tree_matchl(m_router, path.c_str(), path.length(), NULL);
	if(matched_node)
		return static_cast<PluginFactory *>(matched_node->data);
	return NULL;
}
