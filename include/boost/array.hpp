/* The following code declares class array,
 * an STL container (as wrapper) for arrays of constant size.
 *
 * See
 *      http://www.boost.org/libs/array/
 * for documentation.
 *
 * The original author site is at: http://www.josuttis.com/
 *
 * (C) Copyright Nicolai M. Josuttis 2001.
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 *  9 Jan 2013 - (mtc) Added constexpr
 * 14 Apr 2012 - (mtc) Added support for boost::hash
 * 28 Dec 2010 - (mtc) Added cbegin and cend (and crbegin and crend) for C++Ox compatibility.
 * 10 Mar 2010 - (mtc) fill method added, matching resolution of the standard library working group.
 *      See <http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#776> or Trac issue #3168
 *      Eventually, we should remove "assign" which is now a synonym for "fill" (Marshall Clow)
 * 10 Mar 2010 - added workaround for SUNCC and !STLPort [trac #3893] (Marshall Clow)
 * 29 Jan 2004 - c_array() added, BOOST_NO_PRIVATE_IN_AGGREGATE removed (Nico Josuttis)
 * 23 Aug 2002 - fix for Non-MSVC compilers combined with MSVC libraries.
 * 05 Aug 2001 - minor update (Nico Josuttis)
 * 20 Jan 2001 - STLport fix (Beman Dawes)
 * 29 Sep 2000 - Initial Revision (Nico Josuttis)
 * 18 Nov 2021 - Added Javadocs (Alan de Freitas)
 *
 * Jan 29, 2004
 */
#ifndef BOOST_ARRAY_HPP
#define BOOST_ARRAY_HPP

#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)
# pragma warning(push)
# pragma warning(disable:4996) // 'std::equal': Function call with parameters that may be unsafe
# pragma warning(disable:4510) // boost::array<T,N>' : default constructor could not be generated
# pragma warning(disable:4610) // warning C4610: class 'boost::array<T,N>' can never be instantiated - user defined constructor required
#endif

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/swap.hpp>

#include <boost/throw_exception.hpp>
#include <algorithm>

// FIXES for broken compilers
#include <boost/config.hpp>


namespace boost {

    /** A container that encapsulates fixed size arrays

        This class represents a container that encapsulates fixed size arrays.

        It has the same semantics as a struct holding a C-style array `T[N]`.
        Unlike a C-style array, it doesn't decay to T* automatically.

        The class combines the performance of a C-style array with the
        convenience of a standard container.

        There is a special case for a zero-length array (N == 0). In that case,
        array.begin() == array.end(), which is some unique value.


        @par Thread Safety

        Not thread safe

        @tparam T The array value type
        @tparam N The array size

        @see @ref std::vector

    */
    template<class T, std::size_t N>
    class array {
      public:
        /// Underlying fixed-size array of elements of type T
        T elems[N];

      public:
        /// Member group: Types

        /// The array value type
        typedef T              value_type;

        /// Iterator to C-style array, i.e. a pointer
        typedef T*             iterator;

        /// Constant iterator to array, i.e. a pointer
        typedef const T*       const_iterator;

        /// Reference to value type
        typedef T&             reference;

        /// Constant reference to value type
        typedef const T&       const_reference;

        /// Type used to represent array size
        typedef std::size_t    size_type;

        /// Type used to represent distance between iterators
        typedef std::ptrdiff_t difference_type;


        /// Member group: Iterators

        /** Returns an iterator to the beginning

            This function returns an iterator to the first element of the array.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator will be equal to end()

            @return Iterator to the first element of the array

            @see @ref end

        */
        iterator        begin()       { return elems; }

        /** Returns a constant iterator to the beginning

            This function returns a constant iterator to the first element of
            the array. This is the overload that gets called when `this` is
            constant.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator will be equal to end()

            @return Iterator to the first element of the array

            @see @ref end

        */
        const_iterator  begin() const { return elems; }

        /** Returns a constant iterator to the beginning

            This function returns a constant iterator to the first element of
            the array.

            This function is usually called to provide a constant iterator
            when `this` is not constant.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator will be equal to end()

                @return Iterator to the first element of the array

            @see @ref end

        */
        const_iterator cbegin() const { return elems; }


