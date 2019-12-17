// Copyright 2018 Your Name <your_email>
#include <thread>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "../third-party/PicoSHA2/picosha2.h"

#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup.hpp>

using std::thread;
using std::atoi;
using std::vector;
using std::cout;
using std::string;
using std::size_t;
using std::to_string;
using std::rand;

void init_logs() {
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
	srand(time(NULL));
	static const string ideal("0000");
	string str = to_string(rand());
	string hash = picosha2::hash256_hex_string(str);
	if (hash.substr(hash.size() - ideal.size()) == ideal)
		BOOST_LOG_TRIVIAL(info) << "Positive result: " << hash << ", which is result of string " << str << "\n";
	else
		BOOST_LOG_TRIVIAL(trace) << "Non-positive result: " << hash << "' get from string'" << str << "'";
}

int main(int argc, char* argv[]) {
	size_t M = thread::hardware_concurrency;
	if (argc > 1)
		M = atoi(argv[1]);
	cout << "hello";
	init_logs();
	boost::log::add_common_attributes();
	BOOST_LOG_TRIVIAL(trace) << "Threads amount: " << count;
	vector<thread> thrs;
	thrs.resize(count);
	for (size_t i = 0; i < M; i++)
		thrs.emplace_back(logs);
	for (thread& th : thrs)
		th.join();
	return 0;
}

