#ifndef NERDIOSREMOTE_H
#define NERDIOSREMOTE_H

#include <QObject>

class NerdiosRemote : public QObject
{
    Q_OBJECT
public:
    explicit NerdiosRemote(QObject *parent = 0);

signals:

public slots:
};

#endif // NERDIOSREMOTE_H