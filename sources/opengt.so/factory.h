
#ifndef __FACTORY_H
    #define __FACTORY_H

    #include<string>
    #include<map>

    template<class T> class Instantiator
    {
        public:
            virtual T* Instantiate() = 0;
    };

    template<class T> class Factory
    {
        private:
            std::map<std::string, Instantiator<T>*> instantiators;
        public:
            ~Factory();
            void RegisterClass(std::string name, Instantiator<T>* instantiator);
            T* Produce(std::string name);
    };

    template<class T> class FactoryRegistrator
    {
        public:
            FactoryRegistrator(Factory<T>* factory, std::string name, Instantiator<T>* instantiator);
    };



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

    template<class T> FactoryRegistrator<T>::FactoryRegistrator(Factory<T>* factory, std::string name, Instantiator<T>* instantiator)
    {
        factory->RegisterClass(name, instantiator);
    }


#endif
