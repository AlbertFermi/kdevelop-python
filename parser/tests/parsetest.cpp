/* Python Parser Test
 *
 * Copyright 2007 Andreas Pakulat <apaku@gmx.de>
 *
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

#include "parsetest.h"
#include "pythondriver.h"

#include <QDebug>

QTEST_MAIN( ParseTest )

ParseTest::ParseTest( QObject* parent )
    : QObject( parent )
{}

ParseTest::~ParseTest()
{}

void ParseTest::successSimpleSource()
{
    QFETCH( QString, project );
    bool ret = PythonDriver::parse_string( project );
    QVERIFY( ret == true );
}

void ParseTest::successSimpleSource_data()
{
    QTest::addColumn<QString>( "project" );
    QTest::newRow( "row1" ) << "class b:\n  pass\n";
}


void ParseTest::successSimpleSourceIndent()
{
    QFETCH( QString, project );
    bool ret = PythonDriver::parse_string( project );
    QVERIFY( ret == true );
}

void ParseTest::successSimpleSourceIndent_data()
{
    QTest::addColumn<QString>( "project" );
    QTest::newRow( "row1" ) << "class b:\n\t  pass\nclass c:\n  \tpass\n";
}

void ParseTest::init()
{
}

void ParseTest::cleanup()
{
}


#include "parsetest.moc"

// kate: space-indent on; indent-width 4; tab-width: 4; replace-tabs on; auto-insert-doxygen on