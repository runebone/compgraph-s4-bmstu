#ifndef MODEL_H
#define MODEL_H

#include <QObject>

#include <vector>
#include "point.h"
#include "error.h"
#include "solution_data.h"

class ModelData {
    std::vector<Point> firstSetPoints;
    std::vector<Point> secondSetPoints;

    SolutionData solutionData;
};

class ModelMemento {
public:
    ModelMemento(ModelData &modelData);

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

private:
    ModelData m_modelData;

public slots:

signals:
    void updated();
};

class History {
public:
    void push(const ModelMemento& memento);
    ModelMemento pop();

private:
    std::vector<ModelMemento> m_mementos;
};

#endif // MODEL_H
