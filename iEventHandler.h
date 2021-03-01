#ifndef NS_IEVENT_HANDLER_H_
#define NS_IEVENT_HANDLER_H_

#include "event.h"
#include "eventtype.h"

class iEventHandler {
public:
	iEventHandler(const char* name) : name_(name) {}
	virtual ~iEventHandler() {}

	virtual iEvent* handler(const iEvent* ev) { return NULL; }
	std::string& get_name() { return name_; }
private:
	std::string name_;
};

#endif // !NS_IEVENT_HANDLER_H_

