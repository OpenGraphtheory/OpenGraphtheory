#ifndef __OPENGRAPHTHEORY_SETHELPER_H
    #define __OPENGRAPHTHEORY_SETHELPER_H

    #include<set>

    namespace OpenGraphtheory
    {
        namespace SetHelper
        {
            template<class T> std::set<T> Intersection(const std::set<T> &A, const std::set<T> &B);
            template<class T> unsigned int IntersectionSize(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool IntersectionEmpty(const std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> SetMinus(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveSetMinus(std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> Union(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveUnion(const std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> SymmetricDifference(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveSymmetricDifference(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool IsSubset(const std::set<T> &A, const std::set<T> &B);
        }
    }

    #include "../Sources/set_helper.cpp"

#endif
