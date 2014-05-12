
#ifndef __OPENGRAPHTHEORY_FACTORY_H
    #define __OPENGRAPHTHEORY_FACTORY_H

    #include <string>
    #include <map>
    #include <set>

    namespace OpenGraphtheory
    {

        class FactoryEnumerator
        {
            public:
                virtual ~FactoryEnumerator();
                virtual void Enumerate(std::string name, std::string description, std::string url) = 0;
        };

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
                virtual void Enumerate(FactoryEnumerator* enumerator);
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
                void Enumerate(FactoryEnumerator* enumerator);
        };

        template<class T> class FactoryRegistrator
        {
            public:
                FactoryRegistrator(Factory<T>* factory, std::string name, Instantiator<T>* instantiator);
        };

        template<class T> class MultiFactory
        {
            private:
                std::map<std::string, std::set<Instantiator<T>*> > instantiators;
            public:
                ~MultiFactory();
                void RegisterClass(std::string name, Instantiator<T>* instantiator);
                std::set<T*> Produce(std::string name);
                void Enumerate(FactoryEnumerator* enumerator);
        };

        template<class T> class MultiFactoryRegistrator
        {
            public:
                MultiFactoryRegistrator(MultiFactory<T>* factory, std::string name, Instantiator<T>* instantiator);
        };


    }

    #include "../Sources/factory_templates.cpp"

#endif
