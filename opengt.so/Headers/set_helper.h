#ifndef __OPENGRAPHTHEORY_SET_HELPER_H
    #define __OPENGRAPHTHEORY_SET_HELPER_H

    #include<set>
    #include<vector>
    #include<algorithm>

    namespace OpenGraphtheory
    {
        namespace SetHelper
        {
            template<class T> std::set<T> Intersection(const std::set<T> &A, const std::set<T> &B);
            template<class T> size_t IntersectionSize(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool IntersectionEmpty(const std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> SetMinus(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveSetMinus(std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> Union(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveUnion(std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> SymmetricDifference(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveSymmetricDifference(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool IsSubset(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool SetsEqual(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool Contains(const std::set<T> &A, const T &Element);
        }
    }

    #include "../Sources/set_helper.cpp"

#endif
