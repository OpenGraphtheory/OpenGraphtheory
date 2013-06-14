
//#include "../Headers/set_helper.h"

#include <set>

namespace OpenGraphtheory
{
    namespace SetHelper
    {
        template<class T> std::set<T> Intersection(const std::set<T> &A, const std::set<T> &B)
        {
            std::set<T> result;
            for(typename std::set<T>::iterator a = A.begin(); a != A.end(); a++)
                if(B.find(*a) != B.end())
                    result.insert(*a);
            return result;
        }

        template<class T> unsigned int IntersectionSize(const std::set<T> &A, const std::set<T> &B)
        {
            unsigned int result = 0;
            for(typename std::set<T>::iterator a = A.begin(); a != A.end(); a++)
                if(B.find(*a) != B.end())
                    result++;
            return result;
        }

        template<class T> bool IntersectionEmpty(const std::set<T> &A, const std::set<T> &B)
        {
            for(typename std::set<T>::iterator a = A.begin(); a != A.end(); a++)
                if(B.find(*a) != B.end())
                    return false;
            return true;
        }


        template<class T> std::set<T> SetMinus(const std::set<T> &A, const std::set<T> &B)
        {
            std::set<T> result;
            for(typename std::set<T>::iterator a = A.begin(); a != A.end(); a++)
                if(B.find(*a) == B.end())
                    result.insert(*a);
            return result;
        }

        template<class T> void DestructiveSetMinus(std::set<T> &A, const std::set<T> &B)
        {
            for(typename std::set<T>::iterator b = B.begin(); b != B.end(); b++)
                A.erase(*b);
        }


        template<class T> std::set<T> Union(const std::set<T> &A, const std::set<T> &B)
        {
            std::set<T> result;
            for(typename std::set<T>::iterator a = A.begin(); a != A.end(); a++)
                result.insert(*a);
            for(typename std::set<T>::iterator b = B.begin(); b != B.end(); b++)
                result.insert(*b);
            return result;
        }

        template<class T> void DestructiveUnion(std::set<T> &A, const std::set<T> &B)
        {
            for(typename std::set<T>::iterator b = B.begin(); b != B.end(); b++)
                A.insert(*b);
        }


        template<class T> bool IsSubset(const std::set<T> &A, const std::set<T> &B)
        {
            for(typename std::set<T>::iterator a = A.begin(); a != A.end(); a++)
                if(B.find(*a) == B.end())
                    return false;
            return true;
        }

    }
}

