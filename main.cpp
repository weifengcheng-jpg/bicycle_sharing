#include <cstdio>
#include <iostream>
#include <unistd.h>

#include "bike.pb.h"
#include "event.h"
#include "events_def.h"
#include "user_event_handler.h"
#include "DispatchMsgService.h"

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

	UserEventHandler ueh1;
	//ueh1.handler(&me);
	
	DispatchMsgService* DMS = DispatchMsgService::getInstance();
	DMS->open();
	
	MobileCodeReqEv* pmcre = new MobileCodeReqEv("18219491350");
	DMS->enqueue(pmcre);

	//ueh1.handler(&mcre03);
	

	sleep(5);
	DMS->close();
	sleep(5);

	std::cout << "jueshu==========" << std::endl;


    return 0;
}