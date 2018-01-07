// ----------------------------------------------------------------------------
// Copyright (C) 2017-2018 Emilien KIA
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
// ----------------------------------------------------------------------------

#include <cstddef>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <iterator>
#include <utility>



namespace container
{

	/**
	 * Tape iterator.
	 */
	template <class T>
	class tape_iterator : public std::iterator<std::random_access_iterator_tag, T>
	{
	public:
		typedef tape_iterator   self;
		typedef std::iterator<std::random_access_iterator_tag, T>   parent;
			
		typedef typename parent::value_type			value_type;
		typedef typename parent::difference_type		difference_type;
		typedef typename parent::pointer				pointer;
		typedef typename parent::reference			reference;
		typedef typename parent::iterator_category   iterator_category;

	protected:
		pointer _ptr;
			
	public:			
		tape_iterator():_ptr(nullptr){}
		explicit tape_iterator(pointer ptr):_ptr(ptr){}

		pointer get_ptr()const {return _ptr;}

		reference  operator*() const {return *_ptr;}
		value_type operator->() const {return _ptr;}
		reference operator[](difference_type off) const {return _ptr[off];}

		self& operator++() {++_ptr; return *this;}
		self  operator++(int) {pointer tmp = _ptr; ++*this; return self(tmp);}
		self& operator--() {--_ptr; return *this;}
		self  operator--(int) {pointer tmp = _ptr; --*this; return self(tmp);}

		self& operator+=(difference_type off) {_ptr += off; return *this;}
		self  operator+(difference_type off)const {return self(_ptr+off);}
		friend self operator+(difference_type off, const self& right) {return self(off+right._ptr);}
		self& operator-=(difference_type off) {_ptr -= off; return *this;}
		self  operator-(difference_type off)const {return self(_ptr-off);}
		difference_type operator-(const self& right)const {return _ptr - right._ptr;}

		bool operator==(const self& r)const{return _ptr==r._ptr;}
		bool operator!=(const self& r)const{return _ptr!=r._ptr;}
		bool operator<(const self& r)const{return _ptr<r._ptr;}			
		bool operator<=(const self& r)const{return _ptr<=r._ptr;}
		bool operator>(const self& r)const{return _ptr>r._ptr;}			
		bool operator>=(const self& r)const{return _ptr>=r._ptr;}
	};

	/**
	 * Tape constant iterator.
	 */
	template <class T>
	class tape_const_iterator : public std::iterator<std::random_access_iterator_tag, T>
	{
	public:
		typedef tape_const_iterator   self;
		typedef std::iterator<std::random_access_iterator_tag, T>   parent;
			
		typedef typename parent::value_type			value_type;
		typedef typename parent::difference_type	difference_type;
		typedef typename parent::pointer			pointer;
		typedef typename parent::reference			reference;
		typedef typename parent::iterator_category  iterator_category;

	protected:
		pointer _ptr;

	public:
		tape_const_iterator():_ptr(nullptr){}
		explicit tape_const_iterator(pointer ptr):_ptr(ptr){}
		tape_const_iterator(const tape_iterator<T>& it):_ptr(it.get_ptr()){}

		pointer get_ptr()const {return _ptr;}

		const reference  operator*() const {return *_ptr;}
		const value_type operator->() const {return _ptr;}
		const reference operator[](difference_type off) const {return _ptr[off];}

		self& operator++() {++_ptr; return *this;}
		self  operator++(int) {pointer tmp = _ptr; ++*this; return self(tmp);}
		self& operator--() {--_ptr; return *this;}
		self  operator--(int) {pointer tmp = _ptr; --*this; return self(tmp);}

		self& operator+=(difference_type off) {_ptr += off; return *this;}
		self  operator+(difference_type off)const {return self(_ptr+off);}
		friend self operator+(difference_type off, const self& right) {return self(off+right._ptr);}
		self& operator-=(difference_type off) {_ptr -= off; return *this;}
		self  operator-(difference_type off)const {return self(_ptr-off);}
		difference_type operator-(const self& right)const {return _ptr - right._ptr;}

		bool operator==(const self& r)const{return _ptr==r._ptr;}
		bool operator!=(const self& r)const{return _ptr!=r._ptr;}
		bool operator<(const self& r)const{return _ptr<r._ptr;}			
		bool operator<=(const self& r)const{return _ptr<=r._ptr;}
		bool operator>(const self& r)const{return _ptr>r._ptr;}			
		bool operator>=(const self& r)const{return _ptr>=r._ptr;}
	};
	
