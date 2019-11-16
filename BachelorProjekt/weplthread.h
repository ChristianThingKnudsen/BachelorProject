#ifndef WEPLTHREAD_H
#define WEPLTHREAD_H

#include <QThread>

#include "cbctrecon.h"
#include "cbctregistration.h"
#include "cbctregistration_test.hpp"

//QT
#include <QStandardItemModel>

class WEPLThread: public QThread
{
    Q_OBJECT
public:
    // For threading
    explicit WEPLThread(Scui *parent=nullptr);
    void run();
    Scui* m_parent;
    bool Stop = false;

    CbctRecon* m_cbctrecon;
    CbctRegistration* m_cbctregistration; // just for convienience

public slots:
    void SLT_WEPLcalc(QString);
    ctType get_ctType(const QString &selText);
signals:
    void Signal_DrawWEPL();
    void Signal_UpdateProgressBarWEPL(int);
};

#endif // WEPLTHREAD_H
