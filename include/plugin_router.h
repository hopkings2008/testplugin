#ifndef PLUGIN_ROUTER_H
#define PLUGIN_ROUTER_H

#include <string>
#include <radix_tree/radix_tree.hpp>
#include <plugin_factory.h>

class PluginRouter {
	public:
		PluginRouter(const int nodeNum = 10);
		virtual ~PluginRouter();
		void addRoute(const std::string &path, PluginFactory *factory);
		int match(const std::string &path, std::string &root, PluginFactory *&factory);

	protected:
		radix_tree<std::string, PluginFactory *> m_router;
};

#endif
