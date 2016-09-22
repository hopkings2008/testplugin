#include <oss_get.h>
#include <crypto.h>
#include <base64.h>
#include <oss_auth.h>
#include <oss_response.h>
#include <log.h>

extern Crypto g_crypto;

OssGet::OssGet(atscppapi::Transaction &txn, const OSS_PARAM &param):PluginOss(txn, param) {
}

OssGet::~OssGet() {
}

void OssGet::handleSendRequestHeaders(atscppapi::Transaction &txn) {
	if(txn.isInternalRequest()) {
		txn.resume();
		return;
	}
	std::string realPath;
	Base64 base64;
	std::vector<unsigned char> cipher = base64.decode(m_obj);
	if (cipher.size() == 0) {
		TSError(MODULE.c_str(),"the path %s is invalid.", m_obj.c_str());
		OssResponse *resp = new OssResponse(OSS_GET, txn, "invaid request.");
		resp->setStatus(atscppapi::HTTP_STATUS_BAD_REQUEST);
		resp->setReasonPhase("Bad Request");
		txn.addPlugin(resp);
		txn.error();
		return;
	}
	int ret = g_crypto.decrypt(cipher, realPath);
	if (ret != 0) {
		TSError(MODULE.c_str(), "the path %s is invalid, cannot decrypt it", m_obj.c_str());
		OssResponse *resp = new OssResponse(OSS_GET, txn, "invaid request.");
		resp->setStatus(atscppapi::HTTP_STATUS_BAD_REQUEST);
		resp->setReasonPhase("Bad Request");
		txn.addPlugin(resp);
		txn.error();
		return;
	}
	TSDebug(MODULE.c_str(), "got path: %s", realPath.c_str());
	//set real path.
	atscppapi::Request &reqSrv = txn.getServerRequest();
	atscppapi::Headers &hdrsSrv = reqSrv.getHeaders();
	atscppapi::Url &urlSrv = reqSrv.getUrl();
	urlSrv.setPath(realPath);
	//set authorization header.
	atscppapi::ClientRequest &req = txn.getClientRequest();
	std::string method = getClientMethod(req);
	std::string contentMd5 = getClientContentMd5(req);
	std::string contentType = getClientContentType(req);
	std::string date = getClientDate(req);
	std::string hdrs = "";
	BucketInfo bucket(m_bucket, m_ak, m_sk);
	UserInfo user(getUid(txn));
	AuthHeader authHdr(method, contentMd5, contentType, date, hdrs, realPath);
	OssAuth auth(bucket, user);
	std::string authStr = auth.getAuth(authHdr);
	hdrsSrv.set("Authorization", authStr);
	txn.resume();
}

