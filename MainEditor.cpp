#include "MainEditor.h"
#include "ui_MainEditor.h"

#include <iostream>

MainEditor::MainEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainEditor)
{
    ui->setupUi(this);
}

MainEditor::~MainEditor()
{
    delete ui;
}

void MainEditor::on_test_button_clicked()
{
    std::cout << "asd" << std::endl;
}