        /** Returns an iterator to the end

            This function returns an iterator to the element following the
            last element of the array

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator will be equal to end()

            @return Iterator to the element following the last element of the array

            @see @ref begin

        */
        iterator        end()       { return elems+N; }

        /** Returns a constant iterator to the end

            This function returns a constant iterator to the first element of
            the array.

            This function overload gets called when `this` is constant.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator will be equal to end()

            @return Iterator to the element following the last element of the array

            @see @ref begin

        */
        const_iterator  end() const { return elems+N; }

        /** Returns a constant iterator to the end

            This function returns a constant iterator to the element following
            the last element of the array.

            This function is usually called to provide a constant iterator
            when `this` is not constant.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator will be equal to end()

            @return Iterator to the element following the last element of the array

            @see @ref cbegin

        */
        const_iterator cend() const { return elems+N; }


        /// Member group: Types

        // reverse iterator support
#if !defined(BOOST_MSVC_STD_ITERATOR) && !defined(BOOST_NO_STD_ITERATOR_TRAITS)
        /// Reverse array iterator - std::reverse_iterator<iterator>
        typedef std::reverse_iterator<iterator> reverse_iterator;

        /// Constant reverse array iterator - std::reverse_iterator<const_iterator>
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#elif defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
        typedef std::reverse_iterator<iterator, std::random_access_iterator_tag,
              value_type, reference, iterator, difference_type> reverse_iterator;
        typedef std::reverse_iterator<const_iterator, std::random_access_iterator_tag,
              value_type, const_reference, const_iterator, difference_type> const_reverse_iterator;
#else
        // workaround for broken reverse_iterator implementations
        typedef std::reverse_iterator<iterator,T> reverse_iterator;
        typedef std::reverse_iterator<const_iterator,T> const_reverse_iterator;
#endif


        /// Member group: Iterators

        /** Returns a reverse iterator to the beginning
        
            Returns a reverse iterator to the first element of the reversed
            array.

            This reverse iterator corresponds to the last element of the
            non-reversed array.
        
            @par Complexity
        
            Constant
            
            @note If the array is empty, the returned iterator is equal to rend().
        
            @return Reverse iterator to the first element of the reversed array
        
            @see @ref rend
        
        */
        reverse_iterator rbegin() { return reverse_iterator(end()); }

        /** Returns a constant reverse iterator to the beginning

            Returns a reverse iterator to the first element of the reversed
            array.

            This reverse iterator corresponds to the last element of the
            non-reversed array.

            This function overload gets called when `this` is constant.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator is equal to rend().

            @return Reverse iterator to the first element of the reversed array

            @see @ref rend

        */
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        /** Returns a reverse iterator to the beginning

            Returns a reverse iterator to the first element of the reversed
            array.

            This reverse iterator corresponds to the last element of the
            non-reversed array.

            This function is usually called to provide a constant iterator
            when `this` is not constant.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator is equal to rend().

            @return Reverse iterator to the first element of the reversed array

            @see @ref rend

        */
        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(end());
        }

        /** Returns a reverse iterator to the end

            Returns a reverse iterator to the element following the last
            element of the reversed array.

            This iterator corresponds to the element preceding the first
            element of the non-reversed array.

            This element acts as a placeholder. Attempting to access it
            results in undefined behavior.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator is equal to rend().

            @return Reverse iterator to the first element of the reversed array

            @see @ref rend

        */
        reverse_iterator rend() { return reverse_iterator(begin()); }

        /** Returns a constant reverse iterator to the end

            Returns a reverse iterator to the element following the last
            element of the reversed array.

            This iterator corresponds to the element preceding the first
            element of the non-reversed array.

            This element acts as a placeholder. Attempting to access it
            results in undefined behavior.

            This function overload gets called when `this` is constant.

            @par Complexity

            Constant

            @note If the array is empty, the returned iterator is equal to rend().

            @return Reverse iterator to the first element of the reversed array

            @see @ref rend

        */
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

        /** Returns a constant reverse iterator to the end

            Returns a reverse iterator to the element following the last
            element of the reversed array.

            This iterator corresponds to the element preceding the first
            element of the non-reversed array.

            This element acts as a placeholder. Attempting to access it
            results in undefined behavior.

            This function is usually called to provide a constant iterator
            when `this` is not constant.


            @par Complexity

            Constant

            @note If the array is empty, the returned iterator is equal to rend().

            @return Reverse iterator to the first element of the reversed array

            @see @ref rend

        */
        const_reverse_iterator crend() const {
            return const_reverse_iterator(begin());
        }


