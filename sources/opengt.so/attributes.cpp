
    #include "attributes.h"
    #include "factory.h"
    #include <cstdlib>

    /// \defgroup Arbitrary Attributes
    // @{

        Factory<Attribute> AttributeCollection::AttributeFactory;

        void AttributeCollection::Set(XML* attr)
        {
            std::string name = attr->GetAttribute("name");
            if(name == "")
                return;

            for(list<XML_Element*>::iterator child = attr->children.begin(); child != attr->children.end(); child++)
            {
       			XML* xChild = dynamic_cast<XML*>(*child);
                if(xChild == NULL)
                    continue;

                Attribute* attribute = AttributeFactory.Produce(xChild->name);
                if(attribute == NULL)
                    continue;

                if(!attribute->LoadFromXml(xChild))
                {
                    delete attribute;
                    continue;
                }

                Unset(name);
                Attributes[name] = attribute;
                break;
            }
        }

        void AttributeCollection::Clear()
        {
            for(map<string, Attribute*>::const_iterator i = Attributes.begin(); i != Attributes.end(); i++)
                delete i->second;
            Attributes.clear();
        }

        void AttributeCollection::operator=(const AttributeCollection& attrs)
        {
            Clear();
            for(map<string, Attribute*>::const_iterator i = attrs.Attributes.begin(); i != attrs.Attributes.end(); i++)
                Attributes[i->first] = i->second->Clone();
        }

        void AttributeCollection::WriteToStream(ostream& os, int indentlevel)
        {
            string indent(indentlevel*2, ' ');
            for(map<string, Attribute*>::const_iterator i = Attributes.begin(); i != Attributes.end(); i++)
            {
                os << indent << "<attr name=\"" << i->first << "\">\n";
                i->second->WriteToStream(os, indentlevel+1);
                os << indent << "</attr>\n";
            }

        }

        void AttributeCollection::Unset(string name)
        {
            map<string, Attribute*>::iterator it = Attributes.find(name);
            if(it == Attributes.end())
                return;
            delete it->second;
            Attributes.erase(it);
        }

        bool AttributeCollection::HasAttribute(string name)
        {
            map<string, Attribute*>::iterator attr = Attributes.find(name);
            return attr != Attributes.end();
        }

        Attribute* AttributeCollection::GetAttribute(string name)
        {
            map<string, Attribute*>::iterator attr = Attributes.find(name);
            if(attr == Attributes.end())
                return NULL;

            return attr->second;
        }


    // @}



    Attribute* BoolAttributeInstantiator::Instantiate()
    {
        return new BoolAttribute();
    }


    BoolAttribute::BoolAttribute()
        : Attribute()
    {
        Value = false;
    }

    BoolAttribute::BoolAttribute(bool value)
        : Attribute()
    {
        Value = value;
    }

    bool BoolAttribute::LoadFromXml(XML* xml)
    {
        string innertext = xml->InnerText(true);

        if(innertext == "true")
            Value = true;
        else if(innertext == "false")
            Value = false;
        else
            return false;

        return true;
    }

    void BoolAttribute::WriteToStream(ostream& os, int indentlevel)
    {
        string indent(indentlevel*2, ' ');
        os << indent << "<bool>" << (Value ? "true" : "false") << "</bool>\n";
    }

    Attribute* BoolAttribute::Clone()
    {
        return new BoolAttribute(Value);
    }

    FactoryRegistrator<Attribute> BoolAttributeRegistrator(&AttributeCollection::AttributeFactory, "bool", new BoolAttributeInstantiator());




    Attribute* IntAttributeInstantiator::Instantiate()
    {
        return new IntAttribute();
    }

    IntAttribute::IntAttribute()
        : Attribute()
    {
        Value = 0;
    }

    IntAttribute::IntAttribute(int value)
        : Attribute()
    {
        Value = value;
    }

    bool IntAttribute::LoadFromXml(XML* xml)
    {
        string innertext = xml->InnerText(true);
        Value = atoi(innertext.c_str());
        return true;
    }

    void IntAttribute::WriteToStream(ostream& os, int indentlevel)
    {
        string indent(indentlevel*2, ' ');
        os << indent << "<int>" << Value << "</int>\n";
    }

    Attribute* IntAttribute::Clone()
    {
        return new IntAttribute(Value);
    }

    FactoryRegistrator<Attribute> IntAttributeRegistrator(&AttributeCollection::AttributeFactory, "int", new IntAttributeInstantiator());



    Attribute* FloatAttributeInstantiator::Instantiate()
    {
        return new FloatAttribute();
    }

    FloatAttribute::FloatAttribute()
        : Attribute()
    {
        Value = 0.0f;
    }

    FloatAttribute::FloatAttribute(float value)
        : Attribute()
    {
        Value = value;
    }

    bool FloatAttribute::LoadFromXml(XML* xml)
    {
        string innertext = xml->InnerText(true);
        Value = atof(innertext.c_str());
        return true;
    }

    void FloatAttribute::WriteToStream(ostream& os, int indentlevel)
    {
        string indent(indentlevel*2, ' ');
        os << indent << "<float>" << Value << "</float>\n";
    }

    Attribute* FloatAttribute::Clone()
    {
        return new FloatAttribute(Value);
    }

    FactoryRegistrator<Attribute> FloatAttributeRegistrator(&AttributeCollection::AttributeFactory, "float", new FloatAttributeInstantiator());




    Attribute* StringAttributeInstantiator::Instantiate()
    {
        return new StringAttribute();
    }

    StringAttribute::StringAttribute()
        : Attribute()
    {
        Value = "";
    }

    StringAttribute::StringAttribute(string value)
        : Attribute()
    {
        Value = value;
    }

    bool StringAttribute::LoadFromXml(XML* xml)
    {
        Value = xml->InnerText(true);
        return true;
    }

    void StringAttribute::WriteToStream(ostream& os, int indentlevel)
    {
        string indent(indentlevel*2, ' ');
        os << indent << "<string>" << Value << "</string>\n";
    }

    Attribute* StringAttribute::Clone()
    {
        return new StringAttribute(Value);
    }

    FactoryRegistrator<Attribute> StringAttributeRegistrator(&AttributeCollection::AttributeFactory, "string", new StringAttributeInstantiator());





    Attribute* VecAttributeInstantiator::Instantiate()
    {
        return new VecAttribute();
    }

    VecAttribute::VecAttribute()
        : Attribute()
    {
    }

    VecAttribute::VecAttribute(const list<Attribute*>& vec)
        : Attribute()
    {
        for(list<Attribute*>::const_iterator i = vec.begin(); i != vec.end(); i++)
            Value.push_back((*i)->Clone());
    }

    VecAttribute::~VecAttribute()
    {
        for(list<Attribute*>::iterator i = Value.begin(); i != Value.end(); i++)
            delete *i;
    }

    bool VecAttribute::LoadFromXml(XML* xml)
    {
        for(list<XML_Element*>::iterator child = xml->children.begin(); child != xml->children.end(); child++)
        {
   			XML* xChild = dynamic_cast<XML*>(*child);
            if(xChild == NULL)
                continue;

            Attribute* attribute = AttributeCollection::AttributeFactory.Produce(xChild->name);
            if(attribute == NULL)
                continue;

            if(!attribute->LoadFromXml(xChild))
            {
                delete attribute;
                continue;
            }

            Value.push_back(attribute);
        }
        return true;
    }

    void VecAttribute::WriteToStream(ostream& os, int indentlevel)
    {
        string indent(indentlevel*2, ' ');
        os << indent << "<vec>\n";
        for(list<Attribute*>::iterator i = Value.begin(); i != Value.end(); i++)
            (*i)->WriteToStream(os, indentlevel+1);
        os << indent << "</vec>\n";
    }

    Attribute* VecAttribute::Clone()
    {
        return new VecAttribute(Value);
    }

    FactoryRegistrator<Attribute> VecAttributeRegistrator(&AttributeCollection::AttributeFactory, "vec", new VecAttributeInstantiator());
