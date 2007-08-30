/* KDevelop Python Support
 *
 * Copyright 2007 Andreas Pakulat <apaku@gmx.de>
 *
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
 */

#ifndef PYTHONLEXER_H
#define PYTHONLEXER_H

#include <cstddef>
#include <QtCore/QStack>
#include <QtCore/QString>

class QString;

namespace Python
{

class parser;

class Lexer {
public:
    Lexer(parser* _parser, const QString& contents);

    int nextTokenKind();
    std::size_t tokenBegin() const;
    std::size_t tokenEnd() const;

private:
    QString m_content;
    parser* m_parser;
    int m_curpos;
    int m_contentSize;
    std::size_t m_tokenBegin;
    std::size_t m_tokenEnd;
    unsigned int m_openParenNum;

    int state() const;
    void pushState(int state);
    void popState();

    int indentation() const;
    void pushIndentation( int indentation );
    void popIndentation();

    QChar* ignoreWhitespaceAndComments( QChar* it );
    void createNewline( int curpos );
    bool isStringStart( QChar* );

    QStack<int> m_state;
    QStack<int> m_indentation;
    enum State
    {
        ErrorState = -1,
        DefaultState = 0,
        IndentState = 1
    };


};

}

#endif

// kate: space-indent on; indent-width 4; tab-width: 4; replace-tabs on; auto-insert-doxygen on
