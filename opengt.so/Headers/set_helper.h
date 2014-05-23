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
            template<class T> unsigned int IntersectionSize(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool IntersectionEmpty(const std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> SetMinus(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveSetMinus(std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> Union(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveUnion(std::set<T> &A, const std::set<T> &B);
            template<class T> std::set<T> SymmetricDifference(const std::set<T> &A, const std::set<T> &B);
            template<class T> void DestructiveSymmetricDifference(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool IsSubset(const std::set<T> &A, const std::set<T> &B);
            template<class T> bool SetsEqual(const std::set<T> &A, const std::set<T> &B);

            template<class T> class VectorSet : public std::vector<T>
            {
                public:
                    typename VectorSet<T>::iterator find(const T& element);
                    typename VectorSet<T>::const_iterator find(const T& element) const;
                    bool contains(const T& element);
                    bool contains(const T& element) const;
                    void insert(const T& element);
                    void erase(const T& element);

                    bool operator<=(const VectorSet<T>& B) const;
                    bool operator==(const VectorSet<T>& B) const;
                    VectorSet<T> operator+(const VectorSet<T>& B) const;
                    void operator+=(const VectorSet<T>& B);
                    VectorSet<T> operator-(const VectorSet<T>& B) const;
                    void operator-=(const VectorSet<T>& B);

                    VectorSet<T> intersection(const VectorSet<T>& B) const;
                    VectorSet<T> symmetric_difference(const VectorSet<T>& B) const;
            };

        }
    }

    #include "../Sources/set_helper.cpp"

#endif
