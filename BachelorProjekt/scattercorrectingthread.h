#ifndef SCATTERCORRECTINGTHREAD_H
#define SCATTERCORRECTINGTHREAD_H

#include <QThread>

#include "cbctrecon.h"
#include "cbctregistration.h"
#include "cbctregistration_test.hpp"

class ScatterCorrectingThread : public QThread
{
    Q_OBJECT
public:
    explicit ScatterCorrectingThread(Scui *parent=nullptr);
    void run();
    bool Stop;
    Scui* m_parent; // Used to call objects and methods in the main class
    CbctRecon* m_cbctrecon; // Main object to acces
    CbctRegistration* m_cbctregistration; // just for convienience

private slots:
    void SLT_DoRegistrationRigid();
    void SLT_DoRegistrationDeform();
    ctType get_ctType(const QString &selText);
    void SLT_IntensityNormCBCT();
    void SLT_DoScatterCorrection_APRIORI();
    void SLT_IntensityNormCBCT_COR_CBCT();
    FDK_options getFDKoptions() const;
    void SLT_ManualMoveByDCMPlanOpen();
    void SLT_ConfirmManualRegistration();

signals:
    void Signal_UpdateLabelRaw(QString);
    void Signal_UpdateLabelCor(QString);
    void Signal_PassFixedImg();
    void Signal_UpdateProgressBarSC(int);
    void Signal_UpdateVOICombobox(ctType);
    void Signal_FixedImageSelected(QString);
    void Signal_MovingImageSelected(QString);

private:
    int m_enViewArrange{};
};

#endif // SCATTERCORRECTINGTHREAD_H
