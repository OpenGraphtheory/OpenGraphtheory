
#ifndef __ATTRIBUTES_H
    #define __ATTRIBUTES_H

    #include <string>
    #include <iostream>
    #include "factory.h"
    #include "simple_xml.h"

    class Attribute
    {
        public:
            virtual ~Attribute();
            virtual void WriteToXml(XML* xml) = 0;
            virtual bool LoadFromXml(XML* xml) = 0;
            virtual Attribute* Clone() = 0;
    };

    class AttributeCollection
    {
        friend class Graph;
        protected:
            map<string, Attribute*> Attributes;
        public:
            ~AttributeCollection();
            static Factory<Attribute> AttributeFactory;

            void Set(XML* attr);
            bool Add(string name, string type);
            void WriteToXml(XML* xml);

            void Clear();
            void Unset(string name);
            void operator=(const AttributeCollection& attrs);

            bool HasAttribute(string name);
            Attribute* GetAttribute(string name);
    };


    class BoolAttribute : public Attribute
    {
        public:
            bool Value;

            BoolAttribute();
            BoolAttribute(bool value);

            void WriteToXml(XML* xml);
            bool LoadFromXml(XML* xml);
            Attribute* Clone();
    };

    class IntAttribute : public Attribute
    {
        public:
            int Value;

            IntAttribute();
            IntAttribute(int value);

            void WriteToXml(XML* xml);
            bool LoadFromXml(XML* xml);
            Attribute* Clone();
    };

    class FloatAttribute : public Attribute
    {
        public:
            float Value;

            FloatAttribute();
            FloatAttribute(float value);

            void WriteToXml(XML* xml);
            bool LoadFromXml(XML* xml);
            Attribute* Clone();
    };

    class StringAttribute : public Attribute
    {
        public:
            string Value;

            StringAttribute();
            StringAttribute(string value);

            void WriteToXml(XML* xml);
            bool LoadFromXml(XML* xml);
            Attribute* Clone();
    };

    class SeqAttribute : public Attribute
    {
        public:
            list<Attribute*> Value;

            SeqAttribute();
            SeqAttribute(const list<Attribute*>& seq);
            ~SeqAttribute();

            void WriteToXml(XML* xml);
            bool LoadFromXml(XML* xml);
            Attribute* Clone();
    };

#endif