#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "model.h"
#include "view.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(Model *model, View *view, QObject *parent = nullptr);
    ~Controller();

private:
    Model *m_model;
    View *m_view;
    History *m_history;

public slots:
    void on_model_updated();

    void on_solve_button_clicked();
    void on_add_button_1_clicked();
    void on_add_button_2_clicked();
    void on_load_from_file_button_clicked(QString);
    void on_back_button_clicked();
    void on_clear_screen_clicked();

    void on_key_pressed(QKeyEvent*);
    void on_left_mouse_clicked(QMouseEvent*);
    void on_right_mouse_clicked(QMouseEvent*);
    void on_middle_mouse_clicked(QMouseEvent*);

signals:
    void solve_button_clicked();
    void clear_screen_clicked();

    void key_pressed(QKeyEvent*);
    void left_mouse_clicked(QMouseEvent*);
    void right_mouse_clicked(QMouseEvent*);
    void middle_mouse_clicked(QMouseEvent*);
};

#endif // CONTROLLER_H
