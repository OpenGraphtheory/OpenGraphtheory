
#include <map>
#include "factory.h"

using namespace std;

template<class T> Factory<T>::~Factory()
{
    for(typename map<string, Instantiator<T>* >::iterator i = instantiators.begin(); i != instantiators.end(); i++)
        delete i->second;
}

template<class T> void Factory<T>::RegisterClass(string ClassName, Instantiator<T>* instantiator)
{
    instantiators.insert(ClassName, instantiator);
}

template<class T> T* Factory<T>::Produce(string ClassName)
{
    typename map<string, Instantiator<T>* >::iterator iInstantiator = instantiators.find(ClassName);
    if(iInstantiator == instantiators.end())
        return NULL;
    return iInstantiator->second->Instantiate();
}

template<class T> FactoryRegistrator<T>::FactoryRegistrator(Factory<T>* factory, string name, Instantiator<T>* instantiator)
{
    factory->RegisterClass(name, instantiator);
}
