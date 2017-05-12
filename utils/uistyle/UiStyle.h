#ifndef UISTYLE_H
#define UISTYLE_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>
#include <QImage>
#include <QColor>

class UiStyle : public QObject
{
    Q_OBJECT

public:
    explicit UiStyle( QObject *parent = 0 );
    ~UiStyle();

    QJsonObject toJSON();
    void fromJSON(QJsonObject json);

    // METHODS
    static QColor randomColor();
    static QString toHTML(QColor color);

    // VARIABLES
    QColor color; // Agent geometry inner color
    QColor border_color; // Agent geometry border color
    int border_weight; // Agent geometry border weight
    QString dash_array; // Agent geometry line format
    QString icon_url; // Agent icon_url (only for points)
    QImage icon; // Agent image
    double zoom_level; // Zoom level from which on to appear
};



#endif // UISTYLE_H
