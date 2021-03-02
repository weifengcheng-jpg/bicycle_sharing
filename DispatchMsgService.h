/*
���ļ���
����ַ���Ϣ����ģ��, ��ʵ���ǰ��ⲿ�յ�����Ϣ, ת�����ڲ��¼�, Ҳ���� data->msg->event �Ľ������,
Ȼ���ٰ��¼�Ͷ�����̳߳ص���Ϣ����, ���̳߳ص����� process �������¼����д���, ���յ���ÿ�� event �� handler �ķ���
������ event, ��ʱÿ�� event handler ��Ҫ subscribe �� event ��Żᱻ���õ�.

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

	//���¼�Ͷ�ݵ��̳߳ش���
	virtual i32 enqueue(iEvent* ev);

	//�Ծ�����¼����зַ�����
	//virtual iEvent* process(const iEvent* ev);
	virtual void process(const iEvent* ev);

	//�̳߳ػص�����
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