	/**
	 * Tapes are sequence containers representing arrays that can change in size (like STL vectors).
	 *
	 * Introduction
	 * ============
	 *
	 * Just like arrays and vectors, tapes use contiguous storage locations for their elements,
	 * which means that their elements can also be accessed using offsets on regular pointers to its elements,
	 * and just as efficiently as in arrays.
	 *
	 * Like vectors but unlike arrays, their size can change dynamically, with their storage being handled automatically by the container.
	 * Internally, tapes use a dynamically allocated array to store their elements.
	 * This array may need to be reallocated in order to grow in size when new elements are inserted,
	 * which implies allocating a new array and moving all elements to it.
	 * This is a relatively expensive task in terms of processing time, and thus,
	 * tapes do not reallocate each time an element is added to the container.
	 * Instead, tape containers may allocate some extra storage to accommodate for possible growth,
	 * and thus the container may have an actual capacity greater than the storage strictly needed to contain its elements (i.e., its size).
	 *
	 * Compared to the other dynamic sequence containers (deques, lists and forward_lists),
	 * tapes, like vectors, are very efficient accessing its elements (just like arrays)
	 * and relatively efficient adding or removing elements from its begining and end.
	 * For operations that involve inserting or removing elements at positions other than the begining and  end,
	 * they perform worse than the others, and have less consistent iterators and references than lists and forward_lists.
	 * 
	 * Container properties
	 * ====================
	 *
	 * - **Sequence** : Elements in sequence containers are ordered in a strict linear sequence.
	 * Individual elements are accessed by their position in this sequence.
	 * - **Dynamic array** : Allows direct access to any element in the sequence, even through pointer arithmetics,
	 * and provides relatively fast addition/removal of elements at the end of the sequence.
	 * - **Allocator-aware** : The container uses an allocator object to dynamically handle its storage needs.
     *
     *
	 * \tparam T Type of the elements. Only if T is guaranteed to not throw while moving, implementations can optimize to move elements instead of copying them during reallocations. Aliased as member type tape::value_type.
	 *
     * \tparam Allocator Type of the allocator object used to define the storage allocation model. By default, the allocator class template is used, which defines the simplest memory allocation model and is value-independent. Aliased as member type tape::allocator_type.
	 */
	template <typename T, typename Allocator = std::allocator<T> >
	class tape
	{
		typedef Allocator base_t;
	public:
		/**
		 * 
		 * \name STL type definitions:
		 * 
		 * \{ */
		typedef T											value_type;			//!< The type of object stored in the vector. The first template parameter (T).
		typedef Allocator									allocator_type;		//!< The type of allocator used for internal memory management. The second template parameter (Allocator). Defaults to std::allocator<value_type>.

		typedef typename allocator_type::reference			reference;			//!< Reference to the stored element.
		typedef typename allocator_type::const_reference	const_reference;	//!< Const reference to the stored element.
		typedef typename allocator_type::pointer			pointer;			//!< Pointer to the stored element.
		typedef typename allocator_type::const_pointer		const_pointer;		//!< Const pointer to the stored element.

		typedef tape_iterator<value_type>					iterator;			//!< Random access iterator to value_type.
		typedef tape_const_iterator<value_type>				const_iterator;		//!< Random access iterator to const value_type.
		typedef std::reverse_iterator<iterator>				reverse_iterator;	//!< Reverse iterator to value_type.
		typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;	//!< Reverse iterator to const value_type.
			
		typedef ptrdiff_t									difference_type;	//!< Signed integral type representing the distance between two stored objects. Identical to iterator_traits<iterator>::difference_type. Usually the same as ptrdiff_t.
		typedef size_t										size_type;			//!< Unsigned integral type that can represent any non-negative value of difference_type like a quantity of elments. Usually same as size_t.
		/** \} */
			
	protected:
		allocator_type _alloc;  // Allocator
		pointer		_base;		// Base allocated memory chunck
		pointer		_start;		// Begining of used memory (first element)
		size_type   _size;		// Number of used elements
		size_type	_capacity;  // Size of allocated memory in nb of elements

	public:
		/**
		 * \name Construct / Copy / Destroy
		 * \{ */

		/** Default constructor with allocator.
		 * Constructs an empty container, with no elements.
		 * \param alloc Eventual allocator sample.
		 */
#if   __cplusplus < 201402L // (until C++14)
		explicit tape(const allocator_type& alloc = allocator_type())
#elif __cplusplus >= 201402L && __cplusplus < 201703L // (since C++14)(until C++17)
		explicit tape(const allocator_type& alloc)
#else // (since C++17)
		explicit tape( const Allocator& alloc ) noexcept
#endif
		:_alloc(alloc), _base(nullptr), _start(nullptr), _size(0), _capacity(0)
		{}

		/** Default constructor.
		 * Constructs an empty container, with no elements.
		 * \param alloc Eventual allocator sample.
		 */
#if   __cplusplus >= 201402L && __cplusplus < 201703L // (since C++14)(until C++17)
		tape() : tape( Allocator() ) {}
#elif __cplusplus >= 201703L // (since C++17)
		tape() noexcept(noexcept(Allocator())): tape( Allocator() ) {}
#endif

		/** Filling constructor.
		 * Constructs a container with n elements with specified element value.
		 * \param n Number of element to store in container.
		 * \param val Value to fill the container with. Each of the n elements in the container will be initialized to a copy of this value.
		 * \param alloc Eventual allocator sample.
		 */
		tape(size_type n, const value_type& val, const allocator_type& alloc = allocator_type())
		:_alloc(alloc), _base(nullptr), _start(nullptr), _size(0), _capacity(0)
		{
			this->resize(n, val);
		}