        /// Member group: Element access

        /** Access specified element
        
            This function Returns a reference to the element at specified
            location @ref i.

            No bounds checking is performed.

            @par Complexity
        
            Constant
            
            @par Preconditions

            !(@ref i < size())
        
            @note Unlike std::map::operator[], this operator never inserts a new
            element into the container.

            Accessing a nonexistent element through this operator is undefined
            behavior.

            @return Reference to the requested element.
        
            @param i Position of the element to return
            
            @see @ref at
        
        */
        reference operator[](size_type i)
        {
            return BOOST_ASSERT_MSG( i < N, "out of range" ), elems[i];
        }

        /** Access specified element

            This function Returns a reference to the element at specified
            location @ref i.

            No bounds checking is performed.

            This overload gets called when `this` is constant.

            @par Complexity

            Constant

            @par Preconditions

            !(@ref i < size())

            @note Unlike std::map::operator[], this operator never inserts a new
            element into the container.

            Accessing a nonexistent element through this operator is undefined
            behavior.

            @return Reference to the requested element.

            @param i Position of the element to return

            @see @ref at

        */
        /*BOOST_CONSTEXPR*/ const_reference operator[](size_type i) const
        {
            return BOOST_ASSERT_MSG( i < N, "out of range" ), elems[i];
        }

        /** Access specified element with bounds checking

            This function Returns a reference to the element at specified
            location @ref i, with bounds checking.

            If pos is not within the range of the container, an exception of
            type `std::out_of_range` is thrown.

            @par Complexity

            Constant

            @par Preconditions

            !(@ref i < size())

            @par Exception Safety

            `std::out_of_range` if !(@ref i < size()).

            @return Reference to the requested element.

            @param i Position of the element to return

            @see @ref operator[]

        */
        reference                           at(size_type i)       { return rangecheck(i), elems[i]; }

        /** Access specified element with bounds checking

            This function Returns a reference to the element at specified
            location @ref i, with bounds checking.

            If pos is not within the range of the container, an exception of
            type `std::out_of_range` is thrown.

            This overload gets called when `this` is constant.

            @par Complexity

            Constant

            @par Preconditions

            !(@ref i < size())

            @par Exception Safety

            `std::out_of_range` if !(@ref i < size()).

            @return Reference to the requested element.

            @param i Position of the element to return

            @see @ref operator[]

        */
        /*BOOST_CONSTEXPR*/ const_reference at(size_type i) const { return rangecheck(i), elems[i]; }

        /** Access the first array element

            This function returns a reference to the first element in the
            container.

            Calling front on an empty container is undefined.

            @par Complexity

            Constant

            @note `c.front()` is equivalent to `*c.begin()`

            @par Preconditions

            !(empty())

            @return Reference to the first array element.

            @see @ref operator[], @ref at, @ref begin, @ref end

        */
        reference front()
        {
            return elems[0];
        }

        /** Access the first array element

            This function returns a reference to the first element in the
            container.

            Calling front on an empty container is undefined.

            This overload gets called when `this` is constant.

            @par Complexity

            Constant

            @note `c.front()` is equivalent to `*c.begin()`

            @par Preconditions

            !(empty())

            @return Reference to the first array element.

            @see @ref operator[], @ref at, @ref begin, @ref end

        */
        BOOST_CONSTEXPR const_reference front() const
        {
            return elems[0];
        }

        /** Access the last array element

            This function returns a reference to the last element in the
            container.

            Calling front on an empty container is undefined.

            @par Complexity

            Constant

            @note `c.back()` is equivalent to `*std::prev(c.end())`

            @par Preconditions

            !(empty())

            @return Reference to the last array element.

            @see @ref operator[], @ref at, @ref begin, @ref end

        */
        reference back()
        {
            return elems[N-1];
        }

        /** Access the last array element

            This function returns a reference to the last element in the
            container.

            Calling front on an empty container is undefined.

            This overload gets called when `this` is constant.

            @par Complexity

            Constant

            @note `c.back()` is equivalent to `*std::prev(c.end())`

            @par Preconditions

            !(empty())

            @return Reference to the last array element.

            @see @ref operator[], @ref at, @ref begin, @ref end

        */
        BOOST_CONSTEXPR const_reference back() const
        {
            return elems[N-1];
        }


