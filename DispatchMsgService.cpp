#include "DispatchMsgService.h"

#include <algorithm>

DispatchMsgService* DispatchMsgService::DMS_ = nullptr;

DispatchMsgService::DispatchMsgService() {
	this->tp = NULL;
}

DispatchMsgService::~DispatchMsgService() {

}

BOOL DispatchMsgService::open() {
	this->svr_exit_ = FALSE;
	this->tp = thread_pool_init();

	return tp ? TRUE : FALSE;
}

void DispatchMsgService::close() {
	//delete msg_queue_;
	this->svr_exit_ = TRUE;
	thread_pool_destroy(tp);
	this->tp = NULL;

	//this->subscribers_.clear();
}

void DispatchMsgService::svc(void* argv) {
	DispatchMsgService* dms = DispatchMsgService::getInstance();
	iEvent* ev = (iEvent*)argv;
	if (!dms->svr_exit_) {
		LOG_DEBUG("DispatchMsgService::svc ..\n");
		//iEvent* rsp = dms->process(ev);
		printf("dms->process(ev);\n");
		dms->process(ev);
		//ɾ���¼�������Ҫ����
		delete ev;
	}
}

DispatchMsgService* DispatchMsgService::getInstance() {
	if (DMS_ == nullptr) {
		DMS_ = new DispatchMsgService();
	}

	return DMS_;
}

i32 DispatchMsgService::enqueue(iEvent* ev) {
	if (NULL == ev) {
		return -1;
	}

	thread_task_t* task = thread_task_alloc(0);

	task->handler = DispatchMsgService::svc;
	task->ctx = ev;

	return thread_task_post(tp, task);
}

void DispatchMsgService::subscribe(u32 eid, iEventHandler* handler) {
	LOG_DEBUG("DispatchMsgService::subscribe eid: %u\n", eid);
	T_EventHandlersMap::iterator iter = subscribers_.find(eid); 
	if (iter != subscribers_.end()) { //�ҵ� eid
		T_EventHandlers::iterator hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler); //�ٲ�(�ж�)��� id(�¼�id) �Ƿ�󶨵��� event_handler(ͬһ���¼�������) 
		if (hdl_iter == iter->second.end()) { //����ͬһ�� event_handler(�¼�������)
			iter->second.push_back(handler); //k(subscribers_.find(eid);) : v(handler)
		} //һ���¼����Ա��������������	
	}
	else { //�Ҳ��� eid
		subscribers_[eid].push_back(handler); //k : v (��ʼ��)
	}
}

void DispatchMsgService::unsubscribe(u32 eid, iEventHandler* handler) {
	T_EventHandlersMap::iterator iter = subscribers_.find(eid); //���� k(�¼�id)
	if (iter != subscribers_.end()) { //�ҵ�
		T_EventHandlers::iterator hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler); //����v(�¼������� event_handler)
		if (hdl_iter != iter->second.end()) { //�ҵ�
			iter->second.erase(hdl_iter);
		}
	}
}

//iEvent* DispatchMsgService::process(const iEvent* ev) {
void DispatchMsgService::process(const iEvent* ev) {
	LOG_DEBUG("DispatchMsgService::process -ev: %p\n", ev);
	if (NULL == ev) {
		return ;
	}

	u32 eid = ev->get_eid();
	LOG_DEBUG("DispatchMsgService::process - eid: %u\n", eid);
	if (eid == EEVENTID_UNKOWN) {
		LOG_WARN("DispatchMsgService : unknow evend id %d", eid);
		return ;
	}

	//LOG_DEBUG("dispatch ev : %d\n", ev->get_eid());

	T_EventHandlersMap::iterator handlers = subscribers_.find(eid);
	if (handlers == subscribers_.end()) { //�Ҳ��� eid(�¼�id)k
		LOG_WARN("DispatchMsgService : no any event handler subscribed %d\n", eid);
		return ;
	}

	//�ҵ� eid (�¼�id)
	iEvent* rsp = NULL;
	for (auto iter = handlers->second.begin(); iter != handlers->second.end(); iter++) { //���� event_handler (�¼�������)
		iEventHandler* handler = *iter;// �ҵ� v (һ���¼����Ա��������������)
		LOG_DEBUG("DispatchMsgService : get handler: %s\n", handler->get_name().c_str());
		rsp = handler->handler(ev); //��̬
	}
	
	//return rsp;
}