		/** Filling constructor.
		 * Constructs a container with n elements with default element value.
		 * \param n Number of element to store in container.
		 */
#if   /*__cplusplus >= 201103L &&*/ __cplusplus < 201402L // (since C++11)(until C++14)
		explicit tape(size_type n):
		_alloc(), _base(nullptr), _start(nullptr), _size(0), _capacity(0)
		{
			this->resize(n);
		}
#elif __cplusplus >= 201402L // (since C++14)
		explicit tape(size_type n, const allocator_type& alloc = allocator_type()):
		_alloc(alloc), _base(nullptr), _start(nullptr), _size(0), _capacity(0)
		{
			this->resize(n);
		}
#endif

		/** Range constructor.
		 * Constructs a container with as many elements as the range [first,last),
		 * with each element constructed from its corresponding element in that range, in the same order.
		 * \tparam InputIterator Iterator type used to fill the container.
		 * \param first Input iterators to the initial positions in a range.
		 * \param last Input iterators to the final positions in a range.
		 * \param alloc Eventual allocator sample.
		 */
		template <class InputIterator>
		tape(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()):
		_alloc(alloc), _base(nullptr), _start(nullptr), _size(0), _capacity(0)
		{
			this->assign(first, last);
		}

		/** Copy constructor.
		 * Constructs a container with a copy of each of the elements in x, in the same order.
		 * \param x Another tape object of the same type (with the same class template arguments T and Alloc), whose contents are either copied or acquired.
		 */
		tape( const tape& x):
		_alloc(x._alloc), _base(nullptr), _start(nullptr), _size(0), _capacity(0)
		{
			this->assign(x.begin(), x.end());
		}

		/** Copy constructor.
		 * Constructs a container with a copy of each of the elements in x, in the same order.
		 * \param x Another tape object of the same type (with the same class template arguments T and Alloc), whose contents are either copied or acquired.
		 * \param alloc Eventual allocator sample.
		 */
		tape( const tape& x, const allocator_type& alloc):
		_alloc(alloc), _base(nullptr), _start(nullptr), _size(0), _capacity(0)
		{
			this->assign(x.begin(), x.end());
		}

		/** Move constructor. 
		 * Constructs the container with the contents of other using move semantics.
		 * Allocator is obtained by move-construction from the allocator belonging to other.
		 * After the move, other is guaranteed to be empty().
		 * \param other Another tape object to be used as source to initialize the elements of the container with.
		 */
		tape(tape&& other)
#if   __cplusplus >= 201703L // (since C++17)
			noexcept
#endif
		:_alloc(std::move(other._alloc)),
		_base(other._base), _start(other._start), _size(other._size), _capacity(other._capacity)
		{
			other._base = other._start = nullptr;
			other._size = other._capacity = 0;
		}

		/** Allocator-extended move constructor.
		 * \param other Another tape object to be used as source to initialize the elements of the container with.
		 * \param alloc Allocator sample.
		 */
		tape(tape&& other, const allocator_type& alloc):
		_alloc(alloc),
		_base(other._base), _start(other._start), _size(other._size), _capacity(other._capacity)
		{
			other._base = other._start = nullptr;
			other._size = other._capacity = 0;
		}


		/** Initializer list constructor.
		 * Constructs the container with the contents of the initializer list init.
		 * \param init Initializer list to initialize the elements of the container with.
		 * \param alloc Eventual allocator sample.
		 */
		tape(std::initializer_list<value_type> init, const Allocator& alloc = Allocator()):
		_alloc(alloc), _base(nullptr), _start(nullptr), _size(0), _capacity(0)
		{
			this->assign(init.begin(), init.end());
		}

		~tape()
		{
			this->_destroy_all();
			this->_deallocate();
		}

		/** Copy assignment.
		 * Assigns new contents to the container, replacing its current contents, and modifying its size accordingly.
		 * The container preserves its current allocator, which is used to allocate storage in case of reallocation.
		 * Any elements held in the container before the call are either assigned to or destroyed.
		 * \param x A tape object of the same type (i.e., with the same template parameters, T and Alloc).
		 */
		tape& operator=(const tape& x)
		{
			if(&x != this)
				this->assign(x.begin(), x.end());
			return *this;
		}

		/**
		 * Move assignment operator.
		 * Replaces the contents with those of other using move semantics.
		 * \param other Another tape to use as data source 
		 */
		tape& operator=(tape&& other)
#if   __cplusplus >= 201703L // (since C++17)
		noexcept // TODO Review this
#endif
		{
			_alloc = std::move(other._alloc);
			_base = other._base;
			_start = other._start;
			_size = other._size;
			_capacity = other._capacity;
			other._base = other._start = nullptr;
			other._size = other._capacity = 0;
			return *this;
		}

		/** Initializer list assignment operator
		 * Replaces the contents with those identified by initializer list ilist.
		 * \param ilist Initializer list to use as data source.
		 */
		tape& operator=( std::initializer_list<value_type> ilist )
		{
			this->assign(ilist.begin(), ilist.end());
			return *this;
		}
			
		/** \} */


		/**
		 * \name Iterators
		 * \{ */

		/** Returns an iterator pointing to the first element in the vector.
		 * Notice that, unlike member tape::front, which returns a reference to the first element,
		 * this function returns a random access iterator pointing to it.
		 * If the container is empty, the returned iterator value shall not be dereferenced.
		 * \return A random access iterator to the beginning of the sequence container.
		 */
		iterator begin() noexcept
		{return iterator(_start);}