        /// Member group: Capacity


        /** Returns the number of elements

            This function returns the number of elements in the container.

            @par Complexity

            Constant

            @note `size()` is equivalent to `std::distance(begin(), end())`

            @return The number of elements in the container

            @see @ref empty, @ref max_size

        */
        static BOOST_CONSTEXPR size_type size() { return N; }

        /** Checks whether the container is empty

            This function checks whether the container is empty

            @par Complexity

            Constant

            @note `empty()` is equivalent to `begin() == end()`

            @return `true` if the container is empty, `false` otherwise

            @see @ref size, @ref max_size

        */
        static BOOST_CONSTEXPR bool empty() { return false; }

        /** Returns the maximum possible number of elements

            Returns the maximum number of elements the container is able to
            hold due to system or library implementation limitations

            @par Complexity

            Constant

            @note Because array<T, N> is a fixed-size container, the value returned
            by `max_size()` is equivalent to `size()`

            @return Maximum number of elements

            @see @ref size, @ref empty

        */
        static BOOST_CONSTEXPR size_type max_size() { return N; }



#ifndef BOOST_ARRAY_DOCS
        /// Internal enum with static array size
        enum { static_size = N };
#endif // BOOST_ARRAY_DOCS

        /// Member group: Swap


        /** Swaps the container contents

            Exchanges the contents of the container with those of other.

            This function does not cause iterators and references to associate
            with the other container.

            @par Complexity

            Linear

            @note Because array<T, N> is not dynamically allocated, this operation
            has linear complexity.

            @return (none)

            @param y container to exchange the contents with

            @see @ref fill, `std::swap`

        */
        void swap (array<T,N>& y) {
            for (size_type i = 0; i < N; ++i)
                boost::swap(elems[i],y.elems[i]);
        }


        /// Member group: Direct access to data


        /** Direct access to the underlying array

            Returns pointer to the underlying array serving as element storage.

            This overload gets called when `this` is constant.

            @par Complexity

            Constant

            @note The pointer is such that range [data(); data() + size()) is always
            a valid range, even if the container is empty.

            When the container is empty, data() is not dereferenceable.

            If size() is 0, data() may or may not return a null pointer.

            @return Pointer to the underlying element storage

            For non-empty containers, the returned pointer compares equal
            to the address of the first element.

            @see @ref front, @ref back, @ref size

        */
        const T* data() const { return elems; }

        /** Direct access to the underlying array

            Returns pointer to the underlying array serving as element storage.

            @par Complexity

            Constant

            @note The pointer is such that range [data(); data() + size()) is always
            a valid range, even if the container is empty.

            When the container is empty, data() is not dereferenceable.

            If size() is 0, data() may or may not return a null pointer.

            @return Pointer to the underlying element storage

            For non-empty containers, the returned pointer compares equal
            to the address of the first element.

            @see @ref front, @ref back, @ref size

        */
        T* data() { return elems; }

        /** Direct access to the underlying array

            Returns pointer to the underlying array serving as element storage.

            @par Complexity

            Constant

            @note The pointer is such that range [data(); data() + size()) is always
            a valid range, even if the container is empty.

            When the container is empty, data() is not dereferenceable.

            If size() is 0, data() may or may not return a null pointer.

            This function is not available in `std::array`

            @return Pointer to the underlying element storage

            For non-empty containers, the returned pointer compares equal
            to the address of the first element.

            @see @ref front, @ref back, @ref size

        */
        T* c_array() { return elems; }


        /// Member group: Assignment with type conversion


        /** Overwrites element of the array with elements of another array

            This function overwrites every element of the this array with the
            corresponding elements of another array

            @par Complexity

            Linear

            @return Pointer to `this` array

            @see @ref assign

        */
        template <typename T2>
        array<T,N>& operator= (const array<T2,N>& rhs) {
            std::copy(rhs.begin(),rhs.end(), begin());
            return *this;
        }

        /** Assign one value to all elements

            This function assigns the same value to all arrays elements

            @par Complexity

            Linear

            @note This function is not available in `std::array`.

            This function is a synonym for @ref fill.

            @return (none)

            @see @ref operator=, @ref fill

        */
        void assign (const T& value) { fill ( value ); }

