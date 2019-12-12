/********************************************************************************
** Form generated from reading UI file 'MainEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINEDITOR_H
#define UI_MAINEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainEditor
{
public:
    QWidget *centralwidget;
    QPushButton *test_button;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainEditor)
    {
        if (MainEditor->objectName().isEmpty())
            MainEditor->setObjectName(QStringLiteral("MainEditor"));
        MainEditor->resize(800, 600);
        centralwidget = new QWidget(MainEditor);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        test_button = new QPushButton(centralwidget);
        test_button->setObjectName(QStringLiteral("test_button"));
        test_button->setGeometry(QRect(200, 80, 341, 161));
        MainEditor->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainEditor);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainEditor->setMenuBar(menubar);
        statusbar = new QStatusBar(MainEditor);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainEditor->setStatusBar(statusbar);

        retranslateUi(MainEditor);

        QMetaObject::connectSlotsByName(MainEditor);
    } // setupUi

    void retranslateUi(QMainWindow *MainEditor)
    {
        MainEditor->setWindowTitle(QApplication::translate("MainEditor", "MainEditor", Q_NULLPTR));
        test_button->setText(QApplication::translate("MainEditor", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainEditor: public Ui_MainEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINEDITOR_H
