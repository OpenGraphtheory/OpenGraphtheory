
#ifndef __OPENGRAPHTHEORY_SETHELPER_CPP
    #define __OPENGRAPHTHEORY_SETHELPER_CPP

        #include "../Headers/set_helper.h"

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

                template<class T> size_t IntersectionSize(const std::set<T> &A, const std::set<T> &B)
                {
                    size_t result = 0;
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


                template<class T> std::set<T> SymmetricDifference(const std::set<T> &A, const std::set<T> &B)
                {
                    typename std::set<T> result;
                    for(typename std::set<T>::iterator a = A.begin(); a != A.end(); a++)
                        if(B.find(*a) == B.end())
                            result.insert(*a);
                    for(typename std::set<T>::iterator b = B.begin(); b != B.end(); b++)
                        if(A.find(*b) == A.end())
                            result.insert(*b);
                    return result;
                }


                template<class T> void DestructiveSymmetricDifference(std::set<T> &A, const std::set<T> &B)
                {
                    for(typename std::set<T>::iterator b = B.begin(); b != B.end(); b++)
                    {
                        typename std::set<T>::iterator pos = A.find(*b);
                        if(pos == A.end())
                            A.insert(*b);
                        else
                            A.erase(pos);
                    }
                }

                template<class T> bool IsSubset(const std::set<T> &A, const std::set<T> &B)
                {
                    for(typename std::set<T>::iterator a = A.begin(); a != A.end(); a++)
                        if(B.find(*a) == B.end())
                            return false;
                    return true;
                }

                template<class T> bool SetsEqual(const std::set<T> &A, const std::set<T> &B)
                {
                    return IsSubset(A,B) && IsSubset(B,A);
                }

                template<class T> bool Contains(const std::set<T> &A, const T &Element)
                {
                    return A.find(Element) != A.end();
                }



            }
        }

#endif
