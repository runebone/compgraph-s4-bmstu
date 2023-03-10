#include "model.h"

#include <QDebug>

Model::Model(QObject *parent) : QObject(parent) {}

void Model::setData(ModelData &modelData)
{
    m_modelData = modelData;
    emit updated(m_modelData);
}

ModelData Model::getData() const {
    return m_modelData;
}

ModelMemento Model::createMemento() const {
    return ModelMemento(m_modelData);
}

void Model::restoreMemento(const ModelMemento &memento) {
    m_modelData = memento.getData();
    emit memento_restored(m_modelData);
}

#include "algorithm.h"
err_t Model::solve()
{
    err_t status = Algorithm::solve(m_modelData.firstSetPoints, m_modelData.secondSetPoints, m_modelData.solutionData);

    if (!isNotError(status)) // XXX better call isError
    {
        m_modelData.solutionData.set_status(status);
    }
    emit updated(m_modelData);

    return status;
}

Point Model::get_point(size_t index, Set set) const
{
    Point point;

    // XXX DEBUG
    if (set == FIRST && index >= m_modelData.firstSetPoints.size()
            || set == SECOND && index >= m_modelData.secondSetPoints.size())
    {
        qDebug() << "Error: Index out of bounds! (get point)";
        return { 0.0, 0.0 };
    }

    if (set == FIRST) {
        auto &vec = m_modelData.firstSetPoints;
        point = vec.at(index);
    } else if (set == SECOND) {
        auto &vec = m_modelData.secondSetPoints;
        point = vec.at(index);
    }

    return point;
}

void Model::add_point(const Point &point, Set set)
{
    if (set == FIRST) {
        m_modelData.firstSetPoints.push_back(point);
        emit updated(m_modelData);
    } else if (set == SECOND) {
        m_modelData.secondSetPoints.push_back(point);
        emit updated(m_modelData);
    }
}

void Model::remove_point(size_t index, Set set)
{
    // XXX DEBUG
    qDebug() << "Index:" << index << "Size of fsp:" << m_modelData.firstSetPoints.size();
    if (set == FIRST && index >= m_modelData.firstSetPoints.size()
            || set == SECOND && index >= m_modelData.secondSetPoints.size())
    {
        qDebug() << "Error: Index out of bounds! (remove point)";
        return;
    }

    if (set == FIRST) {
        auto &vec = m_modelData.firstSetPoints;
        vec.erase(vec.begin() + index);
        emit updated(m_modelData);
    } else if (set == SECOND) {
        auto &vec = m_modelData.secondSetPoints;
        vec.erase(vec.begin() + index);
        emit updated(m_modelData);
    }
}

void Model::update_point(size_t index, const Point &point, Set set)
{
    // XXX DEBUG
    if (set == FIRST && index >= m_modelData.firstSetPoints.size()
            || set == SECOND && index >= m_modelData.secondSetPoints.size())
    {
        qDebug() << "Error: Index out of bounds! (update point)";
        return;
    }

    if (set == FIRST) {
        auto &vec = m_modelData.firstSetPoints;
        vec.at(index) = point;
        emit updated(m_modelData); // For some reason program crushes when changing coordinates from scrollareas
        // emit dbg_updated();
        // dbg_emit_updated();
    } else if (set == SECOND) {
        auto &vec = m_modelData.secondSetPoints;
        vec.at(index) = point;
        emit updated(m_modelData);
        // emit dbg_updated();
        // dbg_emit_updated();
    }
}

void Model::dbg_update_point(size_t index, const Point &point, Set set)
{
    // XXX DEBUG
    if (set == FIRST && index >= m_modelData.firstSetPoints.size()
            || set == SECOND && index >= m_modelData.secondSetPoints.size())
    {
        qDebug() << "Error: Index out of bounds! (update point)";
        return;
    }

    if (set == FIRST) {
        auto &vec = m_modelData.firstSetPoints;
        vec.at(index) = point;
        emit dbg_updated();
    } else if (set == SECOND) {
        auto &vec = m_modelData.secondSetPoints;
        vec.at(index) = point;
        emit dbg_updated();
    }
}

void Model::replace_points(const std::vector<Point> &points, Set set)
{
    if (set == FIRST) {
        auto &vec = m_modelData.firstSetPoints;
        vec = points;
        emit updated(m_modelData);
    } else if (set == SECOND) {
        auto &vec = m_modelData.secondSetPoints;
        vec = points;
        emit updated(m_modelData);
    }
}

void Model::dbg_emit_updated()
{
    emit updated(m_modelData);
}

ModelMemento::ModelMemento(ModelData modelData) {
    m_modelData = modelData;
}

ModelData ModelMemento::getData() const {
    return m_modelData;
}

#define HISTORY_SIZE 100
void History::push(const ModelMemento &memento) {
    if (m_mementos.size() == HISTORY_SIZE) {
        m_mementos.erase(m_mementos.begin() + 1);
    }
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
