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

#include <cppunit/extensions/HelperMacros.h>

#include "tape.hpp"

#include <list>
#include <vector>


class TapeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TapeTest );
		CPPUNIT_TEST( testDefaultConstruct );
		CPPUNIT_TEST( testSizedConstruct );
		CPPUNIT_TEST( testFillingConstruct );
		CPPUNIT_TEST( testRangeConstructFromArray );
		CPPUNIT_TEST( testRangeConstructFromVector );
		CPPUNIT_TEST( testRangeConstructFromList );
		CPPUNIT_TEST( testCopyConstruct );
		CPPUNIT_TEST( testCopyAssignment );

		CPPUNIT_TEST( testIteratorBegin );
		CPPUNIT_TEST( testIteratorEnd );

		CPPUNIT_TEST( testEmpty );
		CPPUNIT_TEST( testSize );
		CPPUNIT_TEST( testResize );
		CPPUNIT_TEST( testCapacity );
		CPPUNIT_TEST( testReserve );
		CPPUNIT_TEST( testReserveBeforeAndAfter );	
		CPPUNIT_TEST( testReserveBefore );
		CPPUNIT_TEST( testReserveAfter );
		CPPUNIT_TEST( testShrinkToFit );

		CPPUNIT_TEST( testFront );
		CPPUNIT_TEST( testBack );
		CPPUNIT_TEST( testAt );
		CPPUNIT_TEST( testAtOutOfBound );
		CPPUNIT_TEST( testData );

		CPPUNIT_TEST( testRangeAssignFromArray );
		CPPUNIT_TEST( testRangeAssignFromVector );
		CPPUNIT_TEST( testRangeAssignFromList );
		CPPUNIT_TEST( testFillingAssign );
		CPPUNIT_TEST( testPushBackOne );
		CPPUNIT_TEST( testPushBackMany );
		CPPUNIT_TEST( testPushBackRange );
		CPPUNIT_TEST( testPushFrontOne );
		CPPUNIT_TEST( testPushFrontMany );
		CPPUNIT_TEST( testPushFrontRange );
		CPPUNIT_TEST( testPopBackOne );
		CPPUNIT_TEST( testPopBackMany );
		CPPUNIT_TEST( testPopFrontOne );
		CPPUNIT_TEST( testPopFrontMany );
		CPPUNIT_TEST( testInsertOne );
		CPPUNIT_TEST( testInsertMany );
		CPPUNIT_TEST( testInsertRange );
		CPPUNIT_TEST( testEraseOne );
		CPPUNIT_TEST( testEraseMany );
		CPPUNIT_TEST( testSwap );	
		CPPUNIT_TEST( testClear );

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testDefaultConstruct();
	void testSizedConstruct();
	void testFillingConstruct();
	void testRangeConstructFromArray();
	void testRangeConstructFromVector();
	void testRangeConstructFromList();
	void testCopyConstruct();
	void testCopyAssignment();

	void testIteratorBegin();
	void testIteratorEnd();

	void testEmpty();
	void testSize();
	void testResize();
	void testCapacity();
	void testReserve();
	void testReserveBeforeAndAfter();
	void testReserveBefore();
	void testReserveAfter();
	void testShrinkToFit();

	void testFront();
	void testBack();
	void testAt();
	void testAtOutOfBound();
	void testData();

	void testRangeAssignFromArray();
	void testRangeAssignFromVector();
	void testRangeAssignFromList();
	void testFillingAssign();
	void testPushBackOne();
	void testPushBackMany();
	void testPushBackRange();
	void testPushFrontOne();
	void testPushFrontMany();
	void testPushFrontRange();
	void testPopBackOne();
	void testPopBackMany();
	void testPopFrontOne();
	void testPopFrontMany();
	void testInsertOne();
	void testInsertMany();
	void testInsertRange();
	void testEraseOne();
	void testEraseMany();
	void testSwap();
	void testClear();

	template<typename T>
	bool verifyCapacity(const container::tape<T>& tape)const {return tape.capacity() == tape.size() + tape.capacity_before() + tape.capacity_after();}
};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TapeTest );


void TapeTest::setUp()
{
}


void TapeTest::tearDown()
{
}


