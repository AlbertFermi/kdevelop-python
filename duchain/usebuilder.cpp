/*****************************************************************************
 * Copyright (c) 2007 Piyush verma <piyush.verma@gmail.com>                  *
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
#include "usebuilder.h"
#include "pythoneditorintegrator.h"

#include <ktexteditor/smartrange.h>
#include <ktexteditor/smartinterface.h>

#include <language/duchain/declaration.h>
#include <language/duchain/use.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>
#include <parsesession.h>
#include <usebuilder.h>
#include "ast.h"

using namespace KTextEditor;
using namespace KDevelop;

namespace Python {

UseBuilder::UseBuilder (PythonEditorIntegrator* editor) : m_editor(editor)
{
}

// void UseBuilder::buildUses(Ast *node)
// {
//     supportBuild(node);
// //     if (TopDUContext* top = dynamic_cast<TopDUContext*>(m_session->getNode(node)))
// //         top->setHasUses(true);
// }

void UseBuilder::visitName(NameAst* node)
{
    DUChainWriteLocker lock(DUChain::lock());
    QList<Declaration*> declarations = currentContext()->findDeclarations(identifierForNode(node->identifier), editorFindRange(node, node).start);
    if ( ! declarations.length() ) return;
    Declaration* dec = declarations.last();
    if ( node->context == ExpressionAst::Load ) {
        UseBuilderBase::newUse(node, dec);
    }
}

void UseBuilder::visitIdentifier(Identifier* node)
{
    DUChainWriteLocker lock( DUChain::lock() );
    QualifiedIdentifier id = identifierForNode(node);
    RangeInRevision range = editorFindRange(node, node);
    CursorInRevision until = range.start;
    QList<Declaration*> allDeclarations = currentContext()->findDeclarations(id, until);
    
    kDebug() << " >> scanning " << node->value;
    kDebug() << "  > searching for declaration until" << until.line << ":" << until.column << "; " << allDeclarations.length() << "Declarations found";
    
    Declaration *globalDeclaration = 0;
    foreach ( Declaration* dec, allDeclarations ) {
        if ( dec->context() == dec->topContext() ) {
            kDebug() << "There's already a global declaration for" << node->value;
            globalDeclaration = dec;
        }
    }
    
    // if there's a local declaration, use the last one of those
    if ( allDeclarations.length() && allDeclarations.last()->context() != allDeclarations.last()->topContext() ) {
        kDebug() << " ++ Created a use of local declaration for node" << node->value;
        UseBuilderBase::newUse(node, allDeclarations.last());
    }
    // otherwise, use the global one.
    // Note that the following is not allowed by python: a=3; def foo(): print a; a=7
    else if ( globalDeclaration ) {
        kDebug() << " ++ Created a use of global declaration for node" << node->value;
        UseBuilderBase::newUse(node, globalDeclaration);
    }
}

// void UseBuilder::visitIdentifierTarget(IdentifierTargetAst* node)
// {
//     kDebug() << "Target variable identifier: " << node->identifier->identifier.toAscii();
//     UseBuilderBase::visitIdentifierTarget(node);
// }


void UseBuilder::openContext(DUContext * newContext)
{
  UseBuilderBase::openContext(newContext);
  m_nextUseStack.push(0);
}

void UseBuilder::closeContext()
{
  UseBuilderBase::closeContext();
  m_nextUseStack.pop();
}


ParseSession *UseBuilder::parseSession() const
{
    return m_session;
}

}
// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
