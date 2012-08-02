#ifndef EDITACTION_H
#define EDITACTION_H

#include "../../../opengt.so/Headers/opengt.h"
using namespace OpenGraphtheory;

class EditAction
{
    public:
        virtual ~EditAction();

        virtual void Undo() = 0;
        virtual void Redo() = 0;
};

class VertexCreated : public EditAction
{
    private:
        int ID;
        int x;
        int y;
    public:
        void Undo();
        void Redo();
};

class EdgeCreated : public EditAction
{
    private:
        void Undo();
        void Redo();
};

#endif // EDITACTION_H
