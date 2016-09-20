#ifndef PLUGIN_ROUTER_H
#define PLUGIN_ROUTER_H

#include <string>
#include <r3/r3.h>
#include <plugin_factory.h>

class PluginRouter {
	public:
		PluginRouter(const int nodeNum = 10);
		virtual ~PluginRouter();
		void addRoute(const std::string &path, PluginFactory *factory);
		int create();
		PluginFactory *match(const std::string &path);

	protected:
		R3Node *m_router;
};

#endif
