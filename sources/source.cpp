// Copyright 2018 Your Name <your_email>
#include <thread>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "../third-party/PicoSHA2/picosha2.h"

#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup.hpp>



using std::thread;
using std::atoi;
using std::vector;
using std::cout;


void init() {
	boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");

	auto log = boost::log::add_file_log(
		boost::log::keywords::file_name = "../logs/loggerFile_%N.log",
		boost::log::keywords::rotation_size = 10 * 1024 * 1024,
		boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point{ 0, 0, 0 },
		boost::log::keywords::format = "[%TimeStamp%]: %Message%"
	);

	log->set_filter(
		boost::log::trivial::severity >= boost::log::trivial::trace
	);
}

void logs() {
	std::string ch = "0000";
	std::string str = std::to_string(std::rand());
	std::string hash = picosha2::hash256_hex_string(str);
	std::string lastChar = hash.substr(hash.size() - ch.size());
	if (lastChar == ch)
		BOOST_LOG_TRIVIAL(info) << "0000 founded in hash '" << hash << "' of string '" << str << "'";
	else
		BOOST_LOG_TRIVIAL(trace) << "Hash '" << hash << "' get from string'" << str << "'";
}

int main(int argc, char* argv[]) {
	cout << "hello";
	if (false) {
		size_t count;
		srand(time(NULL));
		init();
		boost::log::add_common_attributes();

		if (argc >= 2) {
			count = boost::lexical_cast<size_t>(argv[1]);
		}
		else {
			count = boost::thread::hardware_concurrency();
		}
		BOOST_LOG_TRIVIAL(trace) << "Threads count" << count;

		std::vector<boost::thread> thrs;
		thrs.resize(count);

		for (size_t i = 0; i < count; i++)
			thrs.emplace_back(logs);
		for (boost::thread& thread : thrs)
			thread.join();
		return 0;
	}
}



//int main(int argc, char* argv[]) {
//	int M = thread::hardware_concurrency;
//	if (argc > 1)
//		M = atoi(argv[1]);
//	
//}
