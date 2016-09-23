#include <plugin_factory.h>
#include <plugin_oss.h>
#include <atscppapi/shared_ptr.h>
#include <auth_user.h>
#include <oss_put.h>
#include <oss_get.h>
#include <log.h>

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
	std::string md5 = hdrs.values("Content-MD5");
	if(md5.length() == 0) {
		return "";
	}
	return md5;
}

std::string PluginOss::getClientContentType(atscppapi::ClientRequest &req) {
	atscppapi::Headers &hdrs = req.getHeaders();
	std::string type = hdrs.values("Content-Type");
	if(type.length() == 0){
		return "";
	}
	return type;
}

std::string PluginOss::getClientDate(atscppapi::ClientRequest &req) {
	atscppapi::Headers &hdrs = req.getHeaders();
	return hdrs.values("Date");
}

std::string PluginOss::pathEncrypt(const Crypto &crypto, const Base64 &base64, const std::string &oss_path){
    return oss_path;
    std::vector<unsigned char> cipher;
    int ret = crypto.encrypt(oss_path, cipher);
    if (ret != 0) {
        TSError(MODULE.c_str(), "failed to encrypt path: %s", oss_path.c_str()) ;
        return "";
    }
    return base64.encode(cipher);
}

std::string PluginOss::pathDecrypt(const Crypto &crypto, const Base64 &base64, const std::string &oss_path) {
    return oss_path;
    std::vector<unsigned char> cipher = base64.decode(oss_path);
    std::string plain;
    if(cipher.size() == 0) {
        LOG_ERROR(log, "base64 decode failed for invalid oss_path: %s", oss_path.c_str());
        return "";
    }
    int ret = crypto.decrypt(cipher, plain);
    if (ret != 0) {
        TSError(MODULE.c_str(), "failed to decrypt for invalid oss_path: %s", oss_path.c_str());
        return "";
    }
    return plain;
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
	if (root == "v2/img/add_image") {
		return new OssPut(txn, param);
	} else if (root == "v2/img/get_image") {
		return new OssGet(txn, param);
	}
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
