#include <sstream>
#include <oss_put.h>
#include <oss_auth.h>
#include <oss_response.h>
#include <log.h>

extern Crypto g_crypto;

OssPut::OssPut(atscppapi::Transaction &txn, const OSS_PARAM &param): PluginOss(txn, param) {
}

OssPut::~OssPut() {
}

void OssPut::handleSendRequestHeaders(atscppapi::Transaction &txn) {
	if(txn.isInternalRequest()){
		txn.resume();
		return;
	}
	//get client req.
	atscppapi::ClientRequest &req = txn.getClientRequest();
	//get method.
	std::string method = getClientMethod(req);
	std::string contentMd5 = getClientContentMd5(req);
	std::string contentType = getClientContentType(req);
	std::string date = getClientDate(req);
	std::string hdrs = "";
	BucketInfo bucket(m_bucket, m_ak, m_sk);
	UserInfo user(getUid(txn));
	std::ostringstream  path;
	path << "/" << user.uid << "/" << m_obj;
	std::string resource(path.str());
	AuthHeader authHdr(method, contentMd5, contentType, date, hdrs, resource);
	OssAuth auth(bucket, user);
	std::string authStr = auth.getAuth(authHdr);
	atscppapi::Request &reqSrv = txn.getServerRequest();
	atscppapi::Headers &hdrsSrv = reqSrv.getHeaders();
	atscppapi::Url &urlSrv = reqSrv.getUrl();

	std::ostringstream newpath;
	std::string oldPath = urlSrv.getPath();
	std::string root;
	size_t end = oldPath.find(m_obj);
	root = oldPath.substr(0, end);
	newpath << root << user.uid << "/" << m_obj;
	urlSrv.setPath(newpath.str());

	hdrsSrv.set("Authorization", authStr.c_str());
	//encrypt return url here. and hook response.
	Base64 base64;
	std::string resourceEnc = pathEncrypt(g_crypto, base64, resource);
	if(resourceEnc.length() == 0) {
		TSError(MODULE.c_str(), "failed to encrypt path %s", resource.c_str());
		OssResponse *resp = new OssResponse(OSS_GET, txn, "invalid path.");
		resp->setStatus(atscppapi::HTTP_STATUS_BAD_REQUEST);
		resp->setReasonPhase("Bad Request");
		txn.addPlugin(resp);
		txn.error("failed to encrypt path");
		return;
	}
	std::string respBody = std::string("{\"url_path\": \"") + resourceEnc + std::string("\"}");
	txn.addPlugin(new OssResponse(OSS_PUT, txn, respBody));
	txn.resume();
}
