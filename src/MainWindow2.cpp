//
// Created by Tudor Oancea on 02/05/2021.
//

#include "MainWindow2.hpp"

#include <QMainWindow>
#include <QBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>

MainWindow2::MainWindow2()
	: lhs(new QTableWidget(100,3, this)),
	  rhs(new QVBoxLayout),
	  label1(new QLabel("label1")),
	  label2(new QLabel("label2")),
	  label3(new QLabel("label3"))
{
	rhs->addWidget(label1, 1, Qt::AlignCenter);
	rhs->addWidget(label2, 1, Qt::AlignCenter);
	rhs->addWidget(label3, 1, Qt::AlignCenter);

	for (int i(0); i < 100; ++i) {
		for (int j(0); j < 3; ++j) {
			auto newItem = new QTableWidgetItem(QString("%1 / %2").arg(i).arg(j));
			//if (i == 0 && j == 0) newItem->setText(" blaaaa aaaa");
			lhs->setItem(i,j,newItem);
			lhs->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
			lhs->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		}
	}

	lhs->setHorizontalHeaderLabels(QStringList({"time", "ao5", "ao12"}));
	lhs->setWordWrap(false);
	lhs->resizeColumnsToContents();
	lhs->resizeRowsToContents();
	lhs->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	auto* mainLayout = new QHBoxLayout;
	mainLayout->addWidget(lhs, 0);
	mainLayout->addLayout(rhs, 1);

	this->setCentralWidget(new QWidget);
	this->setMinimumSize(700, 500);
	this->centralWidget()->setLayout(mainLayout);
}

MainWindow2::~MainWindow2() {
	delete lhs;
	delete rhs;
	delete label1;
	delete label2;
	delete label3;
}

