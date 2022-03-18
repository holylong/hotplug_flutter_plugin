#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <vector>

namespace hotplug{

  void split(const std::string& s, std::vector<std::string>& tokens, const char& delim);

}

#endif //_UTIL_H_