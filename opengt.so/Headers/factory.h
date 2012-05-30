
#ifndef __FACTORY_H
    #define __FACTORY_H

    #include<string>
    #include<map>

    template<class T> class Instantiator
    {
        public:
            std::string Name;
            std::string Description;
            std::string URL;

            Instantiator();
            Instantiator(std::string name, std::string description, std::string URL);
            virtual ~Instantiator();

            virtual T* Instantiate() = 0;
    };

    template<class T, class U> class DefaultInstantiator : public Instantiator<T>
    {
        public:
            DefaultInstantiator();
            DefaultInstantiator(std::string Name, std::string Description, std::string URL);
            T* Instantiate();
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


    // ======================================================================================================================


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



    template<class T> FactoryRegistrator<T>::FactoryRegistrator(Factory<T>* factory, std::string name, Instantiator<T>* instantiator)
    {
        factory->RegisterClass(name, instantiator);
    }


#endif
