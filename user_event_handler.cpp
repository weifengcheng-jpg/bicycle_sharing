#include "user_event_handler.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>

UserEventHandler::UserEventHandler() {
	//需要实现订阅时间的处理
	thread_mutex_create(&pm_);
}

UserEventHandler::~UserEventHandler() {
	//需要实现退订时间的处理
	thread_mutex_destroy(&pm_);
}

iEvent* UserEventHandler::handler(const iEvent* ev) {
	if (ev == NULL) {
		//LOG_ERROR("input ev is NULL");
		printf("input ev is NULL");
	}

	u32 eid = ev->get_eid();

	if (eid == EEVENTID_GET_MOBLIE_CODE_REQ) {
		return handle_mobile_code_req((MobileCodeReqEv*)ev);
	}
	else if (eid == EEVENTID_LOGIN_REQ) {
		//
	}
	else if (eid == EEVENTID_RECHARGE_REQ) {
		//
	}
	else if (eid == EEVENTID_GET_ACCOUNT_BALANCE_REQ) {
		//
	}
	else if (eid == EEVENTID_LIST_ACCOUNT_RECORDS_REQ) {
		//
	}
	else if (eid == EEVENTID_LIST_TRAVELS_REQ) {
		//16;50
	}

	return NULL;
}

MobileCodeRspEv* UserEventHandler::handle_mobile_code_req(MobileCodeReqEv* ev) {
	i32 icode = 0;
	std::string mobile_ = ev->get_mobile();
	//LOG_DEBUG("try to get moblie phone %s validate code.", mobile_.c_str());
	printf("try to get moblie phone %s validate code.\n", mobile_.c_str());

	icode = code_gen();
	thread_mutex_lock(&pm_);
	m2c_[mobile_] = icode;
	thread_mutex_destroy(&pm_);
	//LOG_DEBUG("send sms success");
	printf("mobile: %s, code: %d\n", mobile_.c_str(), icode);
	return new MobileCodeRspEv(200, icode);
}

i32 UserEventHandler::code_gen() {
	i32 code = 0;
	srand((unsigned int)time(NULL));

	code = (unsigned int)(rand() % (999999 - 100000) + 1000000);

	return code;
}