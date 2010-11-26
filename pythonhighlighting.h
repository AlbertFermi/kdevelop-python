/*****************************************************************************
 * Copyright (c) 2007 Piyush verma <piyush.verma@gmail.com>                  *
 *   Copyright 2007 Andreas Pakulat <apaku@gmx.de>                           *
 *                                                                           *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *****************************************************************************/
#ifndef KDEVCPPHIGHLIGHTING_H
#define KDEVCPPHIGHLIGHTING_H

#include <QObject>
#include <QHash>
#include <QModelIndex>

#include <ktexteditor/attribute.h>
#include <language/interfaces/icodehighlighting.h>

namespace KTextEditor
{

class SmartRange;
}

namespace KDevelop
{

class DUContext;

class Declaration;
}

namespace Python
{

class Highlighting : public QObject, public KDevelop::ICodeHighlighting
{
    Q_OBJECT
    Q_INTERFACES( KDevelop::ICodeHighlighting )

public:

    enum Types
    {
        FunctionType,
        ClassType,
        FunctionVariableType,
        ClassVariableType,
        LocalVariableType,
        MemberVariableType,
        GlobalVariableType
    };
    enum Contexts
    {
        DefinitionContext,
        DeclarationContext,
        NamespaceContext
    };
    Highlighting( QObject* parent );
    virtual ~Highlighting();

    void highlightTree( KTextEditor::SmartRange* topRange ) const;
    void highlightDUChain( KDevelop::TopDUContext* context ) const;

    virtual void highlightDeclaration( KDevelop::Declaration* declaration ) const;
    virtual void highlightUses( KDevelop::DUContext* ) const;
    KTextEditor::Attribute::Ptr attributeForType( Types type, Contexts context ) const;

private:
    void highlightDUChain( KDevelop::DUContext* context ) const;
    void outputRange( KTextEditor::SmartRange * range ) const;

    Types typeForDeclaration( KDevelop::Declaration* dec ) const;

    mutable QHash<Types, KTextEditor::Attribute::Ptr> m_definitionAttributes;
    mutable QHash<Types, KTextEditor::Attribute::Ptr> m_declarationAttributes;
};

}

#endif
