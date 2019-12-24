#include "GameInstanceWindow.h"
#include "ui_GameInstanceWindow.h"

#include <iostream>

GameInstanceWindow::GameInstanceWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameInstanceWindow)
{
    ui->setupUi(this);
}

GameInstanceWindow::~GameInstanceWindow()
{
    delete ui;
}

QLayout * GameInstanceWindow::getLayout() {
	return ui->horizontalLayout;
}

void GameInstanceWindow::on_test_button_clicked()
{
    std::cout << "asd" << std::endl;
}
