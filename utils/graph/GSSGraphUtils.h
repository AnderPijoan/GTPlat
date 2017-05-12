#ifndef GSSGRAPHUTILS_H
#define GSSGRAPHUTILS_H

#include <QObject>

#include "utils/graph/GSSGraph.h"

class GSSGraphUtils : public QObject
{
    Q_OBJECT

public:

    static GSSGraphUtils* getInstance();

    // METHODS
    QMap<GSSGraphEdge* , QList<GSSGraphEdge*> >  createSimplification( QMap<GSSGraphEdge*,double> cost_map);

private:
    explicit GSSGraphUtils(QObject *parent = 0);
    GSSGraphUtils(GSSGraphUtils const&);
    ~GSSGraphUtils();
};

#endif // GSSGRAPHUTILS_H