void TapeTest::testDefaultConstruct()
{
	container::tape<int> tape;
	CPPUNIT_ASSERT_EQUAL( (size_t)0, tape.size() );
}

void TapeTest::testSizedConstruct()
{
	container::tape<int> tape(5);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)5, tape.size() );
	CPPUNIT_ASSERT_EQUAL( 0, tape.front() );
	CPPUNIT_ASSERT_EQUAL( 0, tape.back() );

	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( 0, tape[n] );
}

void TapeTest::testFillingConstruct()
{
	container::tape<int> tape( (size_t)5, 42);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)5, tape.size() );
	CPPUNIT_ASSERT_EQUAL( 42, tape.front() );
	CPPUNIT_ASSERT_EQUAL( 42, tape.back() );

	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( 42, tape[n] );
}

void TapeTest::testRangeConstructFromArray()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)10, tape.size() );
	CPPUNIT_ASSERT_EQUAL( 0, tape.front() );
	CPPUNIT_ASSERT_EQUAL( 9, tape.back() );

	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( source[n], tape[n] );
}

void TapeTest::testRangeConstructFromVector()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::vector<int> vector(source, source+10);
	
	container::tape<int> tape(vector.begin(), vector.end());
	
	CPPUNIT_ASSERT_EQUAL( (size_t)10, tape.size() );
	CPPUNIT_ASSERT_EQUAL( 0, tape.front() );
	CPPUNIT_ASSERT_EQUAL( 9, tape.back() );

	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( source[n], tape[n] );
}

void TapeTest::testRangeConstructFromList()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::list<int> list(source, source+10);
	
	container::tape<int> tape(list.begin(), list.end());
	
	CPPUNIT_ASSERT_EQUAL( (size_t)10, tape.size() );
	CPPUNIT_ASSERT_EQUAL( 0, tape.front() );
	CPPUNIT_ASSERT_EQUAL( 9, tape.back() );

	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( source[n], tape[n] );
}

void TapeTest::testCopyConstruct()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1(source, source+10);
	
	container::tape<int> tape2(tape1);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)10, tape2.size() );
	CPPUNIT_ASSERT_EQUAL( 0, tape2.front() );
	CPPUNIT_ASSERT_EQUAL( 9, tape2.back() );

	for(size_t n=0; n<tape2.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( source[n], tape2[n] );
}

void TapeTest::testCopyAssignment()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1(source, source+10);
	container::tape<int> tape2;

	tape2 = tape1;
	
	CPPUNIT_ASSERT_EQUAL( (size_t)10, tape2.size() );
	CPPUNIT_ASSERT_EQUAL( 0, tape2.front() );
	CPPUNIT_ASSERT_EQUAL( 9, tape2.back() );

	for(size_t n=0; n<tape2.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( source[n], tape2[n] );
}

void TapeTest::testIteratorBegin()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	container::tape<int>::iterator it = tape.begin();
	container::tape<int>::const_iterator cit = tape.cbegin();

	container::tape<int>::reverse_iterator rit = tape.rbegin();
	container::tape<int>::const_reverse_iterator crit = tape.crbegin();
	
	CPPUNIT_ASSERT_EQUAL( 0, *it );
	CPPUNIT_ASSERT_EQUAL( 0, *cit );
	
	CPPUNIT_ASSERT_EQUAL( 9, *rit );
	CPPUNIT_ASSERT_EQUAL( 9, *crit );
}

void TapeTest::testIteratorEnd()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	container::tape<int>::iterator it = tape.end();
	container::tape<int>::const_iterator cit = tape.cend();

	container::tape<int>::reverse_iterator rit = tape.rend();
	container::tape<int>::const_reverse_iterator crit = tape.crend();
	
	CPPUNIT_ASSERT_EQUAL( 9, *--it );
	CPPUNIT_ASSERT_EQUAL( 9, *--cit );
	
	CPPUNIT_ASSERT_EQUAL( 0, *--rit );
	CPPUNIT_ASSERT_EQUAL( 0, *--crit );
}

void TapeTest::testEmpty()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1;
	container::tape<int> tape2(source, source+10);

	CPPUNIT_ASSERT( tape1.empty() );
	CPPUNIT_ASSERT( !tape2.empty() );
}

