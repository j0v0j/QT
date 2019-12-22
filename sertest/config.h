#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>

class CONFIG
{
public:
    CONFIG();
    void WriteSettings();
    void ReadSettings();
private:

};

#endif // CONFIG_H
