#ifndef BRKS_COMMON_EVENTS_DEF_H_
#define BRKS_COMMON_EVENTS_DEF_H_

#include "event.h"
#include <string>
#include "bike.pb.h"

class MobileCodeReqEv : public iEvent {
public:
	MobileCodeReqEv(const std::string& mobile) : iEvent(EEVENTID_GET_MOBLIE_CODE_REQ, iEvent::generateSeqNo()) {
		this->msg_.set_mobile(mobile);
		
	}

	const std::string& get_mobile() { return msg_.mobile(); }

	virtual std::ostream& dump(std::ostream& out) const;

private:
	tutorial::mobile_request msg_;
};

class MobileCodeRspEv : public iEvent {
public:
	MobileCodeRspEv(i32 code, i32 icode) : iEvent(EEVENTID_GET_MOBLIE_CODE_RSP, iEvent::generateSeqNo()) {
		this->msg_.set_code(code);
		this->msg_.set_icode(icode);
		this->msg_.set_data(getReasonByErrorCode(code));
	}

	const i32 get_code() { return msg_.code(); }
	const i32 get_icode() { return msg_.icode(); }
	const std::string& get_data() { return msg_.data(); }

	virtual std::ostream& dump(std::ostream& out) const;

private:
	//std::string mobile_;
	tutorial::mobile_response msg_;
};

#endif // !BRKS_COMMON_EVENTS_DEF_H_