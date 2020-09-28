#ifndef SEARCHMANAGERSTATUS_H
#define SEARCHMANAGERSTATUS_H


#include <QObject>


namespace Dal {


struct SearchManagerStatusClass {
    Q_GADGET

public:
    SearchManagerStatusClass() = delete;

    enum Value
    {
        Starting,
        Running,
        Stopping,
        Stopped
    };
    Q_ENUM(Value)
};

using SearchManagerStatus = SearchManagerStatusClass::Value;


} // namespace Dal

#endif // SEARCHMANAGERSTATUS_H
