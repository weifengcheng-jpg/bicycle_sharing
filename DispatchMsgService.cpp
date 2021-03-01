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
		//删除事件这里需要斟酌
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
	if (iter != subscribers_.end()) { //找到 eid
		T_EventHandlers::iterator hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler); //再查(判断)这个 id(事件id) 是否绑定的是 event_handler(同一个事件处理器) 
		if (hdl_iter == iter->second.end()) { //不是同一个 event_handler(事件处理器)
			iter->second.push_back(handler); //k(subscribers_.find(eid);) : v(handler)
		} //一个事件可以被多个处理器处理	
	}
	else { //找不到 eid
		subscribers_[eid].push_back(handler); //k : v (初始化)
	}
}

void DispatchMsgService::unsubscribe(u32 eid, iEventHandler* handler) {
	T_EventHandlersMap::iterator iter = subscribers_.find(eid); //查找 k(事件id)
	if (iter != subscribers_.end()) { //找到
		T_EventHandlers::iterator hdl_iter = std::find(iter->second.begin(), iter->second.end(), handler); //查找v(事件处理器 event_handler)
		if (hdl_iter != iter->second.end()) { //找到
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
	if (handlers == subscribers_.end()) { //找不到 eid(事件id)k
		LOG_WARN("DispatchMsgService : no any event handler subscribed %d\n", eid);
		return ;
	}

	//找到 eid (事件id)
	iEvent* rsp = NULL;
	for (auto iter = handlers->second.begin(); iter != handlers->second.end(); iter++) { //遍历 event_handler (事件处理器)
		iEventHandler* handler = *iter;// 找到 v (一个事件可以被多个处理器处理)
		LOG_DEBUG("DispatchMsgService : get handler: %s\n", handler->get_name().c_str());
		rsp = handler->handler(ev); //多态
	}
	
	//return rsp;
}