void TapeTest::testSize()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1;
	container::tape<int> tape2(source, source+10);

	CPPUNIT_ASSERT_EQUAL( (size_t)0, tape1.size() );
	CPPUNIT_ASSERT_EQUAL( (size_t)10, tape2.size() );
}

void TapeTest::testResize()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CPPUNIT_ASSERT_EQUAL( (size_t)10, tape.size() );

	tape.resize(8);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)8, tape.size() );

	tape.resize(12);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)12, tape.size() );
	CPPUNIT_ASSERT_EQUAL( 0, tape.back());

	tape.resize(64, 42);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)64, tape.size() );
	CPPUNIT_ASSERT_EQUAL( 42, tape.back());

}

void TapeTest::testCapacity()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape1;
	container::tape<int> tape2(source, source+10);
	
	CPPUNIT_ASSERT( verifyCapacity(tape1) );
	CPPUNIT_ASSERT( verifyCapacity(tape2) );
}

void TapeTest::testReserve()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve(n);
	CPPUNIT_ASSERT( tape.capacity_after() + tape.size() >= n  );	
}

void TapeTest::testReserveBefore()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve_before(n);
	CPPUNIT_ASSERT( tape.capacity_before() >= n );	
}

void TapeTest::testReserveAfter()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve_after(n);
	CPPUNIT_ASSERT( tape.capacity_after() >= n );
}

void TapeTest::testReserveBeforeAndAfter()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve(n, n);
	CPPUNIT_ASSERT( tape.capacity_before() >= n );
	CPPUNIT_ASSERT( tape.capacity_after() >= n );
}

void TapeTest::testShrinkToFit()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	size_t n = 64;
	tape.reserve(n, n);
	tape.shrink_to_fit();
	CPPUNIT_ASSERT_EQUAL( (size_t)0, tape.capacity_before() );
	CPPUNIT_ASSERT_EQUAL( (size_t)0, tape.capacity_after() );
	CPPUNIT_ASSERT_EQUAL( tape.size(), tape.capacity() );
}

void TapeTest::testFront()
{
	int source[] = {2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CPPUNIT_ASSERT_EQUAL( 2, tape.front() );
	CPPUNIT_ASSERT_EQUAL( tape.front(), tape[0] );
}

void TapeTest::testBack()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CPPUNIT_ASSERT_EQUAL( 9, tape.back() );
	CPPUNIT_ASSERT_EQUAL( tape.back(), tape[tape.size()-1] );
}

void TapeTest::testAt()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	for(size_t n=0; n<10; n++)
	{
		CPPUNIT_ASSERT_EQUAL( source[n], tape[n] );
		CPPUNIT_ASSERT_EQUAL( source[n], tape.at(n) );
	}
	
}

void TapeTest::testAtOutOfBound()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CPPUNIT_ASSERT_THROW( tape.at(tape.size()), std::out_of_range);
	CPPUNIT_ASSERT_THROW( tape.at(200), std::out_of_range);
}

void TapeTest::testData()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CPPUNIT_ASSERT_EQUAL( &tape[0], tape.data() );
}



void TapeTest::testRangeAssignFromArray()
{
	int source1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int source2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	
	container::tape<int> tape(source1, source1+5);

	tape.assign(source2, source2+10);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)10, tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( source2[n], tape[n] );
}

void TapeTest::testRangeAssignFromVector()
{
	int source1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int source2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	std::vector<int> vector(source2, source2+10);
	container::tape<int> tape(source1, source1+5);

	tape.assign(vector.begin(), vector.end());
	
	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );
}

