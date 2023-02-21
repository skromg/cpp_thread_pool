//
// Created by swh on 2/20/23.
//

#ifndef THREAD_POOL_TESTWIDGET_H
#define THREAD_POOL_TESTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "threadpool.h"

class TestWidget : public QWidget
{
public:
    explicit TestWidget(QWidget *parent);
    ~TestWidget() override;

private Q_SLOTS:
    void OnBtnClicked();

private:
    void InitUI();

private:
    QPushButton *btn_{ nullptr };
    ThreadPool thread_pool_;
};

#endif // THREAD_POOL_TESTWIDGET_H
