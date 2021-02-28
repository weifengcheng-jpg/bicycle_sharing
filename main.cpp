#include <cstdio>
#include <iostream>

#include "bike.pb.h"
#include "event.h"
#include "events_def.h"
#include "user_event_handler.h"

int main()
{
    printf("hello from bicycle_sharing_demo01!\n");

	tutorial::mobile_request msg;
	msg.set_mobile("18219491350");

	std::cout << msg.mobile() << std::endl;

	std::cout << "kaishi==========" << std::endl;


	iEvent* ie = new iEvent(EEVENTID_GET_MOBLIE_CODE_REQ, 2);
	std::cout << ie->generateSeqNo() << std::endl;

	MobileCodeReqEv me("18219491350");
	me.dump(std::cout);

	MobileCodeRspEv mcre(ERRC_SUCCESS, 666999);
	mcre.dump(std::cout);

	MobileCodeRspEv mcre02(ERRC_BIKE_IS_TOOK, 666999);
	mcre02.dump(std::cout);

	MobileCodeRspEv mcre03(0000, 666999);
	mcre03.dump(std::cout);

	std::cout << "Gets the CAPTCHA processing implementation=============demo01" << std::endl;
	UserEventHandler ueh1;
	ueh1.handler(&me);
	

	std::cout << "jieshu----------" << std::endl;


    return 0;
}