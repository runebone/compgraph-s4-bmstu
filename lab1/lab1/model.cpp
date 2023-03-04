#include "model.h"

Model::Model(QObject *parent) : QObject(parent) {}

void Model::setData(ModelData &modelData)
{
    m_modelData = modelData;
    emit updated();
}

ModelData Model::getData() const {
    return m_modelData;
}

ModelMemento Model::createMemento() const {
    return Memento(m_modelData);
}

void Model::restoreMemento(const ModelMemento &memento) {
    m_modelData = memento.getData();
    emit updated();
}

#include "algorithm.h"
err_t Model::solve()
{
    err_t status = Algorithm::solve(m_modelData.firstSetPoints, m_modelData.secondSetPoints, m_modelData.solutionData);

    if (isNotError(status))
    {
        emit updated();
    }

    return status;
}

void Model::add_point(const Point &point, Set set)
{
    if (set == FIRST) {
        m_modelData.firstSetPoints.push_back(point);
        emit updated();
    } else if (set == SECOND) {
        m_modelData.secondSetPoints.push_back(point);
        emit updated();
    }
}

void Model::remove_point(size_t index, Set set)
{
    // XXX DEBUG
    if (set == FIRST && index >= m_modelData.firstSetPoints.size()
            || set == SECOND && index >= m_modelData.secondSetPoints.size())
    {
        qDebug() << "Error: Index out of bounds!";
        return;
    }

    if (set == FIRST) {
        auto &vec = m_modelData.firstSetPoints;
        vec.erase(vec.begin() + index);
        emit updated();
    } else if (set == SECOND) {
        auto &vec = m_modelData.secondSetPoints;
        vec.erase(vec.begin() + index);
        emit updated();
    }
}

void Model::replace_points(const std::vector<Point> &points, Set set)
{
    if (set == FIRST) {
        auto &vec = m_modelData.firstSetPoints;
        vec = points;
        emit updated();
    } else if (set == SECOND) {
        auto &vec = m_modelData.secondSetPoints;
        vec = points;
        emit updated();
    }
}

ModelMemento::ModelMemento(ModelData &modelData) {
    m_modelData = modelData;
}

ModelData ModelMemento::getData() const {
    return m_modelData;
}

void History::push(const ModelMemento &memento) {
    m_mementos.push_back(memento);
}

ModelMemento History::pop() {
    ModelMemento memento = m_mementos.back();
    m_mementos.pop_back();

    return memento;
}

ModelMemento History::back() const
{
    return m_mementos.back();
}

size_t History::size() const
{
    return m_mementos.size();
}
