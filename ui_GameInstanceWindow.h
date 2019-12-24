/********************************************************************************
** Form generated from reading UI file 'GameInstanceWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEINSTANCEWINDOW_H
#define UI_GAMEINSTANCEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameInstanceWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;

    void setupUi(QMainWindow *GameInstanceWindow)
    {
        if (GameInstanceWindow->objectName().isEmpty())
            GameInstanceWindow->setObjectName(QStringLiteral("GameInstanceWindow"));
        GameInstanceWindow->resize(1024, 500);
        centralwidget = new QWidget(GameInstanceWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(pushButton, 3, 0, 1, 1);

        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 0, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        GameInstanceWindow->setCentralWidget(centralwidget);

        retranslateUi(GameInstanceWindow);

        QMetaObject::connectSlotsByName(GameInstanceWindow);
    } // setupUi

    void retranslateUi(QMainWindow *GameInstanceWindow)
    {
        GameInstanceWindow->setWindowTitle(QApplication::translate("GameInstanceWindow", "MainEditor", Q_NULLPTR));
        pushButton->setText(QApplication::translate("GameInstanceWindow", "PushButton", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("GameInstanceWindow", "PushButton", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("GameInstanceWindow", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GameInstanceWindow: public Ui_GameInstanceWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEINSTANCEWINDOW_H
