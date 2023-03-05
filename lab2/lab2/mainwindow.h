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
    void load_from_file_button_clicked(QString filename);
    // void back_button_clicked();

private slots:
    void on_exitButton_clicked();
    void on_loadFromFileButton_clicked();
    // void on_backButton_clicked();

private:
    Ui::MainWindow *ui;
    Controller *controller;
};
#endif // MAINWINDOW_H
