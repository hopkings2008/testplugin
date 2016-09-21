#include <sstream>
#include <oss_put.h>
#include <oss_auth.h>
#include <oss_response.h>

OssPut::OssPut(atscppapi::Transaction &txn, const OSS_PARAM &param): PluginOss(txn, param) {
}

OssPut::~OssPut() {
}

void OssPut::handleSendRequestHeaders(atscppapi::Transaction &txn) {
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
	path << m_bucket << "/" << user.uid << "/" << m_obj;
	std::string resource(path.str());
	AuthHeader authHdr(method, contentMd5, contentType, date, hdrs, resource);
	OssAuth auth(bucket, user);
	std::string authStr = auth.getAuth(authHdr);
	atscppapi::Request &reqSrv = txn.getServerRequest();
	atscppapi::Headers &hdrsSrv = reqSrv.getHeaders();
	atscppapi::Url &urlSrv = reqSrv.getUrl();
	urlSrv.setPath(resource);
	hdrsSrv.set("Authorization", authStr);
	//encrypt return url here. and hook response.
    Base64 base64;
    std::string resourceEnc = pathEncrypt(g_crypto, base64, path.str());
    if(resourceEnc.length() == 0) {
        txn.error("failed to encrypt path");
        return;
    }
    std::string respBody = std::string("{ \"url_path\": \"") + resourceEnc + std::string("\"}");
    txn.addPlugin(new OssResponse(txn, respBody));
	txn.resume();
}
