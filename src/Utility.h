/*
 * Utility.h
 *
 *  Created on: Oct 6, 2016
 *      Author: torkit
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <sstream>
#include <strstream>
#include <string>
#include <cstring>
#include <iterator>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

namespace WASQ {

template<class T, class Iterator>
static void tokenize(const char *str, Iterator iterator) {
	std::istrstream is(str, std::strlen(str));
	std::copy(std::istream_iterator < T > (is), std::istream_iterator<T>(),
			iterator);
}

static unsigned int rsHash(vector<int> ids) {
	unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;
    for (int i = 0; i < ids.size(); i++) {
    	hash = hash * a + ids[i];
    	a = a * b;
    }
    return (hash & 0x7FFFFFFF);
}

}
#endif /* UTILITY_H_ */
