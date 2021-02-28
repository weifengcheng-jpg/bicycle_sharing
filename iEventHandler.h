#ifndef NS_IEVENT_HANDLER_H_
#define NS_IEVENT_HANDLER_H_

#include "event.h"
#include "eventtype.h"

class iEventHandler {
public:
	iEventHandler() {}
	virtual ~iEventHandler() {}

	virtual iEvent* handler(const iEvent* ev) { return NULL; }
};

#endif // !NS_IEVENT_HANDLER_H_

