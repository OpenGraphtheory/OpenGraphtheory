
#include "../Headers/attributes.h"

using namespace std;
using namespace OpenGraphtheory::XML;

/// \defgroup arbitraryattributes Arbitrary Attributes
// @{

    OpenGraphtheory::Factory<Attribute> AttributeCollection::AttributeFactory;

    /// \brief Destructor - it also deletes all Attribute instances it contains (no need to handle this outside of the class)
    AttributeCollection::~AttributeCollection()
    {
        Clear();
    }

    /// \brief Adds an Attribute (taken from an XML structure) to the collection
    void AttributeCollection::Set(XML* attr)
    {
        std::string name = attr->GetAttribute("name");
        if(name == "")
            return;

        // iterate the child-elements of the xml element
        for(list<XML_Element*>::iterator child = attr->children.begin(); child != attr->children.end(); child++)
        {
            XML* xChild = dynamic_cast<XML*>(*child);
            if(xChild == NULL)
                continue;

            // instantiate the object
            Attribute* attribute = AttributeFactory.Produce(xChild->name);
            if(attribute == NULL)
                continue;

            // give the xml object to the new instance (it shall load itself from the xml data)
            if(!attribute->LoadFromXml(xChild))
            {
                delete attribute;
                continue;
            }

            // put the new object into the collection
            Unset(name);
            Attributes[name] = attribute;
            break;
        }
    }

    Attribute* AttributeCollection::Add(string name, string type)
    {
        Unset(name);
        Attribute* attribute = AttributeFactory.Produce(type);
        if(attribute != NULL)
            Attributes[name] = attribute;
        return attribute;
    }

    /// \brief Removes all Attribute instances and deletes them
    void AttributeCollection::Clear()
    {
        for(map<string, Attribute*>::const_iterator i = Attributes.begin(); i != Attributes.end(); i++)
            delete i->second;
        Attributes.clear();
    }

    /// \brief Copies all Attribute instances from the given AttributeCollection instance
    void AttributeCollection::operator=(const AttributeCollection& attrs)
    {
        Clear();
        for(map<string, Attribute*>::const_iterator i = attrs.Attributes.begin(); i != attrs.Attributes.end(); i++)
            Attributes[i->first] = i->second->Clone();
    }

    /// \brief Saves all Attribute instances to an XML structure
    void AttributeCollection::WriteToXml(XML* xml)
    {
        for(map<string, Attribute*>::const_iterator i = Attributes.begin(); i != Attributes.end(); i++)
        {
            XML* attr = new XML("attr");
            attr->AddAttribute("name", i->first);
            i->second->WriteToXml(attr);
            xml->AddChild(attr);
        }
    }

    /// \brief Removes an Attribute from the collection and deletes it
    void AttributeCollection::Unset(string name)
    {
        map<string, Attribute*>::iterator it = Attributes.find(name);
        if(it == Attributes.end())
            return;
        delete it->second;
        Attributes.erase(it);
    }

    /// \brief Query whether the collection contains an Attribute with a given name
    bool AttributeCollection::HasAttribute(string name)
    {
        map<string, Attribute*>::iterator attr = Attributes.find(name);
        return attr != Attributes.end();
    }

    /// \brief Get the Attribute that has the given name
    /// \return A pointer to the Attribute (or NULL if it is not present)
    /// \note To test, whether the Attribute is of a certain Type, use dynamic_cast, e.g.
    /// for(OpenGraphtheory::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
    /// {
    ///     Attribute* pFoo = v.Attributes().GetAttribute("foo");
    ///     IntAttribute* iFoo = dynamic_cast<IntAttribute*>(pFoo);
    ///     if(iFoo == NULL) // Attribute has wrong type
    ///         continue;
    ///     cout << iFoo->Value << " ";
    /// }
    Attribute* AttributeCollection::GetAttribute(string name)
    {
        map<string, Attribute*>::iterator attr = Attributes.find(name);
        if(attr == Attributes.end())
            return NULL;

        return attr->second;
    }

// @}


/// \brief (virtual) destructor - needed to avoid memory leaks
Attribute::~Attribute()
{

}


/// \defgroup boolattribute Boolean Attributes
/// \ingroup arbitraryattributes
/// @{

    /// \brief Constructor
    BoolAttribute::BoolAttribute()
        : Attribute()
    {
        Value = false;
    }

    /// \brief Constructor
    BoolAttribute::BoolAttribute(bool value)
        : Attribute()
    {
        Value = value;
    }

    /// \brief Override LoadFromXml method
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

    /// \brief Override WriteToXml method
    void BoolAttribute::WriteToXml(XML* xml)
    {
        XML* child = new XML("bool");
        child->AddChild(new XML_Text(Value ? "true" : "false"));
        xml->AddChild(child);
    }

    /// \brief Override Clone method
    Attribute* BoolAttribute::Clone()
    {
        return new BoolAttribute(Value);
    }

    /// \brief Registers the AttributeInstantiator in the Factory immediately when the library is loaded
    OpenGraphtheory::FactoryRegistrator<Attribute> BoolAttributeRegistrator(&AttributeCollection::AttributeFactory, "bool",
        new OpenGraphtheory::DefaultInstantiator<Attribute, BoolAttribute>());

/// @}


