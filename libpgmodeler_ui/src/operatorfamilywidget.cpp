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

#include "operatorfamilywidget.h"

OperatorFamilyWidget::OperatorFamilyWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_OPFAMILY)
{
	QStringList types;
	map<QString, vector<QWidget *> > fields_map;
	map<QWidget *, vector<QString> > values_map;
	QFrame *frame=nullptr;

	Ui_OperatorFamilyWidget::setupUi(this);
	configureFormLayout(opfamily_grid, OBJ_OPFAMILY);

	IndexingType::getTypes(types);
	indexing_cmb->addItems(types);

	setRequiredField(indexing_lbl);
	fields_map[BaseObjectWidget::generateVersionsInterval(BaseObjectWidget::AFTER_VERSION, PgSQLVersions::PGSQL_VERSION_95)].push_back(indexing_lbl);
	values_map[indexing_lbl].push_back(~IndexingType(IndexingType::brin));

	opfamily_grid->addItem(new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Expanding), opfamily_grid->count()+1, 0, 1, 0);

	frame=BaseObjectWidget::generateVersionWarningFrame(fields_map, &values_map);
	frame->setParent(this);
	opfamily_grid->addWidget(frame, opfamily_grid->count()+1, 0, 1, 5);

	configureTabOrder();
	setMinimumSize(500, 250);
}

void OperatorFamilyWidget::hideEvent(QHideEvent *event)
{
	indexing_cmb->setCurrentIndex(0);
	BaseObjectWidget::hideEvent(event);
}

void OperatorFamilyWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, OperatorFamily *op_family)
{
	BaseObjectWidget::setAttributes(model, op_list, op_family, schema);

	if(op_family)
		indexing_cmb->setCurrentIndex(indexing_cmb->findText(~(op_family->getIndexingType())));
}

void OperatorFamilyWidget::applyConfiguration(void)
{
	try
	{
		OperatorFamily *op_family=nullptr;

		startConfiguration<OperatorFamily>();
		op_family=dynamic_cast<OperatorFamily *>(this->object);
		op_family->setIndexingType(IndexingType(indexing_cmb->currentText()));

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

