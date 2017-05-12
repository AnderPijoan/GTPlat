#ifndef ELEVATIONSKILL_H
#define ELEVATIONSKILL_H

#include <QVector>

#include "geos/geom/Point.h"
#include "skills/Skill.h"
#include "utils/db/PostgresDriver.h"

using namespace geos::geom;

class ElevationSkill : public Skill
{
        Q_OBJECT
public:

    ElevationSkill(QString url, int port, QString db_name, QString db_user, QString db_pass);
    ~ElevationSkill();

    // THIS skill methods
    double getPointElevation(Point* point);

    PostgresDriver* db_driver;
};

#endif // ELEVATIONSKILL_H
