#ifndef GAMEINSTANCEWINDOW_H
#define GAMEINSTANCEWINDOW_H

#include <QMainWindow>
#include <QLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class GameInstanceWindow; }
QT_END_NAMESPACE

class GameInstanceWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameInstanceWindow(QWidget *parent = nullptr);
    ~GameInstanceWindow();

	QLayout* getLayout();

private slots:

    void on_test_button_clicked();

private:
    Ui::GameInstanceWindow *ui;
};
#endif // MAINEDITOR_H