        /** Assign one value to all elements

            This function assigns the same value to all arrays elements

            @par Complexity

            Linear

            @note This function is not available in `std::array`

            @return (none)

            @see @ref operator=

        */
        void fill   (const T& value)
        {
            std::fill_n(begin(),size(),value);
        }


        /// Member group: Check range


        /** Check range

            This function checks if @ref i in the valid range for the array.

            This function may be private because it is static.

            @par Complexity

            Constant

            @par Exception Safety

            Throws `std::out_of_range` if @ref i is not in the valid range.

            @note This function is not available in `std::array`

            @return True if @ref i is in the valid range for the array

            @see @ref size, @ref empty

        */
        static BOOST_CONSTEXPR bool rangecheck (size_type i) {
            return i >= size() ? boost::throw_exception(std::out_of_range ("array<>: index out of range")), true : true;
        }

    };

#ifndef BOOST_ARRAY_DOCS

    template< class T >
    class array< T, 0 > {

      public:
        // type definitions
        typedef T              value_type;
        typedef T*             iterator;
        typedef const T*       const_iterator;
        typedef T&             reference;
        typedef const T&       const_reference;
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;

        // iterator support
        iterator        begin()       { return       iterator( reinterpret_cast<       T * >( this ) ); }
        const_iterator  begin() const { return const_iterator( reinterpret_cast< const T * >( this ) ); }
        const_iterator cbegin() const { return const_iterator( reinterpret_cast< const T * >( this ) ); }

        iterator        end()       { return  begin(); }
        const_iterator  end() const { return  begin(); }
        const_iterator cend() const { return cbegin(); }

        // reverse iterator support
#if !defined(BOOST_MSVC_STD_ITERATOR) && !defined(BOOST_NO_STD_ITERATOR_TRAITS)
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#elif defined(_RWSTD_NO_CLASS_PARTIAL_SPEC)
        typedef std::reverse_iterator<iterator, std::random_access_iterator_tag,
              value_type, reference, iterator, difference_type> reverse_iterator;
        typedef std::reverse_iterator<const_iterator, std::random_access_iterator_tag,
              value_type, const_reference, const_iterator, difference_type> const_reverse_iterator;
#else
        // workaround for broken reverse_iterator implementations
        typedef std::reverse_iterator<iterator,T> reverse_iterator;
        typedef std::reverse_iterator<const_iterator,T> const_reverse_iterator;
#endif

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }
        const_reverse_iterator crend() const {
            return const_reverse_iterator(begin());
        }

        // operator[]
        reference operator[](size_type /*i*/)
        {
            return failed_rangecheck();
        }

        /*BOOST_CONSTEXPR*/ const_reference operator[](size_type /*i*/) const
        {
            return failed_rangecheck();
        }

        // at() with range check
        reference at(size_type /*i*/)               {   return failed_rangecheck(); }
        /*BOOST_CONSTEXPR*/ const_reference at(size_type /*i*/) const   { return failed_rangecheck(); }

        // front() and back()
        reference front()
        {
            return failed_rangecheck();
        }

        BOOST_CONSTEXPR const_reference front() const
        {
            return failed_rangecheck();
        }

        reference back()
        {
            return failed_rangecheck();
        }

        BOOST_CONSTEXPR const_reference back() const
        {
            return failed_rangecheck();
        }

        // size is constant
        static BOOST_CONSTEXPR size_type size() { return 0; }
        static BOOST_CONSTEXPR bool empty() { return true; }
        static BOOST_CONSTEXPR size_type max_size() { return 0; }
        enum { static_size = 0 };

        void swap (array<T,0>& /*y*/) {
        }

        // direct access to data (read-only)
        const T* data() const { return 0; }
        T* data() { return 0; }

        // use array as C array (direct read/write access to data)
        T* c_array() { return 0; }

        // assignment with type conversion
        template <typename T2>
        array<T,0>& operator= (const array<T2,0>& ) {
            return *this;
        }

        // assign one value to all elements
        void assign (const T& value) { fill ( value ); }
        void fill   (const T& ) {}

        // check range (may be private because it is static)
        static reference failed_rangecheck () {
                std::out_of_range e("attempt to access element of an empty array");
                boost::throw_exception(e);
#if defined(BOOST_NO_EXCEPTIONS) || (!defined(BOOST_MSVC) && !defined(__PATHSCALE__))
                //
                // We need to return something here to keep
                // some compilers happy: however we will never
                // actually get here....
                //
                static T placeholder;
                return placeholder;
#endif
            }
    };