		/** Returns a const iterator pointing to the first element in the vector.
		 * Notice that, unlike member tape::front, which returns a reference to the first element,
		 * this function returns a constant random access iterator pointing to it.
		 * If the container is empty, the returned iterator value shall not be dereferenced.
		 * \return A constant random access iterator to the beginning of the sequence container.
		 */
		const_iterator begin() const noexcept
		{return const_iterator(_start);}

		/** Returns a const iterator pointing to the first element in the vector.
		 * Notice that, unlike member tape::front, which returns a reference to the first element,
		 * this function returns a constant random access iterator pointing to it.
		 * If the container is empty, the returned iterator value shall not be dereferenced.
		 * \return A constant random access iterator to the beginning of the sequence container.
		 */
		const_iterator cbegin() const noexcept
		{return const_iterator(_start);}

		/** Returns an iterator referring to the past-the-end element in the tape container.
		 * The past-the-end element is the theoretical element that would follow the last element in the tape.
		 * It does not point to any element, and thus shall not be dereferenced.
		 * Because the ranges used by functions of the standard library do not include the element pointed by their closing iterator,
		 * this function is often used in combination with tape::begin to specify a range including all the elements in the container.
		 * If the container is empty, this function returns the same as tape::begin.
		 * \return A random access iterator to the element past the end of the sequence.
		 */
		iterator end() noexcept
		{return iterator(_start+_size);}
		
		/** Returns a const iterator referring to the past-the-end element in the tape container.
		 * The past-the-end element is the theoretical element that would follow the last element in the tape.
		 * It does not point to any element, and thus shall not be dereferenced.
		 * Because the ranges used by functions of the standard library do not include the element pointed by their closing iterator,
		 * this function is often used in combination with tape::begin to specify a range including all the elements in the container.
		 * If the container is empty, this function returns the same as tape::begin.
		 * \return A constant random access iterator to the element past the end of the sequence.
		 */
		const_iterator end() const noexcept
		{return const_iterator(_start+_size);}

		/** Returns a const iterator referring to the past-the-end element in the tape container.
		 * The past-the-end element is the theoretical element that would follow the last element in the tape.
		 * It does not point to any element, and thus shall not be dereferenced.
		 * Because the ranges used by functions of the standard library do not include the element pointed by their closing iterator,
		 * this function is often used in combination with tape::begin to specify a range including all the elements in the container.
		 * If the container is empty, this function returns the same as tape::begin.
		 * \return A constant random access iterator to the element past the end of the sequence.
		 */
		const_iterator cend() const noexcept
		{return const_iterator(_start+_size);}

		/** Returns a reverse iterator pointing to the last element in the tape (i.e., its reverse beginning).
		 * Reverse iterators iterate backwards: increasing them moves them towards the beginning of the container.
		 * rbegin points to the element right before the one that would be pointed to by member end.
		 * Notice that unlike member tape::back, which returns a reference to this same element, this function returns a reverse random access iterator.
		 * \return A reverse iterator to the reverse beginning of the sequence container.
		 */
		reverse_iterator rbegin() noexcept
		{return reverse_iterator(end());}

		/** Returns a const reverse iterator pointing to the last element in the tape (i.e., its reverse beginning).
		 * Reverse iterators iterate backwards: increasing them moves them towards the beginning of the container.
		 * rbegin points to the element right before the one that would be pointed to by member end.
		 * Notice that unlike member tape::back, which returns a reference to this same element, this function returns a reverse random access iterator.
		 * \return A constant reverse iterator to the reverse beginning of the sequence container.
		 */
		const_reverse_iterator rbegin() const noexcept
		{return const_reverse_iterator(end());}

		/** Returns a const reverse iterator pointing to the last element in the tape (i.e., its reverse beginning).
		 * Reverse iterators iterate backwards: increasing them moves them towards the beginning of the container.
		 * rbegin points to the element right before the one that would be pointed to by member end.
		 * Notice that unlike member tape::back, which returns a reference to this same element, this function returns a reverse random access iterator.
		 * \return A constant reverse iterator to the reverse beginning of the sequence container.
		 */
		const_reverse_iterator crbegin() const noexcept
		{return const_reverse_iterator(end());}

		/** Returns a reverse iterator pointing to the theoretical element preceding the first element in the tape (which is considered its reverse end).
		 * The range between tape::rbegin and tape::rend contains all the elements of the tape (in reverse order).
		 * \return A reverse iterator to the reverse end of the sequence container.
		 */
		reverse_iterator rend() noexcept
		{return reverse_iterator(begin());}

		/** Returns a const reverse iterator pointing to the theoretical element preceding the first element in the tape (which is considered its reverse end).
		 * The range between tape::rbegin and tape::rend contains all the elements of the tape (in reverse order).
		 * \return A constant reverse iterator to the reverse end of the sequence container.
		 */
		const_reverse_iterator rend() const noexcept
		{return const_reverse_iterator(begin());}

		/** Returns a const reverse iterator pointing to the theoretical element preceding the first element in the tape (which is considered its reverse end).
		 * The range between tape::crbegin and tape::crend contains all the elements of the tape (in reverse order).
		 * \return A constant reverse iterator to the reverse end of the sequence container.
		 */
		const_reverse_iterator crend() const noexcept
		{return const_reverse_iterator(begin());}
		/** \} */

