// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of Émilien KIA nor the name of any other
//    contributor may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY Émilien KIA AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL Émilien KIA OR ANY OTHER
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "catch.hpp"

#include "tape.hpp"

#include <list>
#include <vector>

TEST_CASE( "Tape default construction", "[tape]" ) {
	container::tape<int> tape;
	CHECK( tape.size() == (size_t)0 );
}

TEST_CASE( "Tape sized construction", "[tape]" ) {
	container::tape<int> tape(5);
	CHECK( tape.size() == 5);
	CHECK( tape.front() == 0 );
	CHECK( tape.back() == 0 );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == 0);
}

TEST_CASE( "Tape filling construction", "[tape]" ) {
	container::tape<int> tape( (size_t)5, 42);

	CHECK( tape.size() == (size_t)5 );
	CHECK( tape.front() == 42 );
	CHECK( tape.back() == 42 );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == 42 );
}

TEST_CASE( "Tape range construction from array", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);

	CHECK( tape.size() == (size_t)10 );
	CHECK( tape.front() == 0 );
	CHECK( tape.back() == 9 );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == source[n] );
}

TEST_CASE( "Tape range construction from vector", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::vector<int> vector(source, source+10);

	container::tape<int> tape(vector.begin(), vector.end());

	CHECK( tape.size() == (size_t)10 );
	CHECK( tape.front() == vector.front() );
	CHECK( tape.back() == vector.back() );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == source[n] );
}

TEST_CASE( "Tape range construction from list", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::list<int> list(source, source+10);

	container::tape<int> tape(list.begin(), list.end());

	CHECK( tape.size() == (size_t)10 );
	CHECK( tape.front() == list.front() );
	CHECK( tape.back() == list.back() );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == source[n] );
}


TEST_CASE( "Tape copy construction", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1(source, source+10);
	container::tape<int> tape2(tape1);

	CHECK( tape2.size() == tape1.size() );
	CHECK( tape2.front() == tape1.front() );
	CHECK( tape2.back() == tape1.back() );
	for(size_t n=0; n<tape2.size(); ++n)
	    CHECK( tape2[n] == tape1[n] );
}


TEST_CASE( "Tape copy assignement", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1(source, source+10);
	container::tape<int> tape2;

	tape2 = tape1;

	CHECK( tape2.size() == tape1.size() );
	CHECK( tape2.front() == tape1.front() );
	CHECK( tape2.back() == tape1.back() );
	for(size_t n=0; n<tape2.size(); ++n)
	    CHECK( tape2[n] == tape1[n] );
}

TEST_CASE( "Tape iterator begin", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	container::tape<int>::iterator it = tape.begin();
	container::tape<int>::const_iterator cit = tape.cbegin();

	container::tape<int>::reverse_iterator rit = tape.rbegin();
	container::tape<int>::const_reverse_iterator crit = tape.crbegin();
	
	CHECK( *it == source[0] );
	CHECK( *cit == source[0] );

	CHECK( *rit == source[9] );
	CHECK( *crit == source[9] );	
}

TEST_CASE( "Tape iterator end", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	container::tape<int>::iterator it = tape.end();
	container::tape<int>::const_iterator cit = tape.cend();

	container::tape<int>::reverse_iterator rit = tape.rend();
	container::tape<int>::const_reverse_iterator crit = tape.crend();
	
	CHECK( *--it == source[9] );
	CHECK( *--cit == source[9] );

	CHECK( *--rit == source[0] );
	CHECK( *--crit == source[0] );	
}


TEST_CASE( "Tape empty", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1;
	container::tape<int> tape2(source, source+10);

	CHECK( tape1.empty() );
	CHECK( !tape2.empty() );	
}

TEST_CASE( "Tape size", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1;
	container::tape<int> tape2(source, source+10);

	CHECK( tape1.size() == 0 );
	CHECK( tape2.size() == 10 );	
}

TEST_CASE( "Tape resize", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CHECK( tape.size() == 10 );

	tape.resize(8);
	CHECK( tape.size() == 8 );

	tape.resize(12);
	CHECK( tape.size() == 12 );
	CHECK( tape.back() == 0 );

	tape.resize(64, 42);
	CHECK( tape.size() == 64 );
	CHECK( tape.back() == 42 );

}

template<typename T>
static bool verify_capacity(const container::tape<T>& tape)
{
	return tape.capacity() == tape.size() + tape.capacity_before() + tape.capacity_after();
}


TEST_CASE( "Tape capacity", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1;
	container::tape<int> tape2(source, source+10);

	CHECK( verify_capacity(tape1) );
	CHECK( verify_capacity(tape2) );	
}

TEST_CASE( "Tape reserve", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve(n);
	CHECK( tape.capacity_after() + tape.size() >= n  );
}

TEST_CASE( "Tape reserve before", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve_before(n);
	CHECK( tape.capacity_before() >= n  );
}

TEST_CASE( "Tape reserve after", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve_after(n);
	CHECK( tape.capacity_after() >= n  );
}

