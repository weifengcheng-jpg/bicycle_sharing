#include "events_def.h"
#include <iostream>
#include <sstream>


std::ostream& MobileCodeReqEv::dump(std::ostream& out) const {
	out << "MobileCodeReqEv sn=" << get_sn() << ", ";
	out << "mobile=" << msg_.mobile() << std::endl;

	return out;
}

std::ostream& MobileCodeRspEv::dump(std::ostream& out) const {
	out << "MobileCodeRspEv sn =" << get_sn() << ", ";
	out << "code=" << msg_.code() << std::endl;
	out << "icode=" << msg_.icode() << std::endl;
	out << "describle = " << msg_.data() << std::endl;

	return out;
}