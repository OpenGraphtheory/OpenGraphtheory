
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

#endif