TEST_CASE( "Tape reserve before and after", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve(n, n);
	CHECK( tape.capacity_before() >= n  );
	CHECK( tape.capacity_after() >= n  );
}

TEST_CASE( "Tape shrink to fit", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve(n, n);
	CHECK( tape.capacity_before() > 0 );
	CHECK( tape.capacity_after() > 0 );

	tape.shrink_to_fit();
	CHECK( tape.capacity_before() == 0 );
	CHECK( tape.capacity_after() == 0 );
	CHECK( tape.capacity() == tape.size() );
}

TEST_CASE( "Tape front", "[tape]" ) {
	int source[] = {2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CHECK( tape.front() == source[0] );
}

TEST_CASE( "Tape back", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CHECK( tape.back() == source[9] );
	CHECK( tape.back() == tape[tape.size()-1] );
}

TEST_CASE( "Tape at", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	for(size_t n=0; n<10; n++)
	{
		CHECK( tape[n] == source[n] );
		CHECK( tape.at(n) == source[n] );
	}
}

TEST_CASE( "Tape at out of bound", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CHECK_THROWS_AS( tape.at(tape.size()) , std::out_of_range );
	CHECK_THROWS_AS( tape.at(200) , std::out_of_range );
}

TEST_CASE( "Tape data", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CHECK( tape.data() == &tape[0] );
}

TEST_CASE( "Tape range assign from array", "[tape]" ) {
	int source1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int source2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	container::tape<int> tape(source1, source1+5);

	tape.assign(source2, source2+10);

	CHECK( tape.size() == 10 );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == source2[n] );
}

TEST_CASE( "Tape range assign from vector", "[tape]" ) {
	int source1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int source2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	std::vector<int> vector(source2, source2+10);
	container::tape<int> tape(source1, source1+5);

	tape.assign(vector.begin(), vector.end());

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == vector[n] );
}

TEST_CASE( "Tape range assign from list", "[tape]" ) {
	int source1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int source2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	std::list<int> list(source2, source2+10);
	container::tape<int> tape(source1, source1+5);

	tape.assign(list.begin(), list.end());

	CHECK( tape.size() == list.size() );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == source2[n] );
}


TEST_CASE( "Tape filling assign", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	tape.assign((size_t)5, 42);

	CHECK( tape.size() == 5 );
	for(size_t n=0; n<tape.size(); ++n)
	    CHECK( tape[n] == 42 );
}


TEST_CASE( "Tape push back one from empty", "[tape]" ) {
	container::tape<int> tape;
	tape.push_back(42);
	CHECK( tape.size() == 1 );
	CHECK( tape.back() == 42 );
	CHECK( tape.front() == 42 );
}

TEST_CASE( "Tape push back one", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 42};
	container::tape<int> tape(source, source+10);

	tape.push_back(42);

	CHECK( tape.size() == 11 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}

TEST_CASE( "Tape push back many from empty", "[tape]" ) {
	container::tape<int> tape;
	tape.push_back(42, (size_t)5);
	CHECK( tape.size() == 5 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == 42 );
}

TEST_CASE( "Tape push back many", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 42, 42, 42, 42, 42};
	container::tape<int> tape(source, source+10);

	tape.push_back(42, (size_t)5);

	CHECK( tape.size() == 15 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}

TEST_CASE( "Tape push back range from empty", "[tape]" ) {
	int add[]   = {42, 1, 23, 456, 78, 9};
	int verif[] = {42, 1, 23, 456, 78, 9};
	container::tape<int> tape;
	tape.push_back(add, add+6);
	CHECK( tape.size() == 6 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}

TEST_CASE( "Tape push back range", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int add[]    =                               {42, 1, 23, 456, 78, 9};
	int verif[]  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 42, 1, 23, 456, 78, 9};
	container::tape<int> tape(source, source+10);

	tape.push_back(add, add+6);

	CHECK( tape.size() == 16);
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}

TEST_CASE( "Tape push front one from epmty", "[tape]" ) {
	container::tape<int> tape;
	tape.push_front(42);
	CHECK( tape.size() == 1 );
	CHECK( tape.back() == 42 );
	CHECK( tape.front() == 42 );
}

