/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2018 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

#include <QtTest/QtTest>
#include "schemaparser.h"

class SchemaParserTest: public QObject {
  private:
    Q_OBJECT

  private slots:
		void testExpressionEvaluationWithCasts(void);
};

void SchemaParserTest::testExpressionEvaluationWithCasts(void)
{
	SchemaParser schparser;
	QString buffer;
	attribs_map attribs;

	buffer = "%set {ver} 10.0\n";
	buffer += "\n%if ({ver} >f \"9.3\") %then";
	buffer += "\n 10.0";
	buffer += "\n%else";
	buffer += "\n 9.3";
	buffer += "\n%end";

	try
	{
		schparser.loadBuffer(buffer);
		QCOMPARE(schparser.getCodeDefinition(attribs) == "10.0", true);
	}
	catch(Exception &e)
	{
		QFAIL(e.getExceptionsText().toStdString().c_str());
	}
}

QTEST_MAIN(SchemaParserTest)
#include "schemaparsertest.moc"
