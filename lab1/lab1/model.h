#ifndef MODEL_H
#define MODEL_H

#include <QObject>

#include <vector>
#include "point.h"
#include "error.h"
#include "solution_data.h"
#include "set_enum.h"

class ModelData {
public:
    std::vector<Point> firstSetPoints;
    std::vector<Point> secondSetPoints;

    SolutionData solutionData;
};

class ModelMemento {
public:
    ModelMemento(ModelData modelData);

    ModelData getData() const;

private:
    ModelData m_modelData;
};

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    void setData(ModelData &modelData);
    ModelData getData() const;

    ModelMemento createMemento() const;
    void restoreMemento(const ModelMemento& memento);

    err_t solve();

    Point get_point(size_t index, Set set) const;
    void add_point(const Point &point, Set set);
    void remove_point(size_t index, Set set);
    void update_point(size_t index, const Point &point, Set set);
    void replace_points(const std::vector<Point> &points, Set set);

    void dbg_emit_updated();

private:
    ModelData m_modelData;

signals:
    void updated(ModelData);
    void memento_restored(ModelData);
    void dbg_updated();
};

class History {
public:
    void push(const ModelMemento& memento);
    ModelMemento pop();
    ModelMemento back() const;
    size_t size() const;

private:
    std::vector<ModelMemento> m_mementos;
};

#endif // MODEL_H
