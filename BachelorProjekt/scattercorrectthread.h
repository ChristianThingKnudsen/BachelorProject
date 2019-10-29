#ifndef SCATTERCORRECTTHREAD_H
#define SCATTERCORRECTTHREAD_H

#include <QThread>

#include "cbctrecon.h"
#include "cbctregistration.h"
#include "cbctregistration_test.hpp"

class ScatterCorrectThread : public QThread
{
    Q_OBJECT
public:
    // For threading
    explicit ScatterCorrectThread(MainWindow *parent=nullptr);
    void run();
    bool Stop;

    // Relevant variables
    MainWindow* m_parent;
    CbctRecon* m_cbctrecon;
    CbctRegistration* m_cbctregistration; // just for convienience
    //std::unique_ptr<CbctRegistrationTest> m_dlgRegistration;

private slots:
    //Some slots...
    void SLT_DoRegistrationRigid();
    void SLT_DoRegistrationDeform();
    void SLT_KeyMoving(const bool bChecked);
    void SelectComboExternal(const int idx, const enREGI_IMAGES iImage);
    void UpdateListOfComboBox(const int idx) const;
    //void SLT_FixedImageSelected(QString selText);
    //void SLT_MovingImageSelected(QString selText);
    ctType get_ctType(const QString &selText);
    void UpdateVOICombobox(const ctType ct_type) const;
    //void LoadImgFromComboBox(const int idx, QString &strSelectedComboTxt);
    //void SLT_DrawImageWhenSliceChange() const;
    //void SLT_DrawImageInFixedSlice() const;
    void SLT_IntensityNormCBCT();
    void SLT_DoScatterCorrection_APRIORI();
    void SLT_IntensityNormCBCT_COR_CBCT();
    void SLT_InitializeGraphLim() const;
    FDK_options getFDKoptions() const; // This is not implemented...
    void SLT_ManualMoveByDCMPlanOpen();
    //void ImageManualMoveOneShot(const float shiftX, const float shiftY, const float shiftZ);
    //void SLT_PassFixedImgForAnalysis(QString);

signals:
    //Some signals...
    void SigalUpdateLabel(int, QString);
    void SignalPassFixedImg(QString);
    void SignalDrawImageInFixedSlice() const;
    void SignalDrawImageWhenSliceChange();

private:
    // Some private variables or pointers...
    int m_enViewArrange{};

    //YK16GrayImage *m_YKImgFixed;
    //YK16GrayImage *m_YKImgMoving;
public:
    /*UShortImageType::Pointer m_spFixedImg;  // pointer only, for display
    UShortImageType::Pointer m_spMovingImg; // pointer only, for display
    UShortImageType::Pointer m_spFixedDose;  // pointer only, for display
    UShortImageType::Pointer m_spMovingDose; // pointer only, for display*/
    //AG17RGBAImage *m_DoseImgFixed;
    //AG17RGBAImage *m_DoseImgMoving;
    //AG17RGBAImage *m_AGDisp_Overlay;
};

#endif // SCATTERCORRECTTHREAD_H
