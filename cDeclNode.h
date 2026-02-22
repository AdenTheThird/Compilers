#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}

        virtual bool IsArray()  { return false; }
        virtual bool IsStruct() { return false; }
        virtual bool IsType()   { return false; }
        virtual bool IsFunc()   { return false; }
        virtual bool IsVar()    { return false; }
        virtual bool IsFloat()  { return false; }
        virtual bool IsInt()    { return false; }
        virtual bool IsChar()   { return false; }
        virtual int  GetSize()  { return 0; }
        virtual cDeclNode *GetType() = 0;

        cSymbol* GetTypeSymbol()
        {
            return dynamic_cast<cSymbol*>(GetChild(0));
        }

    bool IsCompatibleWith(cDeclNode* rhs)
    {
        // exact match
        if (this->GetType() == rhs->GetType()) return true;

        // same category, smaller → larger is ok
        if (IsInt() && rhs->IsInt() && this->GetSize() >= rhs->GetSize()) return true;
        if (IsFloat() && rhs->IsFloat() && this->GetSize() >= rhs->GetSize()) return true;

        // allow int → float promotion
        if (IsFloat() && rhs->IsInt()) return true;

        // everything else is invalid
        return false;
    }

        virtual string GetTypeName() = 0;

};
