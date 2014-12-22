#ifndef RERSOURCE_H
#define RERSOURCE_H

#include <cstddef>
#include <typeindex>

class Resource
{
    public:
        virtual ~Resource();

        inline int getRefCount() const {return mRefCount;}

        inline void incRefCount() {++mRefCount;}
        inline void decRefCount() {--mRefCount;}

        inline std::type_index getType() const {return mType;}
    protected:
        Resource(std::type_index type);

        unsigned int mRefCount;

        std::type_index mType;
};


template <typename T>
class ResPtr
{
    public:
        inline ResPtr() : mPtr(NULL) {}

        inline ResPtr(T *res) : mPtr(res)
        {
            if (mPtr != NULL)
            {
                mPtr->incRefCount();
            }
        }

        inline ResPtr(const ResPtr<T>& other) : mPtr(other.mPtr)
        {
            if (mPtr != NULL)
            {
                mPtr->incRefCount();
            }
        }

        inline ~ResPtr()
        {
            if (mPtr != NULL)
            {
                mPtr->decRefCount();

                mPtr = NULL;
            }
        }

        ResPtr& operator = (const ResPtr<T>& other)
        {
            if (mPtr != NULL)
            {
                mPtr->decRefCount();
            }

            mPtr = other.mPtr;

            if (mPtr != NULL)
            {
                mPtr->incRefCount();
            }

            return *this;
        }

        ResPtr& operator = (const T*& other)
        {
            if (mPtr != NULL)
            {
                mPtr->decRefCount();
            }

            mPtr = other;

            if (mPtr != NULL)
            {
                mPtr->incRefCount();
            }

            return *this;
        }

        inline bool operator == (const ResPtr<T>& other) const
        {
            return mPtr == other.mPtr;
        }

        inline bool operator != (const ResPtr<T>& other) const
        {
            return mPtr != other.mPtr;
        }

        inline bool operator <= (const ResPtr<T>& other) const
        {
            return mPtr <= other.mPtr;
        }

        inline bool operator >= (const ResPtr<T>& other) const
        {
            return mPtr >= other.mPtr;
        }

        inline bool operator < (const ResPtr<T>& other) const
        {
            return mPtr < other.mPtr;
        }

        inline bool operator > (const ResPtr<T>& other) const
        {
            return mPtr > other.mPtr;
        }

        inline T& operator * ()
        {
            return *mPtr;
        }

        inline T& operator * () const
        {
            return *mPtr;
        }

        inline T* operator -> ()
        {
            return mPtr;
        }

        inline T* operator -> () const
        {
            return mPtr;
        }

        inline T *getPointer() const
        {
            return mPtr;
        }

        template <typename T2>
        inline ResPtr<T2> cast() const
        {
            return mPtr->getType() == typeid(T2) ? ResPtr<T2>((T2 *)mPtr) : ResPtr<T2>();
        }
    private:
        T *mPtr;
};

template <typename T>
inline ResPtr<T> nullRes()
{
    return ResPtr<T>();
}

#endif // RERSOURCE_H
