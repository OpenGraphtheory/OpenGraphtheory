
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







                template<class T> typename VectorSet<T>::iterator VectorSet<T>::find(const T &element)
                {
                    return std::find(this->begin(),this->end(), element);
                }

                template<class T> typename VectorSet<T>::const_iterator VectorSet<T>::find(const T &element) const
                {
                    return std::find(this->begin(),this->end(), element);
                }

                template<class T> bool VectorSet<T>::contains(const T &element)
                {
                    return this->find(element) != this->end();
                }

                template<class T> bool VectorSet<T>::contains(const T &element) const
                {
                    return this->find(element) != this->end();
                }

                template<class T> void VectorSet<T>::erase(const T &element)
                {
                    typename std::vector<T>::iterator i = find(element);
                    while(i != this->end())
                    {
                        std::vector<T>::erase(i);
                        i = find(element);
                    }
                }

                template<class T> void VectorSet<T>::insert(const T &element)
                {
                    if(!contains(element))
                        this->push_back(element);
                }



                template<class T> bool VectorSet<T>::operator<=(const VectorSet<T>& B) const
                {
                    for(typename VectorSet<T>::const_iterator i = this->begin(); i != this->end(); i++)
                        if(!B.contains(*i))
                            return false;
                    return true;
                }

                template<class T> bool VectorSet<T>::operator==(const VectorSet<T>& B) const
                {
                    return *this <= B && B <= *this;
                }

                template<class T> VectorSet<T> VectorSet<T>::operator+(const VectorSet<T>& B) const
                {
                    VectorSet<T> result;
                    for(typename VectorSet<T>::const_iterator i = this->begin(); i != this->end(); i++)
                        result.insert(*i);
                    for(typename VectorSet<T>::const_iterator i = B.begin(); i != B.end(); i++)
                        result.insert(*i);
                    return result;
                }

                template<class T> void VectorSet<T>::operator+=(const VectorSet<T>& B)
                {
                    for(typename VectorSet<T>::const_iterator i = B.begin(); i != B.end(); i++)
                        insert(*i);
                }

                template<class T> VectorSet<T> VectorSet<T>::operator-(const VectorSet<T>& B) const
                {
                    VectorSet<T> result = *this;
                    for(typename VectorSet<T>::const_iterator i = B.begin(); i != B.end(); i++)
                        result.erase(*i);
                    return result;
                }

                template<class T> void VectorSet<T>::operator-=(const VectorSet<T>& B)
                {
                    for(typename VectorSet<T>::const_iterator i = B.begin(); i != B.end(); i++)
                        erase(*i);
                }

                template<class T> VectorSet<T> VectorSet<T>::intersection(const VectorSet<T>& B) const
                {
                    VectorSet<T> result;
                    for(typename VectorSet<T>::const_iterator i = B.begin(); i != B.end(); i++)
                        if(this->contains(*i))
                            result.insert(*i);
                    return result;
                }

                template<class T> VectorSet<T> VectorSet<T>::symmetric_difference(const VectorSet<T>& B) const
                {
                    VectorSet<T> result = *this;
                    for(typename VectorSet<T>::const_iterator i = B.begin(); i != B.end(); i++)
                        if(!result.contains(*i))
                            result.insert(*i);
                        else
                            result.erase(*i);
                    return result;
                }

            }
        }

#endif
