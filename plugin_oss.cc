#include <plugin_factory.h>
#include <plugin_oss.h>
#include <atscppapi/shared_ptr.h>
#include <auth_user.h>

PluginOss::PluginOss(atscppapi::Transaction &txn, const OSS_PARAM &param): atscppapi::TransactionPlugin(txn), m_obj(param.obj), m_bucket(param.bucket),
	m_ak(param.ak), m_sk(param.sk) {
		registerHook(HOOK_SEND_REQUEST_HEADERS);
}

PluginOss::~PluginOss() {
}

void PluginOss::handleSendRequestHeaders(atscppapi::Transaction &txn) {
	txn.resume();
}

int PluginOss::getUid(atscppapi::Transaction &txn) {
	atscppapi::shared_ptr<atscppapi::Transaction::ContextValue> ctx = txn.getContextValue("user.uid");
	shiqi::auth::AuthContext *authCtx = dynamic_cast<shiqi::auth::AuthContext *>(ctx.get());
	if (authCtx) {
		return authCtx->uid();
	}
	return -1;
}

std::string PluginOss::getClientMethod(atscppapi::ClientRequest &req) {
	return  atscppapi::HTTP_METHOD_STRINGS[req.getMethod()];
}

std::string PluginOss::getClientContentMd5(atscppapi::ClientRequest &req) {
	atscppapi::Headers &hdrs = req.getHeaders();
	return hdrs.values("Content-MD5");
}

std::string PluginOss::getClientContentType(atscppapi::ClientRequest &req) {
	atscppapi::Headers &hdrs = req.getHeaders();
	return hdrs.values("Content-Type");
}

std::string PluginOss::getClientDate(atscppapi::ClientRequest &req) {
	atscppapi::Headers &hdrs = req.getHeaders();
	return hdrs.values("Date");
}

PluginOssFactory::PluginOssFactory(const OSS_PARAM &param): m_bucket(param.bucket), m_ak(param.ak), m_sk(param.sk)
{
}

PluginOssFactory::~PluginOssFactory() {
}

atscppapi::TransactionPlugin *PluginOssFactory::create(const std::string &root, const std::string &path, atscppapi::Transaction &txn) {
	OSS_PARAM param;
	param.obj = getObjName(root, path);
	param.bucket = m_bucket;
	param.ak = m_ak;
	param.sk = m_sk;
	return new PluginOss(txn, param);
}

std::string PluginOssFactory::getObjName(const std::string &root, const std::string &path) {
	std::string prefix = root;
	if (prefix[prefix.length()-1] != '/'){
		prefix += "/";
	}
	if (prefix.length() >= path.length()) {
		return "";
	}
	return path.substr(prefix.length());
}