		/**
		 * \name Capacity
		 * \{ */		

		/** Returns whether the vector is empty (i.e. whether its size is 0).
		 * This function does not modify the container in any way.
		 * To clear the content of a tape, see tape::clear.
		 * \return true if the container size is 0, false otherwise.
		 */
		bool empty() const noexcept
		{return this->_size==0;}
		
		/** Returns the number of elements in the tape.
		 * This is the number of actual objects held in the tape, which is not necessarily equal to its storage capacity.
		 * \return The number of elements in the container.
		 */
		size_type size() const noexcept
		{return this->_size;}
		
		/** Returns the maximum number of elements that the tape can hold.
		 * This is the maximum potential size the container can reach due to known system or library implementation limitations,
		 * but the container is by no means guaranteed to be able to reach that size: it can still fail to allocate storage at any point before that size is reached.
		 * \return The maximum number of elements a tape container can hold as content.
		 */
		size_type max_size() const noexcept
		{return std::allocator_traits<allocator_type>::max_size(_alloc);}

		/** Resizes the container so that it contains n elements.
		 * If n is smaller than the current container size, the content is reduced to its first n elements, removing those beyond (and destroying them).
		 * If n is greater than the current container size, the content is expanded by inserting at the end as many elements as needed to reach a size of n.
		 * If n is also greater than the current container capacity, an automatic reallocation of the allocated storage space takes place.
		 * Notice that this function changes the actual content of the container by inserting or erasing elements from it.
		 * \param new_size New container size, expressed in number of elements.
		 */
		void resize(size_type new_size)
		{
			if(new_size < size())
			{
				// Remove last elements
				erase(const_iterator(_start + new_size), end());
			}
			else
			{
				// Reserve enought memory
				reserve(new_size);
				// Copy enought default elements at the end of exising.
				while(size()<new_size)
				{
					*end() = value_type(); // TODO Optimize it !
					_size++;
				}
			}
		}

		/** Resizes the container so that it contains n elements.
		 * If n is smaller than the current container size, the content is reduced to its first n elements, removing those beyond (and destroying them).
		 * If n is greater than the current container size, the content is expanded by inserting at the end as many elements as needed to reach a size of n.
		 * The new elements are initialized as copies of val, otherwise, they are value-initialized.
		 * If n is also greater than the current container capacity, an automatic reallocation of the allocated storage space takes place.
		 * Notice that this function changes the actual content of the container by inserting or erasing elements from it.
		 * \param new_size New container size, expressed in number of elements.
		 * \param val Object whose content is copied to the added elements in case that n is greater than the current container size.
		 */
		void resize(size_type new_size, const value_type& val)
		{
			if(new_size < size())
			{
				// Remove last elements
				erase(const_iterator(_start + new_size), end());
			}
			else
			{
				// Reserve enought memory
				reserve(new_size);
				// Copy enought default elements at the end of exising.
				while(size()<new_size)
					push_back(val); // TODO Optimize it !
			}
		}

		/** Returns the size of the storage space currently allocated for the tape, expressed in terms of elements.
		 * This capacity is not necessarily equal to the tape size.
		 * It can be equal or greater, with the extra space allowing to accommodate for growth without the need to reallocate on each insertion.
		 * Notice that this capacity does not suppose a limit on the size of the tape.
		 * When this capacity is exhausted and more is needed, it is automatically expanded by the container (reallocating it storage space).
		 * The theoretical limit on the size of a tape is given by member max_size.
		 * \return The size of the currently allocated storage capacity in the tape, measured in terms of the number elements it can hold.
		 */
		size_type capacity() const noexcept
		{
			return _capacity;
		}

		/** Returns the size of the storage space currently allocated for the tape, available before the first element, expressed in terms of elements.
		 * This capacity is the number of elements which can be prepended without having to reallocate memory.
		 * When this capacity is exhausted and more is needed, it is automatically expanded by the container (reallocating it storage space).
		 * \return The size of the currently allocated storage capacity free to prepend elements in the tape, measured in terms of the number elements it can hold.
		 */
		size_type capacity_before() const noexcept
		{
			return _start - _base;
		}

		/** Returns the size of the storage space currently allocated for the tape, available after the last element, expressed in terms of elements.
		 * This capacity is the number of elements which can be append without having to reallocate memory.
		 * When this capacity is exhausted and more is needed, it is automatically expanded by the container (reallocating it storage space).
		 * \return The size of the currently allocated storage capacity free to append elements in the tape, measured in terms of the number elements it can hold.
		 */
		size_type capacity_after() const noexcept
		{
			return _capacity - ( capacity_before() + _size) ;
		}

		/** Request a change in capacity. */
		void reserve(size_type n)
		{
			reserve(capacity_before(), n);
		}
			
		/** Request a change in capacity. */
		void reserve(size_type before, size_type after)
		{
			if(capacity_before() < before || capacity_after() < after)
				_reallocate(before, after);
		}

		/** Request to reserve a capacity before used space. */
		void reserve_before(size_type before)
		{
			if(capacity_before() < before)
				_reallocate(before, capacity_after());
		}

		/** Request to reserve a capacity after used space. */
		void reserve_after(size_type after)
		{
			if(capacity_after() < after)
				_reallocate(capacity_before(), after);
		}

