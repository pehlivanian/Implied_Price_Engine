#ifndef __TRACE_H__
#define __TRACE_H__

#include <string>
#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;

namespace Trace {

class Trace {
 public:
  Trace( std::string _msg, std::string _fn, int _ln ) : msg_(_msg),
    fn_(_fn), ln_(_ln) {
    std::cout << "Entering: " 
	 << std::setw(24) << msg_ << " | file: " 
	 << std::setw(12) << fn_  << " | " << "line: " 
	 << std::setw(5)  << ln_  << " @"  << microsec_clock::local_time() 
	 << std::endl;
    }
   Trace( std::string _msg ) : msg_(_msg) {
       std::cout << "Entering: " 
	<< std::setw(24) << msg_ << " @" << microsec_clock::local_time() << std::endl;
    }
    ~Trace() {
      std::cout << "Exiting: " 
	 << std::setw(25) << msg_ << " | file: " 
	 << std::setw(12) << fn_  << "               @" 
	 << microsec_clock::local_time() 
	 << std::endl;
    }
 private:
    std::string msg_;
    std::string fn_;
    int ln_;
};

}
#endif
