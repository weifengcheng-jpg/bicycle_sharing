/*
中文简体
负责分发消息服务模块, 其实就是把外部收到的消息, 转化成内部事件, 也就是 data->msg->event 的解码过程,
然后再把事件投递至线程池的消息队列, 由线程池调用其 process 方法对事件进行处理, 最终调用每个 event 的 handler 的方法
来处理 event, 此时每个 event handler 需要 subscribe 该 event 后才会被调用到.

English
Responsible for the distribution of message service module, which is to convert external received messages into internal events, that is, the decoding process of data-> msg-bbb>ents.
The event is then posted to the message queue of the thread pool, and the thread pool calls its process method to process the event, and finally calls the handler method of each event
Each event handler needs to subscribe the event before it can be called.

3:02
*/

#ifndef BRK_SERVICE_DISPATCH_EVENT_SERVICE_H_
#define BRK_SERVICE_DISPATCH_EVENT_SERVICE_H_

#include <map>
#include <vector>

#include "ievent.h"
#include "eventtype.h"
#include "iEventHandler.h"
#include "threadpool/thread_pool.h"

class DispatchMsgService {
public:
	DispatchMsgService();
	virtual ~DispatchMsgService();

	virtual BOOL open();
	virtual void close();

	virtual void subscribe(u32 eid, iEventHandler* handler);
	virtual void unsubscribe(u32 eid, iEventHandler* handler);

	//把事件投递到线程池处理
	virtual i32 enqueue(iEvent* ev);

	//对具体的事件进行分发处理
	//virtual iEvent* process(const iEvent* ev);
	virtual void process(const iEvent* ev);

	//线程池回调函数
	static void svc(void* argv);
	static DispatchMsgService* getInstance();

protected:
	thread_pool_t* tp;
	static DispatchMsgService* DMS_;

	typedef std::vector<iEventHandler*> T_EventHandlers;
	typedef std::map<u32, T_EventHandlers> T_EventHandlersMap;
	T_EventHandlersMap subscribers_;

	bool svr_exit_;
};



#endif // !BRK_SERVICE_DISPATCH_EVENT_SERVICE_H_
