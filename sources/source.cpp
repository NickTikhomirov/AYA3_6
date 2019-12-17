// Copyright 2018 Your Name <your_email>
#include <thread>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "picosha2.h"

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
	auto log = boost::log::add_file_log(
		boost::log::keywords::file_name = "../logs/logFile_%N.log",
		boost::log::keywords::rotation_size = 10 * 1024 * 1024,
		boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point{ 0, 0, 0 },
		boost::log::keywords::format = "[%TimeStamp%]: %Message%"
	);

	auto log2 = boost::log::add_console_log(
		cout,
		boost::log::keywords::format = "[%TimeStamp%]: %Message%"
	);

	log->set_filter(
		boost::log::trivial::severity >= boost::log::trivial::trace
	);
	log2->set_filter(
		boost::log::trivial::severity >= boost::log::trivial::info
	);
}

void logs() {
	srand(*(new int));
	static const string ideal("0000");
	for (char r : {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'}) {
		string s = to_string(rand());
		string crypted = picosha2::hash256_hex_string(s);
		if (crypted.substr(crypted.size() - ideal.size()) != ideal)
			BOOST_LOG_TRIVIAL(trace) << "Non-positive result: " << crypted << ", which result of " << s;
		else
			BOOST_LOG_TRIVIAL(info) << "Positive result: " << crypted << ", which result of " << s;
	}
}

int main(int argc, char* argv[]) {
	size_t M = thread::hardware_concurrency();
	if (argc > 1)
		M = atoi(argv[1]);
	boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
	init_logs();
	boost::log::add_common_attributes();
	BOOST_LOG_TRIVIAL(trace) << "Threads amount: " << M;
	vector<thread*> threads;
	threads.resize(M);
	for (size_t i = 0; i < M; i++)
		threads[i] = new thread(logs);
	for (auto th : threads)
		th->join();
	return 0;
}

