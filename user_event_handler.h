#ifndef BRKS_BUS_USERM_HANDLER_H_
#define BRKS_BUS_USERM_HANDLER_H_

#include "glo_def.h"
#include "iEventHandler.h"
#include "events_def.h"

#include <string>
#include <map>
#include <memory>

class UserEventHandler : public iEventHandler {
public:
	UserEventHandler();
	virtual ~UserEventHandler();
	
	virtual iEvent* handler(const iEvent* ev);
private:
	MobileCodeRspEv* handle_mobile_code_req(MobileCodeReqEv* ev);
	i32 code_gen();
private:
	std::string mobile_;
	std::map<std::string, i32> m2c_; //first is mobile, second is code
};

#endif // !BRKS_BUS_USERM_HANDLER_H_
