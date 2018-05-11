// Include guard
#pragma once

// Local includes
#include "nx-type.hh"

// Namespace "nx"
namespace nx {
	
// List class - Dynamically resizable arrays -- TODO: concurrent, lock free, virtual interface
template<typename T> class List: public Object
{
public:
	// Element type
	using Type = T;

	// Constructors & destructors
	List()
		: n(0), m(0), data(nullptr) {}
	~List() override
		{nx::type::destroyArrayAt(data, n); nx::type::free(data);}
		
	// Size & capacity	
	inline size_t size() const
		{return n;}
	inline size_t capacity() const
		{return m;}
		
	void resize(size_t n);
	void reserve(size_t n);
	void compact();
	
	// Getters
	inline const T & get(size_t i, const T & def = T())
		{return (0 <= i) && (i < n) ? data[i] : def;}
		
	// Methods
	void append(T && item);
	void append(const T & item);
	
	void extend(List<T> && list);
	void extend(const List<T> & list);
	
	// Itertator methods
	inline T * begin()
		{return data;}
	inline const T * begin() const
		{return data;}
	inline T * end()
		{return data + n;}
	inline const T * end() const
		{return data + n;}
		
	// Operators
	inline T & operator [] (size_t i)
		{return data[i];}
	inline const T & operator [] (size_t i) const
		{return data[i];}
		
	inline List<T> & operator += (List<T> && list)
		{extend(static_cast<List<T> &&>(list)); return * this;}
	inline List<T> & operator += (const List<T> & list)
		{extend(list); return * this;}

private:
	size_t n;
	size_t m;
	T * data;
};


template<typename T> void List<T>::resize(size_t size)
{
	reserve(size);
	if (size > n)
	{
		nx::type::createArrayAt(data + n, size - n);
		n = size;
	}
	if (size < n)
	{
		nx::type::destroyArrayAt(data + size, n - size);
		n = size;
	}
}

template<typename T> void List<T>::reserve(size_t size)
{
	if (size > m)
	{
		List list;
		list.data = nx::type::alloc<T>(sizeof(T) * size);
		nx::type::confirm(list.data);
		
		if (data)
		{
			nx::type::createArrayAtByMove(list.data, data, n);
			nx::type::destroyArrayAt(data, n);
			nx::type::free(data);
		}
		
		m = size;
		data = list.data;
		list.data = nullptr;
	}
}

template<typename T> void List<T>::compact()
{
	if (data)
	{
		List list;
		list.data = nx::type::alloc<T>(sizeof(T) * n);
		nx::type::confirm(list.get());
		
		nx::type::createArrayAtByMove(list.data, data, n);
		nx::type::destroyArrayAt(data, n);
		nx::type::free(data);
		
		m = n;
		data = list.data;
		list.data = nullptr;
	}
}

template<typename T> void List<T>::append(T && item)
{
	if (n + 1 > m)
	{
		// Exponential growth with a 1.5 base (starting from 16)
		m = m > 16 ? m : 16; 
		reserve(m + (m >> 1));
	}
	
	data[n ++] = static_cast<T &&>(item);
}

template<typename T> void List<T>::append(const T & item)
{
	if (n + 1 > m)
	{
		// Exponential growth with a 1.5 base (starting from 16)
		m = m > 16 ? m : 16; 
		reserve(m + (m >> 1));
	}
	
	data[n ++] = item;
}

template<typename T> void List<T>::extend(List<T> && list)
{
	if (n + list.n > m)
	{
	}
	
	if (nx::type::isCreateNoexcept<T>())
	{
	}
	else
	{
	}
}


// Set class - TODO: concurrent, lock free, virtual interface
template<typename T> class Set: public Object
{
};


// Dictionary class - Associative array, map, or dictionary -- TODO: concurrect, lock free, virtual interface
template<typename K, typename V> class Dictionary: public Object
{
};

// Close namespace "nx"
}
