
#ifndef WEAK_PTR
#define WEAK_PTR

#include "weakcount.h"

template<class T> class SharedPtr;

template <class T>
class WeakPtr
{
public:
   WeakPtr();
   WeakPtr(const WeakPtr& ptr);
   WeakPtr(const SharedPtr<T>& ptr);

private:
   WeakCount mCount;
};

#include "weakptr.inl"

#endif // WEAK_PTR
