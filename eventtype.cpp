#include "eventtype.h"

static EErrorReason EER[] = {
	{ERRC_SUCCESS, "OK."},
	{ERRC_INVALID_MSG, "Invalid message."},
	{ERRC_INVALID_DATA, "Invalid data."},
	{ERRC_METHOD_NOT_ALLOWED, "Method not allowed."},
	{ERRC_PROCCESS_FAILED, "Proccess failed."},
	{ERRC_BIKE_IS_TOOK, "Bike is took."},
	{ERRC_BIKE_IS_RUNNING, "Bike is running."},
	{ERRC_BIKE_IS_DAMAGED, "Bike is damaged."},
	{ERR_NULL, "Undefined"}
};

const char* getReasonByErrorCode(i32 icode) {
	i32 i = 0;
	for (i = 0; EER[i].icode != ERR_NULL; i++) {
		if (EER[i].icode == icode) {
			return EER[i].reason;
		}
	}

	return EER[i].reason; //"Undefined"
}