#endif // BOOST_ARRAY_DOCS

    /// Member group: Comparisons


    /** Lexicographically compares the values in the array (@tparam)

        This function lexicographically compares the values in the array.

        @par Complexity

        Linear

        @tparam T array value type

        @tparam N array size

        @return true if `x` == `y`

        @param x First array whose contents to compare

        @param y Second array whose contents to compare
     */
    template<class T, std::size_t N>
    bool operator== (const array<T,N>& x, const array<T,N>& y) {
        return std::equal(x.begin(), x.end(), y.begin());
    }

    /** Lexicographically compares the values in the array (operator<)

        This function lexicographically compares the values in the array.

        @par Complexity

        Linear

        @tparam T array value type

        @tparam N array size

        @return true if `x` < `y`

        @param x First array whose contents to compare

        @param y Second array whose contents to compare

     * */
    template<class T, std::size_t N>
    bool operator< (const array<T,N>& x, const array<T,N>& y) {
        return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }

    /** Lexicographically compares the values in the array (operator!=)

        This function lexicographically compares the values in the array.

        @par Complexity

        Linear

        @tparam T array value type

        @tparam N array size

        @return true if `x` != `y`

        @param x First array whose contents to compare

        @param y Second array whose contents to compare

     */
    template<class T, std::size_t N>
    bool operator!= (const array<T,N>& x, const array<T,N>& y) {
        return !(x==y);
    }

    /** Lexicographically compares the values in the array (operator>)

        This function lexicographically compares the values in the array.

        @par Complexity

        Linear

        @tparam T array value type

        @tparam N array size

        @return true if `x` > `y`

        @param x First array whose contents to compare

        @param y Second array whose contents to compare

     */
    template<class T, std::size_t N>
    bool operator> (const array<T,N>& x, const array<T,N>& y) {
        return y<x;
    }

    /** Lexicographically compares the values in the array (operator<=)

        This function lexicographically compares the values in the array.

        @par Complexity

        Linear

        @tparam T array value type

        @tparam N array size

        @return true if `x` <= `y`

        @param x First array whose contents to compare

        @param y Second array whose contents to compare

     */
    template<class T, std::size_t N>
    bool operator<= (const array<T,N>& x, const array<T,N>& y) {
        return !(y<x);
    }

    /** Lexicographically compares the values in the array (operator>=)

        This function lexicographically compares the values in the array.

        @par Complexity

        Linear

        @tparam T array value type

        @tparam N array size

        @return true if `x` >= `y`

        @param x First array whose contents to compare

        @param y Second array whose contents to compare

     */
    template<class T, std::size_t N>
    bool operator>= (const array<T,N>& x, const array<T,N>& y) {
        return !(x<y);
    }


    /// Member group: Non-member functions

    /** Global swap function

        This function swaps the contents of two arrays with the array `swap`
        member function.

        @par Complexity

        Linear

        @tparam T array value type

        @tparam N array size

        @return (none)

        @param x First array whose contents to swap

        @param y Second array whose contents to swap

     */
    template<class T, std::size_t N>
    inline void swap (array<T,N>& x, array<T,N>& y) {
        x.swap(y);
    }

#if defined(__SUNPRO_CC)
//  Trac ticket #4757; the Sun Solaris compiler can't handle
//  syntax like 'T(&get_c_array(boost::array<T,N>& arg))[N]'
//
//  We can't just use this for all compilers, because the
//      borland compilers can't handle this form.
    namespace detail {
       template <typename T, std::size_t N> struct c_array
       {
           typedef T type[N];
       };
    }

   // Specific for boost::array: simply returns its elems data member.
   template <typename T, std::size_t N>
   typename detail::c_array<T,N>::type& get_c_array(boost::array<T,N>& arg)
   {
       return arg.elems;
   }

   // Specific for boost::array: simply returns its elems data member.
   template <typename T, std::size_t N>
   typename detail::c_array<T,N>::type const& get_c_array(const boost::array<T,N>& arg)
   {
       return arg.elems;
   }
