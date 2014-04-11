
#ifndef __OPENGRAPHTHEORY_IMPORT_GML_GML_H
    #define __OPENGRAPHTHEORY_IMPORT_GML_GML_H

    #include <string>
	#include <map>
	#include <algorithm>
    #include "../import.h"

    namespace OpenGraphtheory
    {
        namespace Import
        {
            class ImportFilterGML : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterGMLRegistrator;
                public:
                    Graph Import(std::istream& is);
            };

            class GMLValue
            {
                public:
                    GMLValue();
                    virtual ~GMLValue();
            };

            class GMLTreeNode
            {
                public:
                    GMLTreeNode();
                    ~GMLTreeNode();

                    std::list<std::pair<std::string, GMLValue*> > Children;
            };

            class GMLValueInt : public GMLValue
            {
                public:
                    GMLValueInt(int value);
                    ~GMLValueInt();
                    int value;
            };

            class GMLValueFloat : public GMLValue
            {
                public:
                    GMLValueFloat(float value);
                    ~GMLValueFloat();
                    float value;
            };

            class GMLValueString : public GMLValue
            {
                public:
                    GMLValueString(std::string value);
                    ~GMLValueString();
                    std::string value;
            };

            class GMLValueNode : public GMLValue
            {
                public:
                    GMLValueNode(GMLTreeNode* value);
                    ~GMLValueNode();
                    GMLTreeNode* value;
            };

        } // namespace Import
    } // namespace OpenGraphtheory

#endif
