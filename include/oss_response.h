#ifndef OSS_RESPONSE_H
#define OSS_RESPONSE_H

#include <string>
#include <atscppapi/TransactionPlugin.h>

enum OssMethod {
	OSS_UNKNOWN = 0,
	OSS_GET = 1,
	OSS_PUT = 2,
};

class OssResponse: public atscppapi::TransactionPlugin {
	public:
		OssResponse(const OssMethod &method, atscppapi::Transaction &txn, const std::string &body);
		virtual ~OssResponse(); 
		virtual void handleSendResponseHeaders(atscppapi::Transaction &txn);
		virtual void handleReadResponseHeaders(atscppapi::Transaction &txn);
		void setStatus(const atscppapi::HttpStatus &status);
		void setReasonPhase(const std::string &reason);
	protected:
		OssMethod m_method;
		atscppapi::HttpStatus m_status;
		std::string m_reason;
		std::string m_body;
};
#endif