		/** Requests the container to reduce its capacity to fit its size.
		 * The request is non-binding, and the container implementation is free to optimize otherwise and leave the tape with a capacity greater than its size.
		 * This may cause a reallocation, but has no effect on the tape size and cannot alter its elements. */
		void shrink_to_fit()
		{
			_reallocate(0, 0);
		}

		/** \} */


		/**
		 * \name Element and data access
		 * \{ */			
		reference front() {return _start[0];}
		const_reference front() const {return _start[0];}
		reference back() {return _start[_size-1];}
		const_reference back() const {return  _start[_size-1];}
		reference operator[](size_type n) {return _start[n];}
		const_reference operator[](size_type n) const {return _start[n];}
		reference at(size_type n) {_check_range(n); return _start[n];}
		const_reference at(size_type n) const {_check_range(n); return _start[n];}
		pointer data() noexcept {return _start;}
		const_pointer data() const noexcept {return _start;}
		/** \} */



		/**
		 * \name Modifiers 
		 * \{ */			

		/** Assigns new contents to the tape, replacing its current contents, and modifying its size accordingly.*/
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last)
		{
			// Count the number of element to insert.
			size_type n = 0;
			for(InputIterator cur = first; cur != last; ++cur, ++n){}
			if(n==0) n = 1;

			// Destroy preceding elements if any
			_destroy_all();
			
			// Ensure tape has enought space and set _start ptr at optimal place
			if(_capacity < n)
			{
				_deallocate();
			}
			if(_capacity == 0)
			{
				_allocate(n);
			}
			else
			{
				_start = _base + ((_capacity - n) / 2);
			}

			// Copy elements
			_size = n;
			if(n == 1)
			{
				_construct(_start, *first);
			}
			else
			{
				pointer ptr = _start;
				while(first != last)
				{
					_construct(ptr++, *first++);
				}
			}
		}

		/** Assigns new contents to the tape, replacing its current contents, and modifying its size accordingly.*/
		void assign(size_type n, const value_type& val)
		{
			// Destroy preceding elements if any
			_destroy_all();
			
			// Ensure tape has enought space and set _start ptr at optimal place
			if(_capacity < n)
			{
				_deallocate();
			}
			if(_capacity == 0)
			{
				_allocate(n);
			}
			else
			{
				_start = _base + ((_capacity - n) / 2);
			}

			// Copy elements
			_size = n;
			pointer ptr = _start;
			while(n--)
			{
				_construct(ptr++, val);
			}
		}

		/**
		 * Replaces the contents of the container with the elements from the initializer list ilist.
		 * \param ilist Initializer list to copy the values from.
		 */
		void assign(std::initializer_list<value_type> ilist)
		{
			this->assign(ilist.begin(), ilist.end());
		}

		/** Adds a new element at the end of the tape, after its current last element. The content of val is copied to the new element. */
		void push_back(const value_type& val)
		{
			if(capacity_after() < 1)
				reserve_after(64); // TODO Compute reservation size smarter.
			_construct(_start+_size++, val);
		}

		/** Adds n new elements at the end of the tape, after its current last element. The content of val is copied to the new element. */
		void push_back(const value_type& val, size_type n)
		{
			if(capacity_after() < n)
				reserve_after(n+64); // TODO Compute reservation size smarter.
			while(n--)
				_construct(_start+_size++, val);
		}

		/** Adds new elements at the end of the tape, after its current last element. The content of val is copied to the new element. */
		template <class InputIterator>			
		void push_back(InputIterator first, InputIterator last)
		{
			for(;first != last; ++first)
			{
				if(capacity_after() < 1)
					reserve_after(64); // TODO Compute reservation size smarter.
				_construct(_start+(_size++), *first);
			}
		}

		/** Adds a new element at the end of the tape, after its current last element. The content of val is moved to the new element. */
		void push_back(value_type&& value)
		{
			if(capacity_after() < 1)
				reserve_after(64); // TODO Compute reservation size smarter.
			_construct(_start+_size++, std::move(value));
		}

		/** Adds a new element at the end of the tape, after its current last element. The new element is constructed emplace. */
		template<class... Args>
#if   __cplusplus < 201703L // (until C++17)
		void
#else // (since C++17)
		reference
#endif
		emplace_back(Args&&... args)
		{
			if(capacity_after() < 1)
				reserve_after(64); // TODO Compute reservation size smarter.
			_construct(_start+_size++, std::forward<Args>(args)...);
#if   __cplusplus >= 201703L // (since C++17)
			return back();
#endif
		}

		/** Removes the last element in the tape, effectively reducing the container size by one. */
		void pop_back()
		{
			if(_size>0)
			{
				_destroy(_start + --_size);
			}
		}

		/** Removes the last n elements in the tape, effectively reducing the container size by n. */
		void pop_back(size_type n)
		{
			while(_size>0 && n-->0)
			{
				_destroy(_start + --_size);
			}
		}

		/** Adds a new element at the begining of the tape, before its current first element. The content of val is copied to the new element. */
		void push_front(const value_type& val)
		{
			if(capacity_before() < 1)
				reserve_before(64); // TODO Compute reservation size smarter.
			_construct(--_start, val);
			++_size;
		}