/// \defgroup intattribute Integral Attributes
/// \ingroup arbitraryattributes
/// @{

    /// \brief Constructor
    IntAttribute::IntAttribute()
        : Attribute()
    {
        Value = 0;
    }

    /// \brief Constructor
    IntAttribute::IntAttribute(int value)
        : Attribute()
    {
        Value = value;
    }

    /// \brief Override LoadFromXml method
    bool IntAttribute::LoadFromXml(XML* xml)
    {
        string innertext = xml->InnerText(true);
        stringstream s;
        s << innertext;
        s >> Value;
        return true;
    }

    /// \brief Override WriteToXml method
    void IntAttribute::WriteToXml(XML* xml)
    {
        XML* child = new XML("int");
        stringstream s;
        s << Value;
        child->AddChild(new XML_Text(s.str()));
        xml->AddChild(child);
    }

    /// \brief Override Clone method
    Attribute* IntAttribute::Clone()
    {
        return new IntAttribute(Value);
    }

    /// \brief Registers the AttributeInstantiator in the Factory immediately when the library is loaded
    OpenGraphtheory::FactoryRegistrator<Attribute> IntAttributeRegistrator(&AttributeCollection::AttributeFactory, "int",
        new OpenGraphtheory::DefaultInstantiator<Attribute, IntAttribute>());

/// @}


/// \defgroup floatattribute Floatingpoint Attributes
/// \ingroup arbitraryattributes
/// @{

    /// \brief Constructor
    FloatAttribute::FloatAttribute()
        : Attribute()
    {
        Value = 0.0f;
    }

    /// \brief Constructor
    FloatAttribute::FloatAttribute(float value)
        : Attribute()
    {
        Value = value;
    }

    /// \brief Override LoadFromXml method
    bool FloatAttribute::LoadFromXml(XML* xml)
    {
        string innertext = xml->InnerText(true);
        stringstream s;
        s << innertext;
        s >> Value;
        return true;
    }

    /// \brief Override WriteToXml method
    void FloatAttribute::WriteToXml(XML* xml)
    {
        XML* child = new XML("float");
        stringstream s;
        s << Value;
        child->AddChild(new XML_Text(s.str()));
        xml->AddChild(child);
    }

    /// \brief Override Clone method
    Attribute* FloatAttribute::Clone()
    {
        return new FloatAttribute(Value);
    }

    /// \brief Registers the AttributeInstantiator in the Factory immediately when the library is loaded
    OpenGraphtheory::FactoryRegistrator<Attribute> FloatAttributeRegistrator(&AttributeCollection::AttributeFactory, "float",
        new OpenGraphtheory::DefaultInstantiator<Attribute, FloatAttribute>());

/// @}



/// \defgroup stringattribute String Attributes
/// \ingroup arbitraryattributes
/// @{

    /// \brief Constructor
    StringAttribute::StringAttribute()
        : Attribute()
    {
        Value = "";
    }

    /// \brief Constructor
    StringAttribute::StringAttribute(string value)
        : Attribute()
    {
        Value = value;
    }

    /// \brief Override LoadFromXml method
    bool StringAttribute::LoadFromXml(XML* xml)
    {
        Value = xml->InnerText(true);
        return true;
    }

    /// \brief Override WriteToXml method
    void StringAttribute::WriteToXml(XML* xml)
    {
        XML* child = new XML("string");
        child->AddChild(new XML_Text(Value));
        xml->AddChild(child);
    }

    /// \brief Override Clone method
    Attribute* StringAttribute::Clone()
    {
        return new StringAttribute(Value);
    }

    /// \brief Registers the AttributeInstantiator in the Factory immediately when the library is loaded
    OpenGraphtheory::FactoryRegistrator<Attribute> StringAttributeRegistrator(&AttributeCollection::AttributeFactory, "string",
        new OpenGraphtheory::DefaultInstantiator<Attribute, StringAttribute>());

/// @}


/// \defgroup seqattribute Sequential Attributes
/// \ingroup arbitraryattributes
/// @{

    /// \brief Constructor
    SeqAttribute::SeqAttribute()
        : Attribute()
    {
    }

    /// \brief Constructor
    SeqAttribute::SeqAttribute(const list<Attribute*>& vec)
        : Attribute()
    {
        for(list<Attribute*>::const_iterator i = vec.begin(); i != vec.end(); i++)
            Value.push_back((*i)->Clone());
    }

    /// \brief Destructor
    SeqAttribute::~SeqAttribute()
    {
        for(list<Attribute*>::iterator i = Value.begin(); i != Value.end(); i++)
            delete *i;
    }

    /// \brief Override LoadFromXml method
    bool SeqAttribute::LoadFromXml(XML* xml)
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

    /// \brief Override WriteToXml method
    void SeqAttribute::WriteToXml(XML* xml)
    {
        XML* child = new XML("seq");
        for(list<Attribute*>::iterator i = Value.begin(); i != Value.end(); i++)
            (*i)->WriteToXml(child);
        xml->AddChild(child);
    }

    /// \brief Override Clone method
    Attribute* SeqAttribute::Clone()
    {
        return new SeqAttribute(Value);
    }

    /// \brief Registers the AttributeInstantiator in the Factory immediately when the library is loaded
    OpenGraphtheory::FactoryRegistrator<Attribute> SeqAttributeRegistrator(&AttributeCollection::AttributeFactory, "seq",
        new OpenGraphtheory::DefaultInstantiator<Attribute, SeqAttribute>());

/// @}
