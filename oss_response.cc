#include <oss_response.h>

OssResponse::OssResponse(const OssMethod &method, atscppapi::Transaction &txn, const std::string &body):atscppapi::TransactionPlugin(txn), 
	m_method(method), m_status(atscppapi::HTTP_STATUS_UNKNOWN), m_reason(""), m_body(body){
		atscppapi::TransactionPlugin::registerHook(HOOK_READ_RESPONSE_HEADERS);
		atscppapi::TransactionPlugin::registerHook(HOOK_SEND_RESPONSE_HEADERS);
		if(m_status >= atscppapi:: HTTP_STATUS_BAD_REQUEST) 
			txn.error(m_body);
	}

OssResponse::~OssResponse() {
}

void OssResponse::handleReadResponseHeaders(atscppapi::Transaction &txn) {
	if(txn.isInternalRequest()) {
		txn.resume();
		return;
	}

	atscppapi::Response &resp = txn.getServerResponse();
	switch (m_method) {
		case OSS_PUT:
			m_status = resp.getStatusCode();
			m_reason = resp.getReasonPhrase();
			//reset the body here.
			if (m_status < atscppapi::HTTP_STATUS_MULTIPLE_CHOICES && m_status >= atscppapi::HTTP_STATUS_OK) {
				txn.error(m_body);
				return;
			}
			break;
		default:
			break;
	}
	txn.resume();
}

void OssResponse::handleSendResponseHeaders(atscppapi::Transaction &txn) {
	txn.getClientResponse().setStatusCode(m_status);
	txn.getClientResponse().setReasonPhrase(m_reason);
	txn.resume();
}

void OssResponse::setStatus(const atscppapi::HttpStatus &status) {
	m_status = status;
}

void OssResponse::setReasonPhase(const std::string &reason) {
	m_reason = reason;
}
