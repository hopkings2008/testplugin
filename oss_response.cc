#include <oss_response.h>

OssResponse::OssResponse(atscppapi::Transaction &txn, atscppapi::HttpStatus status, const std::string &reason, const std::string &body):atscppapi::TransactionPlugin(txn), m_status(status), m_reason(reason), m_body(body){
	atscppapi::TransactionPlugin::registerHook(HOOK_READ_RESPONSE_HEADERS);
	atscppapi::TransactionPlugin::registerHook(HOOK_SEND_RESPONSE_HEADERS);
	if(m_status >= atscppapi:: HTTP_STATUS_BAD_REQUEST) 
		txn.error(m_body);
}

OssResponse::~OssResponse() {
}

void OssResponse::handleReadResponseHeaders(atscppapi::Transaction &txn) {
	atscppapi::Response &resp = txn.getServerResponse();
	m_status = resp.getStatusCode();
	//reset the body here.
	if (m_status < atscppapi::HTTP_STATUS_MULTIPLE_CHOICES && m_status >= atscppapi::HTTP_STATUS_OK) {
	}
	txn.resume();
}

void OssResponse::handleSendResponseHeaders(atscppapi::Transaction &txn) {
	//txn.getClientResponse().setStatusCode(m_status);
	//txn.getClientResponse().setReasonPhrase(m_reason);
	txn.resume();
}
