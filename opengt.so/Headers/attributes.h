
#ifndef __OPENGRAPHTHEORY_ATTRIBUTES_H
    #define __OPENGRAPHTHEORY_ATTRIBUTES_H

    #include <string>
    #include <iostream>
    #include <sstream>

    #include "factory.h"
    #include "simple_xml.h"


    class Attribute
    {
        public:
            virtual ~Attribute();
            virtual void WriteToXml(OpenGraphtheory::XML::XML* xml) = 0;
            virtual bool LoadFromXml(OpenGraphtheory::XML::XML* xml) = 0;
            virtual Attribute* Clone() = 0;
    };

    class AttributeCollection
    {
        friend class Graph;
        protected:
            std::map<std::string, Attribute*> Attributes;
        public:
            ~AttributeCollection();
            static OpenGraphtheory::Factory<Attribute> AttributeFactory;

            void Set(OpenGraphtheory::XML::XML* attr);
            bool Add(std::string name, std::string type);
            void WriteToXml(OpenGraphtheory::XML::XML* xml);

            void Clear();
            void Unset(std::string name);
            void operator=(const AttributeCollection& attrs);

            bool HasAttribute(std::string name);
            Attribute* GetAttribute(std::string name);
    };


    class BoolAttribute : public Attribute
    {
        public:
            bool Value;

            BoolAttribute();
            BoolAttribute(bool value);

            void WriteToXml(OpenGraphtheory::XML::XML* xml);
            bool LoadFromXml(OpenGraphtheory::XML::XML* xml);
            Attribute* Clone();
    };

    class IntAttribute : public Attribute
    {
        public:
            int Value;

            IntAttribute();
            IntAttribute(int value);

            void WriteToXml(OpenGraphtheory::XML::XML* xml);
            bool LoadFromXml(OpenGraphtheory::XML::XML* xml);
            Attribute* Clone();
    };

    class FloatAttribute : public Attribute
    {
        public:
            float Value;

            FloatAttribute();
            FloatAttribute(float value);

            void WriteToXml(OpenGraphtheory::XML::XML* xml);
            bool LoadFromXml(OpenGraphtheory::XML::XML* xml);
            Attribute* Clone();
    };

    class StringAttribute : public Attribute
    {
        public:
            std::string Value;

            StringAttribute();
            StringAttribute(std::string value);

            void WriteToXml(OpenGraphtheory::XML::XML* xml);
            bool LoadFromXml(OpenGraphtheory::XML::XML* xml);
            Attribute* Clone();
    };

    class SeqAttribute : public Attribute
    {
        public:
            std::list<Attribute*> Value;

            SeqAttribute();
            SeqAttribute(const std::list<Attribute*>& seq);
            ~SeqAttribute();

            void WriteToXml(OpenGraphtheory::XML::XML* xml);
            bool LoadFromXml(OpenGraphtheory::XML::XML* xml);
            Attribute* Clone();
    };

#endif