		/** Adds n new elements at the begining of the tape, before its current first element. The content of val is copied to the new element. */
		void push_front(const value_type& val, size_type n)
		{
			if(capacity_before() < n)
				reserve_before(n + 64); // TODO Compute reservation size smarter.
			_size += n;
			while(n--)
				_construct(--_start, val);
		}

		/** Adds new elements at the begining of the tape, before its current first element. The content of val is copied to the new element. */
		template <class InputIterator>			
		void push_front(InputIterator first, InputIterator last)
		{
			// Count the number of element to copy.
			size_type n = 0;
			for(InputIterator cur = first; cur != last; ++cur, ++n){}
			if(n==0) n = 1;

			if(capacity_before() < 1)
				reserve_before(n + 64); // TODO Compute reservation size smarter.

			_start -= n;
			_size += n;

			pointer ptr = _start;
			while(n--)
			{
				_construct(ptr++, *(first++));
			}
		}

		/** Adds a new element at the begining of the tape, before its current first element. The content of val is moved to the new element. */
		void push_front(value_type&& value)
		{
			if(capacity_before() < 1)
				reserve_before(64); // TODO Compute reservation size smarter.
			_construct(--_start, std::move(value));
			++_size;
		}

		/** Adds a new element at the begining of the tape, before its current first element. The new element is constructed emplace. */
		template<class... Args>
#if   __cplusplus < 201703L // (until C++17)
		void
#else // (since C++17)
		reference
#endif
		emplace_front(Args&&... args)
		{
			if(capacity_before() < 1)
				reserve_before(64); // TODO Compute reservation size smarter.
			_construct(--_start, std::forward<Args>(args)...);
			++_size;
#if   __cplusplus >= 201703L // (since C++17)
			return front();
#endif
		}

		/** Removes the first element in the tape, effectively reducing the container size by one. */
		void pop_front()
		{
			if(_size>0)
			{
				_destroy(_start);
				++_start;
				--_size;
			}
		}

		/** Removes the first n elements in the tape, effectively reducing the container size by n. */
		void pop_front(size_type n)
		{
			while(_size>0 && n-->0)
			{
				_destroy(_start++);
				--_size;
			}
		}

		/** The tape is extended by inserting new elements before the element at the specified position, effectively increasing the container size by the number of elements inserted. */
		iterator insert(const_iterator position, const value_type& val)
		{
			// TODO Optimize me
			
			size_type pos = position - begin();
			
			// Ensure tape has enought memory
			reserve_before(1);

			// Move elements before insertion iterator
			_internal_move(_start-1, _start, _start + pos);
			--_start;
			++_size;

			// Insert element
			_construct(_start + pos, val);

			return iterator(_start + pos);
		}

		/** The tape is extended by inserting a new moved element before the element at the specified position, effectively increasing the container size by the number of elements inserted. */
		iterator insert(const_iterator position, value_type&& val)
		{
			// TODO Optimize me

			size_type pos = position - begin();

			// Ensure tape has enought memory
			reserve_before(1);

			// Move elements before insertion iterator
			_internal_move(_start-1, _start, _start + pos);
			--_start;
			++_size;

			// Insert element
			_construct(_start + pos, std::move(val));

			return iterator(_start + pos);
		}

		/** The tape is extended by inserting a new constructed element before the element at the specified position, effectively increasing the container size by the number of elements inserted. The element is created emplaced.*/
		template< class... Args >
		iterator emplace(const_iterator position, Args&&... args)
		{
			// TODO Optimize me

			size_type pos = position - begin();

			// Ensure tape has enought memory
			reserve_before(1);

			// Move elements before insertion iterator
			_internal_move(_start-1, _start, _start + pos);
			--_start;
			++_size;

			// Insert element
			_construct(_start + pos, std::forward<Args>(args)...);

			return iterator(_start + pos);
		}


		/** The tape is extended by inserting new elements before the element at the specified position, effectively increasing the container size by the number of elements inserted. */
		iterator insert (const_iterator position, size_type count, const value_type& val)
		{
			// TODO Optimize me

			size_type pos = position - begin();

			// Ensure tape has enought memory
			reserve_before(count);

			// Move elements before insertion iterator
			_internal_move(_start-count, _start, _start + pos);
			_start -= count;
			_size  += count;

			// Insert elements
			for(pointer ptr = _start + pos; ptr < _start + pos + count; ++ptr)
				_construct(ptr, val);

			return iterator(_start + pos);
		}

		/** The tape is extended by inserting new elements before the element at the specified position, effectively increasing the container size by the number of elements inserted. */
		template <class InputIterator>
		iterator insert (const_iterator position, InputIterator first, InputIterator last)
		{
			// TODO Optimize me

			size_type pos = position - begin();

			// Count the number of element to copy.
			size_type n = 0;
			for(InputIterator cur = first; cur != last; ++cur, ++n){}
			if(n>0)
			{
				// Ensure tape has enought memory
				reserve_before(n);

				// Move elements before insertion iterator
				_internal_move(_start-n, _start, _start + pos);
				_start -= n;
				_size  += n;
				
				// Insert elements
				for(pointer ptr = _start + pos; first != last; ++ptr, first++)
					_construct(ptr, *first);
			}
			return iterator(_start + pos);
		}

