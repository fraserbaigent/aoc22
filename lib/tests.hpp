#ifndef TESTS_HPP
#define TESTS_HPP
#include <unordered_map>
#include <iostream>
#include <functional>

namespace Tests {

    template <typename T, typename P, typename F>
    void doTest(std::unordered_map<T, P> const& test_cases,
		F && function_to_test) {

	for(auto & test : test_cases) {
	    if (function_to_test(test.first) != test.second) {
		std::cout << "Test failed: " << test.first <<
		    " expected " << test.second <<
		    " received " << function_to_test(test.first) <<"\n";
	    } else {
	    std::cout << "Test passed: " << test.first <<"\n";
	}
    }    
    }
};
#endif