void TapeTest::testRangeAssignFromList()
{
	int source1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int source2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	std::list<int> list(source2, source2+10);
	container::tape<int> tape(source1, source1+5);

	tape.assign(list.begin(), list.end());
	
	CPPUNIT_ASSERT_EQUAL( list.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( source2[n], tape[n] );
}

void TapeTest::testFillingAssign()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	tape.assign((size_t)5, 42);
	
	CPPUNIT_ASSERT_EQUAL( (size_t)5, tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
	    CPPUNIT_ASSERT_EQUAL( 42, tape[n] );
}


void TapeTest::testPushBackOne()
{
	{
		container::tape<int> tape;
		tape.push_back(42);
		CPPUNIT_ASSERT_EQUAL( (size_t)1, tape.size() );
		CPPUNIT_ASSERT_EQUAL( 42, tape.back() );
		CPPUNIT_ASSERT_EQUAL( 42, tape.front() );
	}
	{
		int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int verif[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 42};
		container::tape<int> tape(source, source+10);
		tape.push_back(42);
		CPPUNIT_ASSERT_EQUAL( (size_t)11, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
	}
}

void TapeTest::testPushBackMany()
{
	{
		container::tape<int> tape;
		tape.push_back(42, (size_t)5);
		CPPUNIT_ASSERT_EQUAL( (size_t)5, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( 42, tape[n] );
	}
	{
		int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int verif[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 42, 42, 42, 42, 42};
		container::tape<int> tape(source, source+10);
		tape.push_back(42, (size_t)5);
		CPPUNIT_ASSERT_EQUAL( (size_t)15, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
	}
}

void TapeTest::testPushBackRange()
{
	{
		int add[]   = {42, 1, 23, 456, 78, 9};
		int verif[] = {42, 1, 23, 456, 78, 9};
		container::tape<int> tape;
		tape.push_back(add, add+6);
		CPPUNIT_ASSERT_EQUAL( (size_t)6, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
	}
	{
		int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int add[]    =                               {42, 1, 23, 456, 78, 9};
		int verif[]  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 42, 1, 23, 456, 78, 9};
		container::tape<int> tape(source, source+10);
		tape.push_back(add, add+6);
		CPPUNIT_ASSERT_EQUAL( (size_t)16, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
	}
}

void TapeTest::testPushFrontOne()
{
	{
		container::tape<int> tape;
		tape.push_front(42);
		CPPUNIT_ASSERT_EQUAL( (size_t)1, tape.size() );
		CPPUNIT_ASSERT_EQUAL( 42, tape.back() );
		CPPUNIT_ASSERT_EQUAL( 42, tape.front() );
	}
	{
		int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int verif[] = {42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		container::tape<int> tape(source, source+10);
		tape.push_front(42);
		CPPUNIT_ASSERT_EQUAL( (size_t)11, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
	}
}

void TapeTest::testPushFrontMany()
{
	{
		container::tape<int> tape;
		tape.push_front(42, (size_t)5);
		CPPUNIT_ASSERT_EQUAL( (size_t)5, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( 42, tape[n] );
	}
	{
		int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int verif[] = {42, 42, 42, 42, 42, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		container::tape<int> tape(source, source+10);
		tape.push_front(42, (size_t)5);
		CPPUNIT_ASSERT_EQUAL( (size_t)15, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
	}
}

void TapeTest::testPushFrontRange()
{
	{
		int add[]   = {42, 1, 23, 456, 78, 9};
		int verif[] = {42, 1, 23, 456, 78, 9};
		container::tape<int> tape;
		tape.push_back(add, add+6);
		CPPUNIT_ASSERT_EQUAL( (size_t)6, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
	}
	{
		int source[] =                        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		int add[]    = {42, 1, 23, 456, 78, 9};
		int verif[]  = {42, 1, 23, 456, 78, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		container::tape<int> tape(source, source+10);
		tape.push_front(add, add+6);
		CPPUNIT_ASSERT_EQUAL( (size_t)16, tape.size() );
		for(size_t n=0; n<tape.size(); ++n)
			CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
	}
}

void TapeTest::testPopBackOne()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[]  = {0, 1, 2, 3, 4, 5, 6, 7, 8};

	container::tape<int> tape(source, source+10);
	tape.pop_back();
	CPPUNIT_ASSERT_EQUAL( (size_t)9, tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
}

void TapeTest::testPopBackMany()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[]  = {0, 1, 2, 3, 4, 5, 6};

	container::tape<int> tape(source, source+10);
	tape.pop_back(3);
	CPPUNIT_ASSERT_EQUAL( (size_t)7, tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
}

void TapeTest::testPopFrontOne()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[]  =    {1, 2, 3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);
	tape.pop_front();
	CPPUNIT_ASSERT_EQUAL( (size_t)9, tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
}

void TapeTest::testPopFrontMany()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int verif[]  =          {3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);
	tape.pop_front(3);
	CPPUNIT_ASSERT_EQUAL( (size_t)7, tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( verif[n], tape[n] );
}

void TapeTest::testInsertOne()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);
	std::vector<int> vector(source, source+10);

	// Insert at begining
	tape.insert(tape.begin(), 42);
	vector.insert(vector.begin(), 42);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Insert at middle
	tape.insert(tape.begin()+4, 42);
	vector.insert(vector.begin()+4, 42);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Insert at end
	tape.insert(tape.end(), 42);
	vector.insert(vector.end(), 42);
	
	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );
}

void TapeTest::testInsertMany()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);
	std::vector<int> vector(source, source+10);

	// Insert at begining
	tape.insert(tape.begin(), (size_t)3, 42);
	vector.insert(vector.begin(), (size_t)3, 42);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Insert at middle
	tape.insert(tape.begin()+6, (size_t)5, 42);
	vector.insert(vector.begin()+6, (size_t)5, 42);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Insert at end
	tape.insert(tape.end(), (size_t)8, 42);
	vector.insert(vector.end(), (size_t)8, 42);
	
	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );
}

void TapeTest::testInsertRange()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int modifs[] = {42, 1, 23, 456, 78, 9};
	container::tape<int> tape(source, source+10);
	std::vector<int> vector(source, source+10);

	// Insert at begining
	tape.insert(tape.begin(), modifs, modifs+6);
	vector.insert(vector.begin(), modifs, modifs+6);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Insert at middle
	tape.insert(tape.begin()+9, modifs, modifs+6);
	vector.insert(vector.begin()+9, modifs, modifs+6);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Insert at end
	tape.insert(tape.end(), modifs, modifs+6);
	vector.insert(vector.end(), modifs, modifs+6);
	
	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );
}

void TapeTest::testEraseOne()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	container::tape<int> tape(source, source+10);
	std::vector<int> vector(source, source+10);

	// Erase at begining
	tape.erase(tape.begin());
	vector.erase(vector.begin());

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Erase at middle
	tape.erase(tape.begin()+4);
	vector.erase(vector.begin()+4);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Erase at end
	tape.erase(tape.end()-1);
	vector.erase(vector.end()-1);
	
	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );
}

void TapeTest::testEraseMany()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	container::tape<int> tape(source, source+16);
	std::vector<int> vector(source, source+16);

	// Erase at begining
	tape.erase(tape.begin(), tape.begin()+2);
	vector.erase(vector.begin(), vector.begin()+2);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Erase at middle
	tape.erase(tape.begin()+4, tape.begin()+6);
	vector.erase(vector.begin()+4, vector.begin()+6);

	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );

	// Erase at end
	tape.erase(tape.end()-3, tape.end()-1);
	vector.erase(vector.end()-3, vector.end()-1);
	
	CPPUNIT_ASSERT_EQUAL( vector.size(), tape.size() );
	for(size_t n=0; n<tape.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( vector[n], tape[n] );
}

void TapeTest::testSwap()
{
	int source1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};	
	int source2[] = {25, 38, 42, 72, -24, 36};

	container::tape<int> tape1(source1, source1+16);
	container::tape<int> tape2(source2, source2+6);

	// Member swap
	tape1.swap(tape2);

	CPPUNIT_ASSERT_EQUAL( (size_t)6, tape1.size() );
	for(size_t n=0; n<tape1.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( source2[n], tape1[n] );

	CPPUNIT_ASSERT_EQUAL( (size_t)16, tape2.size() );
	for(size_t n=0; n<tape2.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( source1[n], tape2[n] );

	// External swap
	swap(tape1, tape2);

	CPPUNIT_ASSERT_EQUAL( (size_t)16, tape1.size() );
	for(size_t n=0; n<tape1.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( source1[n], tape1[n] );

	CPPUNIT_ASSERT_EQUAL( (size_t)6, tape2.size() );
	for(size_t n=0; n<tape2.size(); ++n)
		CPPUNIT_ASSERT_EQUAL( source2[n], tape2[n] );
}

void TapeTest::testClear()
{
	int source[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	container::tape<int> tape(source, source+10);

	CPPUNIT_ASSERT( !tape.empty() );
	tape.clear();
	CPPUNIT_ASSERT( tape.empty() );	
}

