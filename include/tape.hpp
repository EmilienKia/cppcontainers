/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Émilien KIA nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Émilien KIA AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL Émilien KIA OR ANY OTHER
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstddef>
#include <memory>
#include <algorithm>
#include <stdexcept>
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
		tape_iterator():_ptr(0){}
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
		tape_const_iterator():_ptr(0){}
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
	 * Tape container template class.
	 *
	 * A tape is a vector but with constant front inserting complexity.
	 */
	template <typename T, typename Allocator = std::allocator<T> >
	class tape
	{
		typedef Allocator base_t;
	public:
		/**
		 * 
		 * STL type definitions:
		 * 
		 * \{ */
		typedef T											value_type;
		typedef Allocator									allocator_type;

		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;

		typedef tape_iterator<T>							iterator;
		typedef tape_const_iterator<T>						const_iterator;
		typedef std::reverse_iterator<iterator>				reverse_iterator;
		typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;
			
		typedef ptrdiff_t									difference_type;
		typedef size_t										size_type;
		/** \} */
			
	protected:
		allocator_type _alloc;  // Allocator
		pointer		_base;		// Base allocated memory chunck
		pointer		_start;		// Begining of used memory (first element)
		size_type   _size;		// Number of used elements
		size_type	_capacity;  // Size of allocated memory in nb of elements

	public:
		/**
		 * Construct / Copy / Destroy
		 * \{ */

		/** Default constructor. */
		explicit tape(const allocator_type& alloc = allocator_type()):
		_alloc(alloc), _base(0), _start(0), _size(0), _capacity(0)
		{}

		/** Filling constructor. */
		explicit tape(size_type n):
		_alloc(), _base(0), _start(0), _size(0), _capacity(0)
		{
			this->resize(n);
		}

		/** Filling constructor. */
		explicit tape(size_type n, const value_type& val, const allocator_type& alloc = allocator_type()):
		_alloc(alloc), _base(0), _start(0), _size(0), _capacity(0)
		{
			this->resize(n, val);
		}

		/** Range constructor. */
		template <class InputIterator>
		tape(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()):
		_alloc(alloc), _base(0), _start(0), _size(0), _capacity(0)
		{
			this->assign(first, last);
		}

		/** Copy constructor. */
		tape( const tape& x):
		_alloc(x._alloc), _base(0), _start(0), _size(0), _capacity(0)
		{
			this->assign(x.begin(), x.end());
		}

		/** Copy constructor. */
		tape( const tape& x, const allocator_type& alloc):
		_alloc(alloc), _base(0), _start(0), _size(0), _capacity(0)
		{
			this->assign(x.begin(), x.end());
		}

		// TODO Add "move" and initializer list constroctors.

		~tape()
		{
			this->_destroy_all();
			this->_deallocate();
		}

		/** Copy assignment. */
		tape& operator=(const tape& x)
		{
			if(&x != this)
				this->assign(x.begin(), x.end());
		}

		// TODO Add "move" and initializer list assignment operators.
			
		/** \} */


		/**
		 * Iterators
		 * \{ */

		iterator begin() {return iterator(_start);}
		const_iterator begin()const {return const_iterator(_start);}
		const_iterator cbegin()const {return const_iterator(_start);}

		iterator end() {return iterator(_start+_size);}
		const_iterator end()const {return const_iterator(_start+_size);}
		const_iterator cend()const {return const_iterator(_start+_size);}

		reverse_iterator rbegin() {return reverse_iterator(end());}
		const_reverse_iterator rbegin()const {return const_reverse_iterator(end());}
		const_reverse_iterator crbegin()const {return const_reverse_iterator(end());}

		reverse_iterator rend() {return reverse_iterator(begin());}
		const_reverse_iterator rend()const {return const_reverse_iterator(begin());}
		const_reverse_iterator crend()const {return const_reverse_iterator(begin());}		

		/** \} */

		/**
		 * Capacity
		 * \{ */		

		bool empty()const {return this->_size==0;}
		size_type size()const {return this->_size;}
		size_type max_size() const {return _alloc.max_size();}

		/** Resizes the container so that it contains n elements. */
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

		/** Resizes the container so that it contains n elements. */
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

		/** Return size of allocated storage capacity. */
		size_type capacity() const
		{
			return _capacity;
		}

		/** Return size of storage available memory before used space. */
		size_type capacity_before() const
		{
			return _start - _base;
		}

		/** Return size of storage available memory after used space. */
		size_type capacity_after() const
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

		/** Requests the container to reduce its capacity to fit its size. */
		void shrink_to_fit()
		{
			_reallocate(0, 0);
		}

		/** \} */


		/**
		 * Element and data access
		 * \{ */			

		reference front() {return _start[0];}
		const_reference front()const {return _start[0];}
		reference back() {return _start[_size-1];}
		const_reference back()const {return  _start[_size-1];}
		reference operator[](size_type n) {return _start[n];}
		const_reference operator[](size_type n)const {return _start[n];}
		reference at(size_type n) {_check_range(n); return _start[n];}
		const_reference at(size_type n)const {_check_range(n); return _start[n];}
		pointer data() {return _start;}
		const_pointer data()const {return _start;}

		/** \} */



		/**
		 * Modifiers 
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
				_alloc.deallocate(_base, _capacity);
				_capacity = 0;
				_base     = 0;
			}
			if(_capacity == 0)
			{
				_capacity = 3*n;
				_base  = _alloc.allocate(_capacity);
				_start = _base + n;
				_size  = 0;
			}
			else
			{
				_start = _base + ((_capacity - n) / 2);
			}

			// Copy elements
			_size = n;
			if(n == 1)
			{
				_alloc.construct(_start, *first);
			}
			else
			{
				pointer ptr = _start;
				while(first != last)
				{
					_alloc.construct(ptr++, *first++);
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
				_alloc.deallocate(_base, _capacity);
				_capacity = 0;
				_base     = 0;
			}
			if(_capacity == 0)
			{
				_capacity = 3*n;
				_base  = _alloc.allocate(_capacity);
				_start = _base + n;
				_size  = 0;
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
				_alloc.construct(ptr++, val);
			}
		}

		// TODO Add assign with initializer list

		/** Adds a new element at the end of the tape, after its current last element. The content of val is copied (or moved) to the new element. */
		void push_back(const value_type& val)
		{
			if(capacity_after() < 1)
				reserve_after(64); // TODO Compute reservation size smarter.
			_alloc.construct(_start+_size++, val);
		}

		/** Adds n new elements at the end of the tape, after its current last element. The content of val is copied (or moved) to the new element. */
		void push_back(const value_type& val, size_type n)
		{
			if(capacity_after() < n)
				reserve_after(n+64); // TODO Compute reservation size smarter.
			while(n--)
				_alloc.construct(_start+_size++, val);
		}

		/** Adds new elements at the end of the tape, after its current last element. The content of val is copied (or moved) to the new element. */
		template <class InputIterator>			
		void push_back(InputIterator first, InputIterator last)
		{
			for(;first != last; ++first)
			{
				if(capacity_after() < 1)
					reserve_after(64); // TODO Compute reservation size smarter.
				_alloc.construct(_start+(_size++), *first);				
			}
		}

		// TODO add push_back with move semantic

		/** Removes the last element in the tape, effectively reducing the container size by one. */
		void pop_back()
		{
			if(_size>0)
			{
				_alloc.destroy(_start + --_size);
			}
		}

		/** Removes the last n elements in the tape, effectively reducing the container size by n. */
		void pop_back(size_type n)
		{
			while(_size>0 && n-->0)
			{
				_alloc.destroy(_start + --_size);
			}
		}

		/** Adds a new element at the begining of the tape, before its current first element. The content of val is copied (or moved) to the new element. */
		void push_front(const value_type& val)
		{
			if(capacity_before() < 1)
				reserve_before(64); // TODO Compute reservation size smarter.
			_alloc.construct(--_start, val);
			++_size;
		}

		/** Adds n new elements at the begining of the tape, before its current first element. The content of val is copied (or moved) to the new element. */
		void push_front(const value_type& val, size_type n)
		{
			if(capacity_before() < n)
				reserve_before(n + 64); // TODO Compute reservation size smarter.
			_size += n;
			while(n--)
				_alloc.construct(--_start, val);
		}

		/** Adds new elements at the begining of the tape, before its current first element. The content of val is copied (or moved) to the new element. */
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
				_alloc.construct(ptr++, *(first++));
			}
		}

		// TODO add push_front with move semantic

		/** Removes the first element in the tape, effectively reducing the container size by one. */
		void pop_front()
		{
			if(_size>0)
			{
				_alloc.destroy(_start);
				++_start;
				--_size;
			}
		}

		/** Removes the first n elements in the tape, effectively reducing the container size by n. */
		void pop_front(size_type n)
		{
			while(_size>0 && n-->0)
			{
				_alloc.destroy(_start++);
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
			_alloc.construct(_start + pos, val);

			return iterator(_start + pos);
		}

		/** The tape is extended by inserting new elements before the element at the specified position, effectively increasing the container size by the number of elements inserted. */
		iterator insert (const_iterator position, size_type n, const value_type& val)
		{
			// TODO Optimize me

			size_type pos = position - begin();

			// Ensure tape has enought memory
			reserve_before(n);

			// Move elements before insertion iterator
			_internal_move(_start-n, _start, _start + pos);
			_start -= n;
			_size  += n;

			// Insert elements
			for(pointer ptr = _start + pos; ptr < _start + pos + n; ++ptr)
				_alloc.construct(ptr, val);
			
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
					_alloc.construct(ptr, *first);
			}
			return iterator(_start + pos);
		}

		// TODO Add insert with move and initializer_list

		/** Removes element from the tape. */
		iterator erase(const_iterator position)
		{
			// TODO Optimize me by testing if erased element is first or last one.

			pointer ptr = position.get_ptr();

			// Destroy erased element
			_alloc.destroy(ptr);
			
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
					_alloc.destroy(cur.get_ptr());

				// Move content
				_internal_move(first.get_ptr(), last.get_ptr(), _start + _size);

				_size -= nb;
			}
			return iterator(first.get_ptr());
		}

		/** Exchanges the content of the container by the content of x, which is another tape object of the same type. Sizes may differ.*/
		void swap(tape& x)
		{
			std::swap(_alloc,    x._alloc);
			std::swap(_base,     x._base);
			std::swap(_start,    x._start);
			std::swap(_size,     x._size);
			std::swap(_capacity, x._capacity);
		}

		/** Removes all elements from the tape (which are destroyed), leaving the container with a size of 0. */
		void clear()
		{
			_destroy_all();
		}

		// TODO add emplace emplace_back and emplace_front.

		/** \} */


		/**
		 * Allocator 
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

		/** Release allocated memory. Assume no element is assigned. */
		void _deallocate()
		{
			if(_capacity)
			{
				_alloc.deallocate(_base, _capacity);
				_base = _start = 0;
				_capacity = _size = 0;
			}
		}
			
		/** Destroy an element. */
		void _destroy(value_type* p)
		{
			_alloc.destroy(p);
		}
			
		/** Destroy n contigous elements. */
		void _destroy_n(value_type* p, size_type n)
		{
			for(;n--;++p)
				_alloc.destroy(p);
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
				_alloc.construct(dst++, *src);
				_alloc.destroy(src++);
			}
		}

		/** Move elements from a place to another. No allocation is done. */
		void _internal_move(pointer dst, pointer src_begin, pointer src_end)
		{
			while(src_begin!=src_end)
			{
				_alloc.construct(dst++, *src_begin);
				_alloc.destroy(src_begin++);
			}
		}

		/** Reallocate content in new memory with specified extra slots. */
		void _reallocate(size_type before, size_type after)
		{
			size_type capa = before + after + _size;
			
			// Allocate new memory
			pointer mem = capa>0 ? _alloc.allocate(capa, _base) : NULL;
			
			// Move existing elements
			_internal_move(mem+before, _start, _size);

			
			// Release old memory
			if(_base)
				_alloc.deallocate(_base, _capacity);

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
