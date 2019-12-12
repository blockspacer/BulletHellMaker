#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainEditor; }
QT_END_NAMESPACE

class MainEditor : public QMainWindow
{
    Q_OBJECT

public:
    MainEditor(QWidget *parent = nullptr);
    ~MainEditor();

private slots:

    void on_test_button_clicked();

private:
    Ui::MainEditor *ui;
};
#endif // MAINEDITOR_H
