#ifndef OSS_RESPONSE_H
#define OSS_RESPONSE_H

#include <string>
#include <atscppapi/TransactionPlugin.h>

class OssResponse: public atscppapi::TransactionPlugin {
	public:
		OssResponse(atscppapi::Transaction &txn, atscppapi::HttpStatus status, const std::string &reason, const std::string &body);
		virtual ~OssResponse(); 
		virtual void handleSendResponseHeaders(atscppapi::Transaction &txn);
		virtual void handleReadResponseHeaders(atscppapi::Transaction &txn);
	protected:
		atscppapi::HttpStatus m_status;
		std::string m_reason;
		std::string m_body;
};
#endif
