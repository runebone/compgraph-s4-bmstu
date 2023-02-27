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
    Model *model;
    View *view;

    History *model_caretaker;

signals:
    void model_solve();
    void model_add_point1();
    void model_add_point2();
    void model_load_from_file(QString filename);

public slots:
    void on_solve_button_clicked();
    void on_add1_button_clicked();
    void on_add2_button_clicked();
    void on_load_from_file_button_clicked(QString filename);
    void on_back_button_clicked();
    void on_clear_screen_clicked();

    void on_model_updated();

    // Accept MyPointWidget signals
    // void mpw_x_input_changed(int point_index, Set set, const QString &str);
    // void mpw_y_input_changed(int point_index, Set set, const QString &str);
    // void mpw_group_radiobutton_toggled(int point_index, Set set);
    // void mpw_remove_button_clicked(int point_index, Set set);
};

#endif // CONTROLLER_H
