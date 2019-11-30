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
    // For threading
    explicit ScatterCorrectingThread(Scui *parent=nullptr);
    void run();
    bool Stop;

    // Relevant variables
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
    FDK_options getFDKoptions() const; // This is not implemented...
    void SLT_ManualMoveByDCMPlanOpen();
    void SLT_ConfirmManualRegistration();

signals:
    void Signal_UpdateLabelRaw(QString);
    void Signal_UpdateLabelCor(QString);
    void Signal_PassFixedImg();
    void SignalDrawImageInFixedSlice() const;
    void SignalDrawImageWhenSliceChange();
    void Signal_UpdateProgressBarSC(int);
    void Signal_SCThreadIsDone();
    void Signal_UpdateVOICombobox(ctType);
    void Signal_FixedImageSelected(QString);
    void Signal_MovingImageSelected(QString);
    void Signal_ImageManualMoveOneShot(float,float,float);

private:
    int m_enViewArrange{};
};

#endif // SCATTERCORRECTINGTHREAD_H
