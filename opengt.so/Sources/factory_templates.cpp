
#ifndef __OPENGRAPHTHEORY_FACTORY_CPP
    #define __OPENGRAPHTHEORY_FACTORY_CPP

    #include "../Headers/factory.h"

    namespace OpenGraphtheory
    {

        template<class T> Instantiator<T>::Instantiator()
        {
        }

        template<class T> Instantiator<T>::Instantiator(std::string Name, std::string Description, std::string URL)
        {
            this->Name = Name;
            this->Description = Description;
            this->URL = URL;
        }

        template<class T> Instantiator<T>::~Instantiator()
        {

        }

        template<class T> void Instantiator<T>::Enumerate(FactoryEnumerator* enumerator)
        {
            if(enumerator != NULL)
                enumerator->Enumerate(Name, Description, URL);
        }


        template<class T, class U> DefaultInstantiator<T,U>::DefaultInstantiator()
            : Instantiator<T>()
        {
        }

        template<class T, class U> DefaultInstantiator<T,U>::DefaultInstantiator(std::string Name, std::string Description, std::string URL)
            : Instantiator<T>(Name, Description, URL)
        {
        }

        template<class T, class U> T* DefaultInstantiator<T, U>::Instantiate()
        {
            return new U();
        }


        // ======================================================================================================================


        template<class T> Factory<T>::~Factory()
        {
            for(typename std::map<std::string, Instantiator<T>* >::iterator i = instantiators.begin(); i != instantiators.end(); i++)
                delete i->second;
        }

        template<class T> void Factory<T>::RegisterClass(std::string ClassName, Instantiator<T>* instantiator)
        {
            typename std::map<std::string, Instantiator<T>* >::iterator iInstantiator = instantiators.find(ClassName);
            if(iInstantiator != instantiators.end())
                delete instantiators[ClassName];
            instantiators[ClassName] = instantiator;
        }

        template<class T> T* Factory<T>::Produce(std::string ClassName)
        {
            typename std::map<std::string, Instantiator<T>* >::iterator iInstantiator = instantiators.find(ClassName);
            if(iInstantiator == instantiators.end())
                return NULL;
            return iInstantiator->second->Instantiate();
        }

        template<class T> void Factory<T>::Enumerate(FactoryEnumerator* enumerator)
        {
            for(typename std::map<std::string, Instantiator<T>* >::iterator i = instantiators.begin(); i != instantiators.end(); i++)
                i->second->Enumerate(enumerator);
        }



        template<class T> FactoryRegistrator<T>::FactoryRegistrator(Factory<T>* factory, std::string name, Instantiator<T>* instantiator)
        {
            factory->RegisterClass(name, instantiator);
        }

    }

#endif
