#pragma once
#include <vector>

		// TEMPLATE CLASS undostack
template<class _Ty,
class _Container = std::vector<_Ty> >
	class undostack
	{	// LIFO undo-stack with undo/redo iteration support
public:
	typedef _Container container_type;
	typedef typename _Container::pointer pointer;
	typedef typename _Container::const_pointer const_pointer;
	typedef typename _Container::const_reference const_reference;
	typedef typename _Container::size_type size_type;


	undostack(size_type _size)
		: c(_size)
		, _MaxSize(_size)
		, _Head(-1), _Tail(-1), _Current(-1)
		{
		}

	void clear()
		{
		_Head = _Tail = _Current = -1;
		}

	size_type deep() const
		{
		return _MaxSize;
		}

	void size(size_type newsize)
		{
		c.reserve(newsize);
		_MaxSize = newsize;
		clear();
		}

	bool empty() const
		{	// test if stack is empty
		return (c.empty());
		}

	size_type size() const
		{	// test length of stack
		return (c.size());
		}

	void push(const_reference _UInfo)
		{
		// push & update "current"
		c[++_Current % _MaxSize] = _UInfo;	
		// update "head"
		_Head = _Current;
		// update "tail"
		if (_Tail == (size_type)-1)
			_Tail = 0;
		else if (_Head - _Tail == _MaxSize)
			++_Tail;
		}

	pointer undo()
		{	// get undo and dec stack frame pointer
		return _Current == _Tail ? 0 : &c[--_Current % _MaxSize];
		}

	pointer redo()
		{	// get redo and inc stack frame pointer
		return _Current == _Head ? 0 : &c[++_Current % _MaxSize];
		}

	pointer top()
		{	// get real top (stack head)
		return _Head == -1 ? 0 : &c[_Head % _MaxSize];
		}

	const_pointer top() const
		{	// get real top (non-mutable stack head)
		return _Head == -1 ? 0 : &c[_Head % _MaxSize];
		}

	pointer prev()
		{	// get previous undo frame
		return _Current == _Tail ? 0 : &c[(_Current - 1) % _MaxSize];
		}

	const_pointer prev() const
		{	// get previous non-mutable undo frame
		return _Current == _Tail ? 0 : &c[(_Current - 1) % _MaxSize];
		}

	pointer current()
		{	// get current undo frame
		return _Current == -1 ? 0 : &c[_Current % _MaxSize];
		}

	const_pointer current() const
		{	// get current non-mutable undo frame
		return _Current == -1 ? 0 : &c[_Current % _MaxSize];
		}

	pointer next()
		{	// get next undo frame
		return _Current == _Head ? 0 : &c[(_Current + 1) % _MaxSize];
		}

	const_pointer next() const
		{	// get next non-mutable undo frame
		return _Current == _Head ? 0 : &c[(_Current + 1) % _MaxSize];
		}

	const_pointer undoinfo() const
		{
		return _Current == _Tail ? 0 : &c[_Current % _MaxSize];
		}

	const_pointer redoinfo() const
		{
		return next();
		}


//protected:
	_Container	c;			// the underlying container
	size_type	_MaxSize,
				_Head,
				_Tail,
				_Current;	// current undo frame position
	};


		// undostack TEMPLATE FUNCTIONS
template<class _Ty,
	class _Container> inline
	bool operator==(const undostack<_Ty, _Container>& _Left,
		const undostack<_Ty, _Container>& _Right)
	{	// test for undostack equality
	return (_Left.c == _Right.c);
	}

template<class _Ty,
	class _Container> inline
	bool operator!=(const undostack<_Ty, _Container>& _Left,
		const undostack<_Ty, _Container>& _Right)
	{	// test for undostack inequality
	return (!(_Left == _Right));
	}

template<class _Ty,
	class _Container> inline
	bool operator<(const undostack<_Ty, _Container>& _Left,
		const undostack<_Ty, _Container>& _Right)
	{	// test if _Left < _Right for stacks
	return (_Left.c < _Right.c);
	}

template<class _Ty,
	class _Container> inline
	bool operator>(const undostack<_Ty, _Container>& _Left,
		const undostack<_Ty, _Container>& _Right)
	{	// test if _Left > _Right for stacks
	return (_Right < _Left);
	}

template<class _Ty,
	class _Container> inline
	bool operator<=(const undostack<_Ty, _Container>& _Left,
		const undostack<_Ty, _Container>& _Right)
	{	// test if _Left <= _Right for stacks
	return (!(_Right < _Left));
	}

template<class _Ty,
	class _Container> inline
	bool operator>=(const undostack<_Ty, _Container>& _Left,
		const undostack<_Ty, _Container>& _Right)
	{	// test if _Left >= _Right for stacks
	return (!(_Left < _Right));
	}