#else
    // Specific for boost::array: simply returns its elems data member.
    template <typename T, std::size_t N>
    T(&get_c_array(boost::array<T,N>& arg))[N]
    {
        return arg.elems;
    }

    // Const version.
    template <typename T, std::size_t N>
    const T(&get_c_array(const boost::array<T,N>& arg))[N]
    {
        return arg.elems;
    }
#endif

#if 0
    // Overload for std::array, assuming that std::array will have
    // explicit conversion functions as discussed at the WG21 meeting
    // in Summit, March 2009.
    template <typename T, std::size_t N>
    T(&get_c_array(std::array<T,N>& arg))[N]
    {
        return static_cast<T(&)[N]>(arg);
    }
    
    // Const version.
    template <typename T, std::size_t N>
    const T(&get_c_array(const std::array<T,N>& arg))[N]
    {
        return static_cast<T(&)[N]>(arg);
    }
#endif

    /** Calculate combined hash value of the elements of an iterator range

        Forward declaration for Boost.ContainerHash

        @par Complexity

        Linear

        @note

        This function is used to calculate the hash for an array

        @return combined hash value of the elements in the range

        @param It First element in the range
        @param It Second element in the range

        @see @ref hash_value

    */
    template <class It> std::size_t hash_range(It, It);

    /** Calculate hash value for an array

        This function calculates the hash value for an array

        @par Complexity

        Linear

        @note

        This function is used to calculate the hash for an array, which
        allows it to be inserted in unordered containers.

        @return hash value for an array

        @param arr Array for which we calculate the hash

        @see @ref hash_range

    */
    template<class T, std::size_t N>
    std::size_t hash_value(const array<T,N>& arr)
    {
        return boost::hash_range(arr.begin(), arr.end());
    }

   /** Extracts the Ith element from the array
   
       This function wxtracts the @ref Idx -th element element from the array.
       
       Unlike @ref at and @ref operator[], the condition that Idx is in the
       range [0,N) is enforced at compile-time.
   
       @par Complexity
   
       Constant
       
       @par Preconditions

       Idx < size()   

       @tparam Idx Element index to extract

       @tparam T array value type

       @tparam N array size

       @return A reference to the Ith element of @ref arr
   
       @param arr array whose contents to extract
       
       @see @ref at, @ref operator[]
   
   */
   template <size_t Idx, typename T, size_t N>
   T &get(boost::array<T,N> &arr) BOOST_NOEXCEPT {
       BOOST_STATIC_ASSERT_MSG ( Idx < N, "boost::get<>(boost::array &) index out of range" );
       return arr[Idx];
   }

   /** Extracts the Ith element from a constant array

       This function wxtracts the @ref Idx -th element element from a constant
       array.

       Unlike @ref at and @ref operator[], the condition that Idx is in the
       range [0,N) is enforced at compile-time.

       @par Complexity

       Constant

       @par Preconditions

       Idx < size()

       @tparam Idx Element index to extract

       @tparam T array value type

       @tparam N array size

       @return A constant reference to the Ith element of @ref arr

       @param arr array whose contents to extract

       @see @ref at, @ref operator[]

   */
   template <size_t Idx, typename T, size_t N>
   const T &get(const boost::array<T,N> &arr) BOOST_NOEXCEPT {
       BOOST_STATIC_ASSERT_MSG ( Idx < N, "boost::get<>(const boost::array &) index out of range" );
       return arr[Idx];
   }

} /* namespace boost */

#ifndef BOOST_NO_CXX11_HDR_ARRAY
//  If we don't have std::array, I'm assuming that we don't have std::get
namespace std {
   template <size_t Idx, typename T, size_t N>
   T &get(boost::array<T,N> &arr) BOOST_NOEXCEPT {
       BOOST_STATIC_ASSERT_MSG ( Idx < N, "std::get<>(boost::array &) index out of range" );
       return arr[Idx];
   }

   template <size_t Idx, typename T, size_t N>
   const T &get(const boost::array<T,N> &arr) BOOST_NOEXCEPT {
       BOOST_STATIC_ASSERT_MSG ( Idx < N, "std::get<>(const boost::array &) index out of range" );
       return arr[Idx];
   }
}
#endif

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)
# pragma warning(pop)
#endif

#endif /*BOOST_ARRAY_HPP*/
