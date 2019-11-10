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
    Scui* m_parent;
    CbctRecon* m_cbctrecon;
    CbctRegistration* m_cbctregistration; // just for convienience

private slots:
    void SLT_DoRegistrationRigid();
    void SLT_DoRegistrationDeform();
    void SLT_KeyMoving(const bool bChecked);
    ctType get_ctType(const QString &selText);
    void SLT_IntensityNormCBCT();
    void SLT_DoScatterCorrection_APRIORI();
    void SLT_IntensityNormCBCT_COR_CBCT();
    void SLT_InitializeGraphLim() const;
    FDK_options getFDKoptions() const; // This is not implemented...
    void SLT_ManualMoveByDCMPlanOpen();
    void SLT_ConfirmManualRegistration();

signals:
    //Some signals...
    void Signal_UpdateLabelRaw(QString);
    void Signal_UpdateLabelCor(QString);
    void SignalPassFixedImg(QString);
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
