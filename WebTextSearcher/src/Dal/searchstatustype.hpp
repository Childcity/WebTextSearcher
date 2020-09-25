#ifndef SEARCHSTATUSTYPE_H
#define SEARCHSTATUSTYPE_H

#include <QObject>


namespace Dal {


struct SearchStatusTypeClass {
    Q_GADGET

public:
    SearchStatusTypeClass() = delete;

    enum Value
    {
        NotSet,
        Found,
        NotFound,
        Downloading,
        Error
    };
    Q_ENUM(Value)
};

using SearchStatusType = SearchStatusTypeClass::Value;


} // namespace Dal


#endif // SEARCHSTATUSTYPE_H
