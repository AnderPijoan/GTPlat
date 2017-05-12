#ifndef WASTECONSTANTS
#define WASTECONSTANTS

#include <QObject>

class WasteConstants : public QObject
{
    Q_OBJECT

public:

    // ENUM
    enum Fractions {
        WASTE ,
        PLASTIC_PACKAGING ,
        GLASS_BOTTLES ,
        PAPER ,
        ORGANIC
    };

};

#endif // WASTECONSTANTS