		/** Inserts elements from initializer list ilist before pos.*/
		iterator insert(const_iterator position, std::initializer_list<value_type> ilist)
		{
			return this->insert(position, ilist.begin(), ilist.end());
		}

		/** Removes element from the tape. */
		iterator erase(const_iterator position)
		{
			// TODO Optimize me by testing if erased element is first or last one.

			pointer ptr = position.get_ptr();

			// Destroy erased element
			_destroy(ptr);
			
			// Move content
			_internal_move(ptr, ptr+1, end().get_ptr());

			--_size;

			return iterator(ptr);
		}

		/** Removes range of elements ([first,last)) from the tape. */
		iterator erase(const_iterator first, const_iterator last)
		{
			// TODO Optimize me by testing if erased elements are first or last ones.
			if(first != last)
			{
				size_type nb = last - first;

				// Destroy all erased elements
				for(const_iterator cur = first; cur != last; ++cur)
					_destroy(cur.get_ptr());

				// Move content
				_internal_move(first.get_ptr(), last.get_ptr(), _start + _size);

				_size -= nb;
			}
			return iterator(first.get_ptr());
		}

		/** Exchanges the content of the container by the content of x, which is another tape object of the same type. Sizes may differ.*/
		void swap(tape& x)
#if   __cplusplus >= 201703L // (since C++17)
		noexcept // TODO Review this
#endif
		{
			std::swap(_alloc,    x._alloc);
			std::swap(_base,     x._base);
			std::swap(_start,    x._start);
			std::swap(_size,     x._size);
			std::swap(_capacity, x._capacity);
		}

		/** Removes all elements from the tape (which are destroyed), leaving the container with a size of 0. */
		void clear() noexcept
		{
			_destroy_all();
		}

		/** \} */


		/**
		 * \name Allocator 
		 * \{ */
		/** Returns a copy of the allocator object associated with the tape. */
		allocator_type get_allocator() const
		{
			return _alloc;
		}
			
		/** \} */

		// TODO Add external relational operators.

	private:
			
		/** Check if there is enought allocated memory and throw except if not. Used by tape::at(). */ 
		void _check_range(size_type n) const
		{
			//If n is out of range, throw an out_of_range exception
			if (n >= size())
				throw std::out_of_range("tape::at");
		}

		/** Allocate memory and set pointers to first third of it.
		 * Assume no memory is allocated.
		 */
		void _allocate(size_type size)
		{
			_capacity = 3*size;
			_base  = std::allocator_traits<allocator_type>::allocate(_alloc, _capacity);
			_start = _base + size;
			_size  = 0;
		}

		/** Release allocated memory. Assume no element is assigned. */
		void _deallocate()
		{
			if(_capacity)
			{
				std::allocator_traits<allocator_type>::deallocate(_alloc, _base, _capacity);
				_base = _start = nullptr;
				_capacity = _size = 0;
			}
		}

		/** Construct an element. */
		template<class... Args>
		void _construct(value_type* p, Args&&... args)
		{
			std::allocator_traits<allocator_type>::construct(_alloc, p, std::forward<Args>(args)...);
		}

		/** Destroy an element. */
		void _destroy(value_type* p)
		{
			std::allocator_traits<allocator_type>::destroy(_alloc, p);
		}
			
		/** Destroy n contigous elements. */
		void _destroy_n(value_type* p, size_type n)
		{
			for(;n--;++p)
				std::allocator_traits<allocator_type>::destroy(_alloc, p);
		}
			
		/** Destroy all elements in the tape and set size to 0. */
		void _destroy_all()
		{
			_destroy_n(_start, _size);
			_size = 0;
			// TODO Reset start pointer to middle of allocated space ?
		}

		/** Move elements from a place to another. No allocation is done. */
		void _internal_move(pointer dst, pointer src, size_type n = 1)
		{
			while(n--)
			{
				// TODO Optimize this (use of std::uninitialized_copy ?)
				_construct(dst++, *src);
				std::allocator_traits<allocator_type>::destroy(_alloc, src++);
			}
		}

		/** Move elements from a place to another. No allocation is done. */
		void _internal_move(pointer dst, pointer src_begin, pointer src_end)
		{
			while(src_begin!=src_end)
			{
				// TODO Optimize this (use of std::uninitialized_copy ?)
				_construct(dst++, *src_begin);
				std::allocator_traits<allocator_type>::destroy(_alloc, src_begin++);
			}
		}

		/** Reallocate content in new memory with specified extra slots. */
		void _reallocate(size_type before, size_type after)
		{
			size_type capa = before + after + _size;
			
			// Allocate new memory
			pointer mem = capa>0 ? std::allocator_traits<allocator_type>::allocate(_alloc, capa, _base) : nullptr;
			
			// Move existing elements
			_internal_move(mem+before, _start, _size);

			
			// Release old memory
			if(_base)
				std::allocator_traits<allocator_type>::deallocate(_alloc, _base, _capacity);

			// Reset pointers
			_base     = mem;
			_start    = mem + before;
			_capacity = capa;
			// _size is unchanged			
		}

	};

	template <class T, class Allocator>
	inline void swap(tape<T, Allocator>& x, tape<T, Allocator>& y)
	{  x.swap(y);  }
	
} // namespace container
