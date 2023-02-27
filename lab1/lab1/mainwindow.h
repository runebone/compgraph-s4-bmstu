#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void solve_button_clicked();
    void add1_button_clicked();
    void add2_button_clicked();
    void load_from_file_button_clicked(QString filename);
    void back_button_clicked();

    void key_pressed(QKeyEvent *event);

private slots:

    void on_actionTask_triggered();

    void on_solveButton_clicked();

    void on_exitButton_clicked();

    void on_addPoint1Button_clicked();

    void on_addPoint2Button_clicked();

    void on_loadFromFileButton_clicked();

    void on_backButton_clicked();

private:
    Ui::MainWindow *ui;
    Controller *controller;

    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
