/*****************************************************************************
 * Copyright (c) 2007 Piyush verma <piyush.verma@gmail.com>                  *
 *   Copyright 2007 Andreas Pakulat <apaku@gmx.de>                           *
 *                                                                           *
 * Permission is hereby granted, free of charge, to any person obtaining     *
 * a copy of this software and associated documentation files (the           *
 * "Software"), to deal in the Software without restriction, including       *
 * without limitation the rights to use, copy, modify, merge, publish,       *
 * distribute, sublicense, and/or sell copies of the Software, and to        *
 * permit persons to whom the Software is furnished to do so, subject to     *
 * the following conditions:                                                 *
 *                                                                           *
 * The above copyright notice and this permission notice shall be            *
 * included in all copies or substantial portions of the Software.           *
 *                                                                           *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,           *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF        *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                     *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE    *
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION    *
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION     *
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.           *
 *****************************************************************************/
#include "pythonhighlighting.h"

#include <ktexteditor/smartrange.h>

#include <language/duchain/topducontext.h>
#include <language/duchain/declaration.h>
#include <language/duchain/definition.h>
#include <language/duchain/use.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>

using namespace KTextEditor;

using namespace KDevelop;

namespace Python
{

Highlighting::Highlighting( QObject * parent )
        : QObject( parent )
{
}

Highlighting::~Highlighting( )
{
}

KTextEditor::Attribute::Ptr Highlighting::attributeForType( Types type, Contexts context ) const
{
    KTextEditor::Attribute::Ptr a;
    a = m_definitionAttributes[type];

    if ( !a )
    {
        a = KTextEditor::Attribute::Ptr( new KTextEditor::Attribute() );
        a->setBackgroundFillWhitespace( true );
        m_definitionAttributes.insert( type, a );

        switch ( type )
        {

        case NamespaceType:
            a->setBackground( QColor( Qt::green ).light( 170 ) );
            break;

        case ClassType:
            a->setBackground( QColor( Qt::blue ).light( 175 ) );
            break;

        case FunctionType:
            a->setBackground( QColor( Qt::green ).light( 175 ) );
            break;

        case FunctionVariableType:
            a->setBackground( QColor( Qt::blue ).light( 175 ) );
            break;

        case NamespaceVariableType:
            a->setBackground( QColor( Qt::red ).light( 175 ) );
            break;

        case ClassVariableType:
            a->setBackground( QColor( Qt::green ).light( 165 ) );
            break;

        default:
            a->setBackground( QColor( Qt::green ).light( 175 ) );
            break;
        }

        a->setFontBold();
    }

    return a;
}

void Highlighting::highlightTree( KTextEditor::SmartRange * range ) const
{
    foreach( KTextEditor::SmartRange* child, range->childRanges() )
    highlightTree( child );
}

void Highlighting::outputRange( KTextEditor::SmartRange * range ) const
{
    Q_ASSERT( range->start() <= range->end() );
    foreach( SmartRange* child, range->childRanges() )
    outputRange( child );
}

void Highlighting::highlightDUChain( KDevelop::TopDUContext* context ) const
{
    DUChainReadLocker lock( DUChain::lock() );
    Q_ASSERT( context->topContext() == context );
    highlightDUChain( static_cast<DUContext*>( context ) );
}

void Highlighting::highlightDUChain( DUContext* context ) const
{

    kDebug() << "Highlighting duchain";

    if ( !context->smartRange() )
    {
        kDebug() << "Ooops, no smart range, somethings broken";
        return;
    }

    kDebug() << "Highlighting declarations:" << context->localDeclarations();

    foreach( Declaration* dec, context->localDeclarations() )
    highlightDeclaration( dec );
    kDebug() << "Highlighting definitions:" << context->localDefinitions();
    foreach( Definition* def, context->localDefinitions() )
    highlightDefinition( def );
    kDebug() << "Highlighting child contexts:" << context->childContexts();
    foreach( DUContext* child, context->childContexts() )
    highlightDUChain( child );
}


Highlighting::Types Highlighting::typeForDeclaration( Declaration * dec ) const
{
    Types type;

    switch ( dec->context()->type() )
    {

    case DUContext::Class:
        type = ClassVariableType;
        break;

    case DUContext::Function:
        type = FunctionVariableType;
        break;

    case DUContext::Namespace:
        type = NamespaceVariableType;
        break;

    default:
        break;
    }

    return type;
}

void Highlighting::highlightDefinition( Definition * definition ) const
{
    if ( Declaration* declaration = definition->declaration() )
        if ( SmartRange * range = definition->smartRange() )
            range->setAttribute( attributeForType( typeForDeclaration( declaration ), DeclarationContext ) );
}

void Highlighting::highlightDeclaration( Declaration * declaration ) const
{
    if ( SmartRange* range = declaration->smartRange() )
        range->setAttribute( attributeForType( typeForDeclaration( declaration ), DeclarationContext ) );
}

void Highlighting::highlightUse( KDevelop::Use* ) const
{
}

}

#include "pythonhighlighting.moc"
