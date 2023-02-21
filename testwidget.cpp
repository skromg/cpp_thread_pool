//
// Created by swh on 2/20/23.
//
#include <QDebug>
#include <iostream>
#include "testwidget.h"

TestWidget::TestWidget(QWidget *parent) : QWidget(parent), thread_pool_(5)
{
    InitUI();
}

TestWidget::~TestWidget()
{
}

void TestWidget::InitUI()
{
    this->resize(200, 100);
    btn_ = new QPushButton(this);
    btn_->setText("insert task");
    connect(btn_, &QPushButton::clicked, this, &TestWidget::OnBtnClicked);
    btn_->setGeometry(50, 50, btn_->width(), btn_->height());
}

void TestWidget::OnBtnClicked()
{
    //    thread_pool_.EnqueueTask();
}