TEST_CASE( "Tape push front one", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[] = {42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	tape.push_front(42);

	CHECK( tape.size() == 11 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}

TEST_CASE( "Tape push front many from epmty", "[tape]" ) {
	container::tape<int> tape;
	tape.push_front(42, (size_t)5);
	CHECK( tape.size() == 5 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == 42 );
}

TEST_CASE( "Tape push front many", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[] = {42, 42, 42, 42, 42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	tape.push_front(42, (size_t)5);

	CHECK( tape.size() == 15 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}


TEST_CASE( "Tape push front range from epmty", "[tape]" ) {
	int add[]   = {42, 1, 23, 456, 78, 9};
	int verif[] = {42, 1, 23, 456, 78, 9};
	container::tape<int> tape;
	tape.push_back(add, add+6);
	CHECK( tape.size() == 6 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}

TEST_CASE( "Tape push front range", "[tape]" ) {
	int source[] =                        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int add[]    = {42, 1, 23, 456, 78, 9};
	int verif[]  = {42, 1, 23, 456, 78, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	tape.push_front(add, add+6);

	CHECK( tape.size() == 16 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}

TEST_CASE( "Tape pop back one", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[]  = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	container::tape<int> tape(source, source+10);

	tape.pop_back();

	CHECK( tape.size() == 9 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}


TEST_CASE( "Tape pop back many", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[]  = {0, 1, 2, 3, 4, 5, 6};
	container::tape<int> tape(source, source+10);

	tape.pop_back(3);

	CHECK( tape.size() == 7 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}

TEST_CASE( "Tape pop front one", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[]  =    {1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	tape.pop_front();

	CHECK( tape.size() == 9 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}


TEST_CASE( "Tape pop front many", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[]  =          {3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	tape.pop_front(3);

	CHECK( tape.size() == 7 );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == verif[n] );
}


TEST_CASE( "Tape insert one", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);
	std::vector<int> vector(source, source+10);

	// Insert at begining
	tape.insert(tape.begin(), 42);
	vector.insert(vector.begin(), 42);

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Insert at middle
	tape.insert(tape.begin()+4, 42);
	vector.insert(vector.begin()+4, 42);

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Insert at end
	tape.insert(tape.end(), 42);
	vector.insert(vector.end(), 42);
	
	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );
}


TEST_CASE( "Tape insert many", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);
	std::vector<int> vector(source, source+10);

	// Insert at begining
	tape.insert(tape.begin(), (size_t)3, 42);
	vector.insert(vector.begin(), (size_t)3, 42);
	
	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Insert at middle
	tape.insert(tape.begin()+6, (size_t)5, 42);
	vector.insert(vector.begin()+6, (size_t)5, 42);

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Insert at end
	tape.insert(tape.end(), (size_t)8, 42);
	vector.insert(vector.end(), (size_t)8, 42);
	
	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );
}

TEST_CASE( "Tape insert range", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int modifs[] = {42, 1, 23, 456, 78, 9};
	container::tape<int> tape(source, source+10);
	std::vector<int> vector(source, source+10);

	// Insert at begining
	tape.insert(tape.begin(), modifs, modifs+6);
	vector.insert(vector.begin(), modifs, modifs+6);

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Insert at middle
	tape.insert(tape.begin()+9, modifs, modifs+6);
	vector.insert(vector.begin()+9, modifs, modifs+6);

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Insert at end
	tape.insert(tape.end(), modifs, modifs+6);
	vector.insert(vector.end(), modifs, modifs+6);
	
	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );
}

TEST_CASE( "Tape erase one", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);
	std::vector<int> vector(source, source+10);

	// Erase at begining
	tape.erase(tape.begin());
	vector.erase(vector.begin());

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Erase at middle
	tape.erase(tape.begin()+4);
	vector.erase(vector.begin()+4);

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Erase at end
	tape.erase(tape.end()-1);
	vector.erase(vector.end()-1);
	
	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );
}

TEST_CASE( "Tape erase many", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	container::tape<int> tape(source, source+16);
	std::vector<int> vector(source, source+16);

	// Erase at begining
	tape.erase(tape.begin(), tape.begin()+2);
	vector.erase(vector.begin(), vector.begin()+2);

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Erase at middle
	tape.erase(tape.begin()+4, tape.begin()+6);
	vector.erase(vector.begin()+4, vector.begin()+6);

	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );

	// Erase at end
	tape.erase(tape.end()-3, tape.end()-1);
	vector.erase(vector.end()-3, vector.end()-1);
	
	CHECK( tape.size() == vector.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CHECK( tape[n] == vector[n] );
}

TEST_CASE( "Tape swap", "[tape]" ) {
	int source1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};	
	int source2[] = {25, 38, 42, 72, -24, 36};

	container::tape<int> tape1(source1, source1+16);
	container::tape<int> tape2(source2, source2+6);

	// Member swap
	tape1.swap(tape2);

	CHECK( tape1.size() == 6 );
	for(size_t n=0; n<tape1.size(); ++n)
		CHECK( tape1[n] == source2[n] );

	CHECK( tape2.size() == 16 );
	for(size_t n=0; n<tape2.size(); ++n)
		CHECK( tape2[n] == source1[n] );

	// External swap
	swap(tape1, tape2);

	CHECK( tape1.size() == 16 );
	for(size_t n=0; n<tape1.size(); ++n)
		CHECK( tape1[n] == source1[n] );

	CHECK( tape2.size() == 6 );
	for(size_t n=0; n<tape2.size(); ++n)
		CHECK( tape2[n] == source2[n] );
}


TEST_CASE( "Tape clear", "[tape]" ) {
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CHECK( !tape.empty() );
	tape.clear();
	CHECK( tape.empty() );	
}
