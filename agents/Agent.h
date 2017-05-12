#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QtQml/QQmlEngine>
#include <QMetaType>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlRecord>
#include <QStringList>
#include <QPointer>
#include <QImage>
#include <QMutex>
#include <QTimer>

#include "utils/geometry/GSSCoordinate.h"
#include "utils/geometry/GSSGeometry.h"
#include "utils/exporters/custom_json_exporter/CustomJsonExporter.h"

#include "utils/exporters/debugger/Debugger.h"
#include "utils/storage/ObjectStorage.h"
#include "utils/uistyle/UiStyle.h"

QT_FORWARD_DECLARE_CLASS(Environment)
QT_FORWARD_DECLARE_CLASS(Skill)

class Agent : public QObject
{
    friend class Environment; // Environment will be able to overwrite the attributes
    friend class ExecutionEnvironment; // ExecutionEnvironment will be able to overwrite the attributes
    friend class PhysicalEnvironment; // PhyisicalEnvironment will be able to overwrite the attributes (geometry and node)
    friend class SocialEnvironment; // SocialEnvironment will be able to overwrite the attributes
    friend class ClimateEnvironment; // ClimateEnvironment will be able to overwrite the attributes
    friend class TimeEnvironment; // TimeEnvironment will be able to overwrite the attributes

    Q_OBJECT // Required for every child!! Adds Qt extra functionality like SIGNAL/SLOTS
    // DECLARE PROPERTIES TO BE CREATED FROM QML
    Q_PROPERTY(QDateTime born_datetime READ getBornDatetime)

public:
    ~Agent();

    // EXPORTERS
    virtual QJsonObject toMiniJSON();
    virtual QJsonObject toJSON(); // Called when asked for more info about this agent. Can be overwritten if want to add extra variables to the exporters
    virtual QImage toImage( const GSSEnvelope image_bounds , unsigned int image_width = 1024 , unsigned int image_height = 1024 );

    // IMPORTERS
    virtual void fromSQLRecord(QSqlRecord record); // From record received from database
    virtual void fromJSON(QJsonObject json); // Update agent according to parameters sent from importers

    // GETTERS
    unsigned int getId() const;
    const GSSGeometry* getGeometry() const;
    GSSCoordinate getRepresentativeCoordinate() const;
    bool isRunning() const;
    UiStyle* getStyle() const;
    QDateTime getBornDatetime() const;
        // ENVIRONMENT
    Environment* getEnvironment() const;
        // SKILLS
    bool hasSkill( QString class_name ) const;
    Skill* getSkill( QString class_name ) const;
    QList< Skill* > getSkills( QString class_name ) const;

    // SETTERS
    virtual void setGeometry( GSSGeometry* geometry );
    void setNextTick( QDateTime datetime );
    void setNextTick( TimeUnit seconds = TimeUnit(1) );
    void addSkill( Skill* skill );
    void removeSkill( Skill* skill );

    // SLOTS, try always to invoke them by SLOT, it will make to be executed in the agent's thread
private slots:
    void run();
    void behaveWrapper();
    virtual void behave(){} // Behaviour, To be implemented by children, must be synchronous
    void stop();

signals:
    // Start and end signals to be overwriten by children
    void agentStartedSignal( QString class_name , int id );
    void agentEndedSignal( QString class_name , int id );
    void agentBehavedSignal( QString class_name , int id );

protected:
    // Abstract agent, cant be created
    explicit Agent(GSSGeometry* geom = 0, QObject* parent = 0);
    Agent( QJsonObject json, QObject* parent = 0 );

    /**
     * @brief Agent skills container
     */
    ObjectStorage* skills = 0; // QMAP < SKILL_NAME , QLIST<SKILL*> >

    /**
      * Pointer to the environment where it is living
      */
    Environment* environment = 0;

    /**
     * @brief born_datetime Agent born/creation datetime
     */
    QDateTime born_datetime; // Agent creation/borning datetime

    /**
     * Color, icon style...
     **/
    UiStyle* style = 0;

    /**
     * @brief next_operation_datetime Not to call this agent's tick untill this datetime has been reached
     * To be compared with the TimeEnvironment and call this agent's tick
     */
    QDateTime next_tick_datetime;

    /**
      * Mutex for paralelism
      */
    mutable QMutex mutex;

private:

    // AUTOINCREMENTAL FOR IDS
    static unsigned int counter;

    // ALL ELEMENTS COMMON VARIABLES
    /**
     * @brief id
     */
    const unsigned int id; // Element id

    /**
     * @brief Agent's geometry
     */
    GSSGeometry* geometry = 0; // Agents geometry

    /**
     * @brief timer
     * Each running agent MUST have its timer, to schedule its slots.
     * It is not good to singleshot many slots in one timer, so each agent will have its own
     */
    QTimer* timer = 0;

    /**
     * @brief running Flag Will only be changed when calling RUN or STOP
     */
    bool running = false;

    /**
     * @brief busy Flag Will only be changed by environment
     */
    bool busy = false;
};

#endif // AGENT_H
