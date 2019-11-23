#include "scui.h"
#include "ui_scui.h"

#include "qyklabel.h"
#include "progressbar.h"
#include "informationwindow.h"

// QT
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>


// PLM
#include "mha_io.h"

//Local
#include "cbctrecon_io.h"
#include "loadingthread.h"
#include "scattercorrectingthread.h"
#include "weplthread.h"

//For loading CBCTRecon
#include <iostream>
#include <shellapi.h>

enum enCOLOR {
  RED,
  BLUE,
};

Scui::Scui(QWidget *parent) // Constructor
    : QMainWindow(parent)
    , ui(new Ui::Scui)

{   
    ui->setupUi(this);

    ui->comboBox_region->addItem("Head-Neck");
    ui->comboBox_region->addItem("Pelvis");
    ui->comboBox_region->addItem("Thorax");
    ui->comboBoxWEPL->addItem("No structures");
    ui->comboBoxPlanView->addItem("Axial");
    ui->comboBoxPlanView->addItem("Frontal");
    ui->comboBoxPlanView->addItem("Sagital");
    // Icon for Load Data
    QPixmap pixmapLoad("C:\\Users\\ct-10\\OneDrive - Aarhus universitet\\7 Semester ST\\Bachelor\\UI_Kode\\BachelorProject\\pictures\\upload.png");
    QIcon ButtonLoad(pixmapLoad);
    ui->btnLoadData->setIcon(ButtonLoad);
    ui->btnLoadData->setIconSize(QSize(20,20));//pixmap.rect().size();

    // Icon for Info button
    QPixmap pixmapInfo("C:\\Users\\ct-10\\OneDrive - Aarhus universitet\\7 Semester ST\\Bachelor\\UI_Kode\\BachelorProject\\pictures\\information.png");
    QIcon ButtonInfo(pixmapInfo);
    ui->btnInfo->setIcon(ButtonInfo);
    ui->btnInfo->setIconSize(QSize(30,30));

    //Color for combobox
    QPalette p = ui->comboBox_region->palette();
    p.setColor(QPalette::Highlight, Qt::transparent);
    ui->comboBox_region->setPalette(p);

    this->m_cbctrecon = std::make_unique<CbctRecon>();
    this->m_cbctregistration =
        std::make_unique<CbctRegistration>(this->m_cbctrecon.get());

    m_YKImgFixed = &m_cbctregistration->m_YKImgFixed[0];
    m_YKImgMoving = &m_cbctregistration->m_YKImgMoving[0];
    m_YKDisp = &m_cbctregistration->m_YKDisp[0];

    m_YKDispRaw = &m_cbctregistration->m_YKDisp[0];
    m_YKImgRawFixed = &m_cbctregistration->m_YKImgFixed[0];
    m_YKImgRawMoving = &m_cbctregistration->m_YKImgMoving[0];

    m_DoseImgFixed = &m_cbctregistration->m_DoseImgFixed[0];
    m_DoseImgMoving = &m_cbctregistration->m_DoseImgMoving[0];
    m_AGDisp_Overlay = &m_cbctregistration->m_AGDisp_Overlay[0];

    lThread = new LoadingThread(this);
    connect(lThread,SIGNAL(SignalMessageBox(int,QString,QString)), this, SLOT(ShowMessageBox(int, QString, Qstring)));
    connect(lThread,SIGNAL(Signal_SetButtonsAfterLoad()),this, SLOT(SLT_SetButtonsAfterLoad()));
    connect(lThread,SIGNAL(Signal_UpdateSlider(int)), this, SLOT(SLT_UpdateSlider(int)));
    connect(lThread,SIGNAL(Signal_DisconnectSlider()), this, SLOT(SLT_DisconnectSlider()));
    connect(lThread,SIGNAL(Signal_ReConnectSlider(int)),this,SLOT(SLT_ReConnectSlider(int)));
    connect(lThread,SIGNAL(Signal_UpdateProgressBarLoad(int)),this,SLOT(SLT_UpdateProgressBarLoad(int)));
    connect(lThread,SIGNAL(Signal_LThreadIsDone()),this,SLOT(SLT_LThreadIsDone()));
    //connect(lThread,SIGNAL(finished()),lThread,SLOT(quit()));


    scThread = new ScatterCorrectingThread(this);
    connect(scThread,SIGNAL(Signal_UpdateLabelRaw(QString)), this, SLOT(SLT_UpdateLabelRaw(QString)));
    connect(scThread,SIGNAL(Signal_UpdateLabelCor(QString)), this, SLOT(SLT_UpdateLabelCor(QString)));
    connect(scThread,SIGNAL(SignalPassFixedImg(QString)),this, SLOT(SLT_PassFixedImgForAnalysis(QString)));

    connect(scThread,SIGNAL(SignalDrawImageInFixedSlice()),this,SLOT(SLT_DrawImageInFixedSlice()));
    connect(scThread,SIGNAL(SignalDrawImageWhenSliceChange()),this,SLOT(SLT_DrawImageWhenSliceChange()));
    connect(scThread,SIGNAL(Signal_UpdateProgressBarSC(int)),this,SLOT(SLT_UpdateProgressBarSC(int)));
    connect(scThread,SIGNAL(Signal_SCThreadIsDone()),this,SLOT(SLT_SCThreadIsDone()));
    connect(scThread,SIGNAL(Signal_UpdateVOICombobox(ctType)),this,SLOT(UpdateVOICombobox(const ctType)));
    connect(scThread,SIGNAL(Signal_FixedImageSelected(QString)),this,SLOT(SLT_FixedImageSelected(QString)));
    connect(scThread,SIGNAL(Signal_MovingImageSelected(QString)),this,SLOT(SLT_MovingImageSelected(QString)));
    connect(scThread,SIGNAL(Signal_ImageManualMoveOneShot(float,float,float)),this,SLOT(ImageManualMoveOneShot(const float,const float,const float)));
    //connect(scThread,SIGNAL(finished()),lThread,SLOT(quit()));

    weplThread = new WEPLThread(this);
    connect(weplThread,SIGNAL(Signal_DrawWEPL()),this,SLOT(SLT_DrawImageWhenSliceChange()));
    connect(weplThread,SIGNAL(Signal_UpdateProgressBarWEPL(int)),this,SLOT(SLT_UpdateProgressBarWEPL(int)));
    //connect(weplThread,SIGNAL(finished()),lThread,SLOT(quit()));


}

Scui::~Scui() // Destructor
{
    delete ui;
    delete lThread;
    delete scThread;
    delete weplThread;
}
void Scui::SLT_OpenInfo() // Is called when the info button is pushed
{
    InformationWindow infoWindow;
    infoWindow.setModal(true);
    infoWindow.exec();
    ui->labelImageRaw->resize(675,675);
}
void Scui::SLT_OpenAdvancedMode() // Is called when the advanced button is pushed
{
    //Link: https://stackoverflow.com/questions/15435994/how-do-i-open-an-exe-from-another-c-exe
    ShellExecute(NULL, "open", "C:\\Users\\ct-10\\CbctRecon\\build-vs19-mt\\bin\\CbctRecon.exe", NULL, NULL, SW_MAXIMIZE);
}
void Scui::SLT_Exit() // Is called when the exit button is pushed
{
    delete ui;
    delete ui;
}
void Scui::SLT_IncreaseSliderValue() // Is called when the + button is pushed
{
    auto curValue = ui->verticalSlider->value();
    ui->verticalSlider->setValue(curValue+1);
}
void Scui::SLT_DecreaseSliderValue() // Is called when the - button is pushed
{
    auto curValue = ui->verticalSlider->value();
    ui->verticalSlider->setValue(curValue-1);
}
void Scui::SLT_SliderValueChanged(){
    if(scatterCorrectingIsDone){
        SLT_DrawReconImageInSlices();
        SLT_DrawImageWhenSliceChange();
    }else{
        SLT_DrawReconImage();
        SLT_DrawImageWhenSliceChange();
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------Threading methods -----------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Scui::SLT_StartLoadingThread(){
    //SLT_GetCBCTPath();
    CBCTPath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\2019-07-04_084333_2019-07-04 06-43-22-2985\\1ba28724-69b3-4963-9736-e8ab0788c31f\\Acquisitions\\781076550");
    //SLT_GetCTPath();
    CTPath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\Plan CT\\E_PT1 plan");
    ui->btnLoadData->setEnabled(false);
    lThread->start();
}

void Scui::SLT_StartScatterCorrectingThread(){
    ui->comboBox_region->setEnabled(false);
    ui->comboBox_region->setStyleSheet("QComboBox{font-weight: bold;font-size: 18px;background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));color: rgba(255,255,255,60%);border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QComboBox QAbstractItemView{selection-background-color: rgba(255,190,56,100%);}QComboBox::drop-down{border: 0px;}QComboBox::down-arrow {image: url(/Users/ct-10/Desktop/down.png);width: 14px;height: 14px;}");
    ui->btnScatterCorrect->setEnabled(false);
    scThread->start();
}

void Scui::SLT_StartWEPLThread(){
    weplThread->start();
}

void Scui::SLT_ShowMessageBox(int idx, QString header,QString message){

    if(idx == 1){
        QMessageBox::warning(this, header, message);
        return;
    }
    QMessageBox::warning(this, "warning", "Error on File Name Sorting!");
    return;
}

void Scui::SLT_InitializeSlider(FDK_options fdkoptions){
    this->ui->verticalSlider->setMinimum(1);//this->ui.spinBoxReconImgSliceNo->setMinimum(0);
    this->ui->verticalSlider->setMaximum(fdkoptions.ct_size[1] - 1);//this->ui.spinBoxReconImgSliceNo->setMaximum(fdk_options.ct_size[1] - 1);
    this->ui->labelSliderIdx->setText(QString("Slice: ") + QString::number(qRound(fdkoptions.ct_size[1] / 2.0)));
    this->ui->verticalSlider->setValue(qRound(fdkoptions.ct_size[1] / 2.0));
}

void Scui::SLT_SetButtonsAfterLoad(){
    ui->btnLoadData->setEnabled(false);
    ui->btnLoadData->setStyleSheet("QPushButton{color: rgba(255,255,255,60%);font-size: 18px;border-width: 1.4px; border-color: rgba(0,0,0,60%);border-style: solid; border-radius: 7px;}");
    ui->btnScatterCorrect->setEnabled(true);
    ui->btnScatterCorrect->setStyleSheet("QPushButton{background-color: #1367AB; color: #ffffff;font-size: 18px;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QPushButton:pressed{background-color: #E4A115}");
    //SLT_PreProcessCT(); // Is added by us. Added for later use
}

void Scui::SLT_UpdateSlider(int max){
    this->ui->verticalSlider->setMinimum(1);
    this->ui->verticalSlider->setMaximum(max-1);
}

void Scui::SLT_DisconnectSlider(){
    disconnect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));
}

void Scui::SLT_ReConnectSlider(int initVal){
    this->ui->verticalSlider->setValue(initVal);
    connect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));
    SLT_DrawReconImage();
    ui->btnScatterCorrect->setEnabled(true);
    ui->btnScatterCorrect->setStyleSheet("QPushButton{background-color: #1367AB; color: #ffffff;font-size: 18px;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}");
}
void Scui::SLT_UpdateLabelRaw(QString string){
        ui->labelRawImgTitle->setText(string);
}
void Scui::SLT_UpdateLabelCor(QString string){
    ui->labelCorImgTitle->setText(string);
}
void Scui::SLT_UpdateProgressBarLoad(int progress){
    ui->progressBarLoad->setValue(progress);
}
void Scui::SLT_UpdateProgressBarSC(int progress){
    ui->progressBarSC->setValue(progress);
}
void Scui::SLT_UpdateProgressBarWEPL(int progress){
    ui->progressBarWEPL->setValue(progress);
}
void Scui::SLT_SCThreadIsDone(){
    scatterCorrectingIsDone = true;
    ui->btnScatterCorrect->setEnabled(false);
    ui->btnScatterCorrect->setStyleSheet("QPushButton{color: rgba(255,255,255,60%);font-size: 18px;border-width: 1.4px; border-color: rgba(0,0,0,60%);border-style: solid; border-radius: 7px;}");
    ui->comboBoxWEPL->setEnabled(true);
    ui->comboBoxWEPL->setStyleSheet("QComboBox{font-weight: bold;font-size: 18px;background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));color: #ffffff;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QComboBox QAbstractItemView{selection-background-color: rgba(255,190,56,100%);}QComboBox::drop-down{border: 0px;}QComboBox::down-arrow {image: url(/Users/ct-10/Desktop/down.png);width: 14px;height: 14px;}");
    ui->comboBoxWEPL->setEnabled(true);
    ui->comboBoxWEPL->setCurrentIndex(0);
    SLT_WEPLcalc(ui->comboBoxWEPL->currentText());
    ui->progressBarWEPL->setValue(100);
    ui->comboBoxPlanView->setEnabled(true);
    ui->comboBoxPlanView->setStyleSheet("QComboBox{font-weight: bold;font-size: 18px;background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));color: #ffffff;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QComboBox QAbstractItemView{selection-background-color: rgba(255,190,56,100%);}QComboBox::drop-down{border: 0px;}QComboBox::down-arrow {image: url(/Users/ct-10/Desktop/down.png);width: 14px;height: 14px;}");
}
void Scui::SLT_LThreadIsDone(){
    ui->comboBox_region->setEnabled(true);
    ui->comboBox_region->setStyleSheet("QComboBox{font-weight: bold;font-size: 18px;background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));color: #ffffff;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QComboBox QAbstractItemView{selection-background-color: rgba(255,190,56,100%);}QComboBox::drop-down{border: 0px;}QComboBox::down-arrow {image: url(/Users/ct-10/Desktop/down.png);width: 14px;height: 14px;}");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------LOAD DATA FUNCTIONS---------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Is called by SLT_DoReconstruction()
void Scui::UpdateReconImage(UShortImageType::Pointer &spNewImg,
                                       QString &fileName) {

  m_cbctrecon->m_spCrntReconImg = spNewImg;


  const auto &p_curimg = this->m_cbctrecon->m_spCrntReconImg;
  const auto origin_new = p_curimg->GetOrigin();
  const auto spacing_new = p_curimg->GetSpacing();
  const auto size_new = p_curimg->GetBufferedRegion().GetSize();

  std::cout << "New Origin" << origin_new << std::endl;
  std::cout << "New spacing" << spacing_new << std::endl;
  std::cout << "New size" << size_new << std::endl;

  //this->ui.lineEdit_Cur3DFileName->setText(fileName);

  auto size = p_curimg->GetBufferedRegion().GetSize();

  this->m_cbctrecon->m_dspYKReconImage->CreateImage(size[0], size[1], 0); // maybe 100 instead of 0.

  disconnect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));
  this->ui->verticalSlider->setMinimum(1);
  this->ui->verticalSlider->setMaximum(size[2] - 1);

  const auto initVal = qRound((size[2] - 1) / 2.0);

  //SLT_InitializeGraphLim(); // What about this???


  this->ui->verticalSlider->setValue(initVal);
  //this->ui.radioButton_graph_recon->setChecked(true);

  connect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));

  SLT_DrawReconImage();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by set_points_by_slice()
template <enCOLOR color> auto get_qtpoint_vector(qyklabel *window) {
  switch (color) {
  case RED:
    return &window->m_vPt;
  case BLUE:
  default:
    return &window->m_vPt_green;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DrawImageWhenSliceChange()
template <typename ImageBase, enPLANE plane, enCOLOR color>
auto set_points_by_slice(qyklabel *window, Rtss_roi_modern *voi,
                         std::array<double, 3> curPhysPos,
                         typename ImageBase::SpacingType imgSpacing,
                         typename ImageBase::PointType imgOriginFixed,
                         typename ImageBase::SizeType imgSize) {
  auto *Wnd_contour = get_qtpoint_vector<color>(window);
  Wnd_contour->clear();

  auto wnd_size = window->size();
  auto wnd_height = wnd_size.rheight();
  const auto wnd_width = wnd_size.rwidth();
  auto x_scale = 1.0 / static_cast<double>(wnd_width);
  auto y_scale = 1.0 / static_cast<double>(wnd_height);
  switch (plane) {
  case PLANE_AXIAL:
    x_scale *= imgSpacing[0] * imgSize[0];
    y_scale *= imgSpacing[1] * imgSize[1];
    break;
  case PLANE_FRONTAL:
    x_scale *= imgSpacing[0] * imgSize[0];
    y_scale *= imgSpacing[2] * imgSize[2];
    break;
  case PLANE_SAGITTAL:
    x_scale *= imgSpacing[1] * imgSize[1];
    y_scale *= imgSpacing[2] * imgSize[2];
  }

  for (auto contour : voi->pslist) {
    if (contour.coordinates.empty()) {
      continue;
    }
    const auto first_point = contour.coordinates.at(0);
    // Axial
    if (first_point.z > curPhysPos[0] - imgSpacing[2] &&
        first_point.z < curPhysPos[0] + imgSpacing[2] && plane == PLANE_AXIAL) {
      for (auto point : contour.coordinates) {
        Wnd_contour->emplace_back((point.x - imgOriginFixed[0]) / x_scale,
                                  (point.y - imgOriginFixed[1]) / y_scale);
      }
    }
    for (auto &point : contour.coordinates) {
      // Frontal
      if (point.y > curPhysPos[1] - imgSpacing[1] &&
          point.y < curPhysPos[1] + imgSpacing[1] && plane == PLANE_FRONTAL) {
        Wnd_contour->emplace_back((point.x - imgOriginFixed[0]) / x_scale,
                                  wnd_height -
                                      (point.z - imgOriginFixed[2]) / y_scale);
      }
      // Sagittal
      if (point.x > curPhysPos[2] - imgSpacing[0] &&
          point.x < curPhysPos[2] + imgSpacing[0] && plane == PLANE_SAGITTAL) {
        Wnd_contour->emplace_back((point.y - imgOriginFixed[1]) / x_scale,
                                  wnd_height -
                                      (point.z - imgOriginFixed[2]) / y_scale);
      }
    }
  }

  window->m_bDrawPoints = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_DrawReconImage() {

  if (m_cbctrecon->m_dspYKReconImage == nullptr) {
    return;
  }

  if (m_cbctrecon->m_spCrntReconImg == nullptr) {
    std::cout << "no recon image to be displayed" << std::endl;
    return;
  }

  //  The ExtractImageFilter type is instantiated using the input and
  //  output image types. A filter object is created with the New()
  //  method and assigned to a SmartPointer.

  using ExtractFilterType =
      itk::ExtractImageFilter<UShortImageType, UShortImage2DType>;
  auto extractFilter = ExtractFilterType::New();


  using DuplicatorType = itk::ImageDuplicator<UShortImageType>;
  auto duplicator = DuplicatorType::New();

  duplicator->SetInputImage(m_cbctrecon->m_spCrntReconImg);


  duplicator->Update();
  const auto clonedImage = duplicator->GetOutput();

  extractFilter->SetDirectionCollapseToSubmatrix();

  auto crnt_region_3d = clonedImage->GetBufferedRegion();

  //  We take the size from the region and collapse the size in the $Z$
  //  component by setting its value to $1$.

  // Get Image Size and Extraction Index info.
  auto size = crnt_region_3d.GetSize();
  size[2] = 0; // z size number = 0 --> should not be 1

  auto start = crnt_region_3d.GetIndex();
  const auto iSliceNumber = this->ui->verticalSlider->value();//this->ui.spinBoxReconImgSliceNo->value();
  this->ui->labelSliderIdx->setText(QString("Slice: ") + QString::number(iSliceNumber));
  start[2] = iSliceNumber; // 60

  const auto originZ = m_cbctrecon->m_spCrntReconImg->GetOrigin()[2];
  const auto spacingZ = m_cbctrecon->m_spCrntReconImg->GetSpacing()[2];
  const auto posZ = originZ + iSliceNumber * spacingZ;

  const auto strPosZ = QString("%1").arg(posZ, 0, 'f', 2);
  // strPosZ.sprintf("%4.2f", posZ);
  //this->ui.lineEdit_CurrentPosZ->setText(strPosZ);

  // Define a region to generate
  UShortImageType::RegionType desiredRegion;
  desiredRegion.SetSize(size);   // 410 410 0
  desiredRegion.SetIndex(start); // 0 0 60

  // Error occurred here --> sloved by crntRegion3D =
  // m_spReconImg->GetBufferedRegion();
  extractFilter->SetExtractionRegion(desiredRegion); // error

  //  Below we connect the reader, filter and writer to form the data
  //  processing pipeline.
  extractFilter->SetInput(clonedImage);

  extractFilter->Update();

  UShortImage2DType::Pointer pCrnt2D = extractFilter->GetOutput();
  m_cbctrecon->m_dspYKReconImage = YK16GrayImage::CopyItkImage2YKImage(
      pCrnt2D,
      std::move(m_cbctrecon->m_dspYKReconImage)); // dimension should be
                                                        // same automatically.

  const auto physPosX = 0;//this->ui.lineEdit_PostFOV_X->text().toFloat();
  const auto physPosY = 0;//this->ui.lineEdit_PostFOV_Y->text().toFloat();
  const auto physRadius = 190;//this->ui.lineEdit_PostFOV_R->text().toFloat();
  const auto physTablePosY = 120;//this->ui.lineEdit_PostTablePosY->text().toFloat();
  m_cbctrecon->PostApplyFOVDispParam(physPosX, physPosY, physRadius,
                                           physTablePosY);

  auto p_dspykimg = m_cbctrecon->m_dspYKReconImage.get();
  if (false){//this->ui.checkBox_PostDispObjOn->isChecked()) {
    p_dspykimg->m_bDrawFOVCircle = true;
    p_dspykimg->m_bDrawTableLine = true;
  }

  else {
    p_dspykimg->m_bDrawFOVCircle = false;
    p_dspykimg->m_bDrawTableLine = false;
  }

  p_dspykimg->FillPixMapMinMax(0,2031);//this->ui.sliderReconImgMin->value(),
                               //this->ui.sliderReconImgMax->value());
/*
  if(scatterCorrectingIsDone){
      if(View ==0){
          this->ui->labelImageRaw->SetBaseImage(&m_YKDisp[0]);
      }else if(View ==1){
          this->ui->labelImageRaw->SetBaseImage(&m_YKDisp[1]);
      }
      else{
        return;
      }
  }else{
      this->ui->labelImageRaw->SetBaseImage(p_dspykimg);
  }
  */
  this->ui->labelImageRaw->SetBaseImage(p_dspykimg);
  this->ui->labelImageRaw->update();

}
void Scui::SLT_DrawReconInFixedSlice(){
    // Constitute m_YKDisp from Fixed and Moving
    // In Andreases code this checkbox was checked, so we replace this with true (checkBoxDrawSplit)
    if (true){//this->ui.checkBoxDrawSplit->isChecked()) {
      for (auto i = 0; i < 3; i++) {
        auto idxAdd = m_enViewArrange; // m_iViewArrange = 0,1,2
        if (idxAdd + i >= 3) {
          idxAdd = idxAdd - 3;
        }

        m_YKDisp[i].SetSpacing(m_YKImgFixed[i + idxAdd].m_fSpacingX,
                               m_YKImgFixed[i + idxAdd].m_fSpacingY);

        m_YKDisp[i].SetSplitOption(PRI_LEFT_TOP);
        if (!m_YKDisp[i].ConstituteFromTwo(m_YKImgFixed[i + idxAdd],
                                           m_YKImgMoving[i + idxAdd])) {
            std::cout << "Image error " << i + 1 << " th view" << std::endl;
        }
      }
    } else {
      for (auto i = 0; i < 3; i++) {
        auto addedViewIdx = m_enViewArrange;
        if (i + addedViewIdx >= 3) {
          addedViewIdx = addedViewIdx - 3;
        }

        m_YKDisp[i].CloneImage(m_YKImgFixed[i + addedViewIdx]);
      }
    }

    // For dose overlay
    if (m_cbctregistration->dose_loaded) {
        // In Andreases code this checkbox was checked, so we replace this with true (checkBoxDrawSplit)
      if (false){//this->ui.checkBoxDrawSplit->isChecked()) {
        for (auto i = 0; i < 3; i++) {
          auto idxAdd = m_enViewArrange; // m_iViewArrange = 0,1,2
          if (idxAdd + i >= 3) {
            idxAdd = idxAdd - 3;
          }

          m_AGDisp_Overlay[i].SetSpacing(m_DoseImgFixed[i + idxAdd].m_fSpacingX,
                                         m_DoseImgFixed[i + idxAdd].m_fSpacingY);

          m_AGDisp_Overlay[i].SetSplitOption(PRI_LEFT_TOP);
          if (!m_AGDisp_Overlay[i].ConstituteFromTwo(
                  m_DoseImgFixed[i + idxAdd], m_DoseImgMoving[i + idxAdd])) {
            std::cout << "Dose Image error " << i + 1 << " th view" << std::endl;
          }
        }
      } else {
        for (auto i = 0; i < 3; i++) {
          auto addedViewIdx = m_enViewArrange;
          if (i + addedViewIdx >= 3) {
            addedViewIdx = addedViewIdx - 3;
          }

          m_AGDisp_Overlay[i].CloneImage(m_DoseImgFixed[i + addedViewIdx]);
        }
      }
    }
    // In Andreases code this is set to 2000 (sliderFixedW)
    const auto sliderW1 = 2000;//this->ui.sliderFixedW->value();
    // In Andreases code this is set to 2000 (sliderMovingW)
    const auto sliderW2 = 2000;//this->ui.sliderMovingW->value();
    // In Andreases code this is set to 1024 (sliderFixedL)
    const auto sliderL1 = 1024;//this->ui.sliderFixedL->value();
    // In Andreases code this is set to 1024 (sliderMovingL)
    const auto sliderL2 = 1024;//this->ui.sliderMovingL->value();

    m_YKDisp[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_YKDisp[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_YKDisp[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);

    // In Andreases code he plots this on the registration ui, but we want to plot this on our ui
    if(View ==0){
        this->ui->labelImageRaw->SetBaseImage(&m_YKDisp[0]); //this->ui.labelOverlapWnd1->SetBaseImage(&m_YKDisp[0]);
    }else{
        this->ui->labelImageRaw->SetBaseImage(&m_YKDisp[1]); //this->ui.labelOverlapWnd2->SetBaseImage(&m_YKDisp[1]);
    }

    /*
    this->ui.labelOverlapWnd2->SetBaseImage(&m_YKDisp[1]);
    this->ui.labelOverlapWnd3->SetBaseImage(&m_YKDisp[2]);
    */

    // here gPMC results could be checked for and displayed, possibly with
    // modification to the qyklabel class /AGA 02/08/2017
    if (m_cbctregistration->dose_loaded) {
      m_AGDisp_Overlay[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
      m_AGDisp_Overlay[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
      m_AGDisp_Overlay[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
      // In Andreases code he plots this on the registration ui, but we want to plot this on our ui
      if(View ==0){
          this->ui->labelImageRaw->SetOverlayImage(&m_AGDisp_Overlay[0]); //this->ui.labelOverlapWnd1->SetOverlayImage(&m_AGDisp_Overlay[0]);
      }else{
          this->ui->labelImageRaw->SetOverlayImage(&m_AGDisp_Overlay[1]); //this->ui.labelOverlapWnd2->SetOverlayImage(&m_AGDisp_Overlay[1]);
      }
      /*
      this->ui.labelOverlapWnd2->SetOverlayImage(&m_AGDisp_Overlay[1]);
      this->ui.labelOverlapWnd3->SetOverlayImage(&m_AGDisp_Overlay[2]);
      */
    }
    // In Andreases code he plots this on the registration ui, but we only have one window
    this->ui->labelImageRaw->update();//this->ui.labelOverlapWnd1->update();
    /*
    this->ui.labelOverlapWnd2->update();
    this->ui.labelOverlapWnd3->update();
    */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//Is called by SLT_LoadSelectedProjFiles
void Scui::SLT_OpenElektaGeomFile() {
  auto strPath = QFileDialog::getOpenFileName(
      this, "Select a single file to open",
      this->m_cbctrecon->m_strPathDirDefault, "Geometry file (*.xml)");

  if (strPath.length() <= 1) {
    return;
  }
  //this->ui.lineEdit_ElektaGeomPath->setText(strPath);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_PreProcessCT() {
    /*
  if (!true){//this->ui.checkBoxCropBkgroundCT->isChecked()) {
    std::cout << "Preprocessing is not selected." << std::endl;
    return;
  }

  const auto iAirThresholdShort = 500;//this->ui.spinBoxBkDetectCT->value();

  if (false){//this->ui.comboBox_VOItoCropBy->count() < 1) {
    std::cout
        << "Reference CT DIR should be specified for structure based cropping"
        << std::endl;
    if (m_spMovingImg == nullptr || m_spFixedImg == nullptr) {
      return;
    }
    const auto fixed_size = m_spFixedImg->GetLargestPossibleRegion().GetSize();
    const auto moving_size = m_spMovingImg->GetLargestPossibleRegion().GetSize();
    if (fixed_size[0] != moving_size[0] || fixed_size[1] != moving_size[1] ||
        fixed_size[2] != moving_size[2]) {
      std::cout
          << "Fixed and moving image is not the same size, consider using "
             "a platimatch registration to solve this."
          << std::endl;
      return;
    }

    const auto reply =
        QMessageBox::question(this, "No reference structures found!",
                              "Do you wan't to attempt an auto correction "
                              "of air and excessive circumference?",
                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      std::cout << "Attempting automatic air filling and skin cropping..."
                << std::endl;
      m_cbctregistration->autoPreprocessCT(iAirThresholdShort, m_spFixedImg,
                                           m_spMovingImg);
    }
    return;
  }

  const auto strRSName = this->ui.comboBox_VOItoCropBy->currentText();
  const auto fill_bubble = this->ui.checkBoxFillBubbleCT->isChecked();
  const auto iBubbleFillingVal =
      this->ui.spinBoxBubFillCT->value(); // 1000 = soft tissue
  const auto iAirFillValShort = this->ui.spinBoxBkFillCT->value(); // 0 = air

  const auto cur_ct_text = ui.comboBoxImgMoving->currentText();
  const auto cur_ct = get_ctType(cur_ct_text);
  const auto &rt_structs =
      m_cbctregistration->m_pParent->m_structures->get_ss(cur_ct);

  QString image_str;
  if (ui.comboBoxImToCropFill->currentText().compare("Moving") == 0) {
    image_str = ui.comboBoxImgMoving->currentText();
  } else {
    image_str = ui.comboBoxImgFixed->currentText();
  }

  auto &image = m_cbctregistration->get_image_from_combotext(image_str);

  if (!m_cbctregistration->PreprocessCT(image, iAirThresholdShort, rt_structs,
                                        strRSName, fill_bubble,
                                        iBubbleFillingVal, iAirFillValShort)) {
    std::cout
        << "Error in PreprocessCT!!!scatter correction would not work out."
        << std::endl;
    m_cbctregistration->m_pParent->m_bMacroContinue = false;
  }

  show();

  UpdateListOfComboBox(0); // combo selection signalis called
  UpdateListOfComboBox(1);
  // if not found, just skip
  SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  SelectComboExternal(1, REGISTER_MANUAL_RIGID);
  SLT_DrawImageWhenSliceChange();

  std::cout << "FINISHED!: Pre-processing of CT image" << std::endl;

  ////Load DICOM plan
  if (m_cbctregistration->m_pParent->m_strPathPlan.isEmpty()) {
    std::cout << "No DCM plan file was found. Skipping dcm plan." << std::endl;
    return;
  }
  // QString dcmplanPath = m_pParent->m_strPathPlan;
  m_cbctregistration->LoadRTPlan(
      m_cbctregistration->m_pParent->m_strPathPlan); // fill RT_studyplan
      */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//--------------------------------------------------------SCATTER CORRECT FUNCTIONS---------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
ctType Scui::get_ctType(const QString &selText) {
  if (selText.compare("REF_CT") == 0) {
    return PLAN_CT;
  }
  if (selText.compare("MANUAL_RIGID_CT") == 0 ||
      selText.compare("AUTO_RIGID_CT") == 0) {
    return RIGID_CT;
  }
  if (selText.compare("DEFORMED_CT1") == 0 ||
      selText.compare("DEFORMED_CT2") == 0 ||
      selText.compare("DEFORMED_CT3") == 0 ||
      selText.compare("DEFORMED_CT_FINAL") == 0) {
    return DEFORM_CT;
  }
  return PLAN_CT;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SelectComboExternal
void Scui::SLT_FixedImageSelected(QString selText) {
  // QString strCrntText = this->ui.comboBoxImgFixed->currentText();
  LoadImgFromComboBox(0, selText); // here, m_spMovingImg and Fixed images are determined
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SelectComboExternal
void Scui::SLT_MovingImageSelected(QString selText) {
  // QString strCrntText = this->ui.comboBoxImgMoving->currentText();
  // std::cout << "SLT_MovingImageSelected" << std::endl;
  LoadImgFromComboBox(1, selText);
  const auto cur_ct = get_ctType(selText);
  UpdateVOICombobox(cur_ct);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_MovingImageSelected
void Scui::UpdateVOICombobox(const ctType ct_type) const {
  auto struct_set =
      m_cbctregistration->m_pParent->m_structures->get_ss(ct_type);
  if (struct_set == nullptr) {
    return;
  }
  if (struct_set->slist.empty()) {
    std::cerr << "Structures not initialized yet" << std::endl;
    return;
  }
  // In Andreases code he uses checkboxes therefore this is outcommented (comboBox_VOI)
  ui->comboBoxWEPL->clear();//this->ui.comboBox_VOI->clear();
  for (const auto &voi : struct_set->slist) {
    this->ui->comboBoxWEPL->addItem(QString(voi.name.c_str()));
    //this->ui.comboBox_VOItoCropBy->addItem(QString(voi.name.c_str()));
    //this->ui.comboBox_VOItoCropBy_copy->addItem(QString(voi.name.c_str()));
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_FixedImageSelected and SLT_MovingImageSelected
void Scui::LoadImgFromComboBox(const int idx, QString &strSelectedComboTxt) // -->when fixed image loaded will be called here!
{

  UShortImageType::Pointer spTmpImg;
  if (strSelectedComboTxt.compare(QString("RAW_CBCT"), Qt::CaseSensitive) ==
      0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spRawReconImg;
  } else if (strSelectedComboTxt.compare(QString("REF_CT"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spRefCTImg;
  } else if (strSelectedComboTxt.compare(QString("MANUAL_RIGID_CT"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spManualRigidCT;
  } else if (strSelectedComboTxt.compare(QString("AUTO_RIGID_CT"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spAutoRigidCT;
  } else if (strSelectedComboTxt.compare(QString("DEFORMED_CT1"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spDeformedCT1;
  } else if (strSelectedComboTxt.compare(QString("DEFORMED_CT2"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spDeformedCT2;
  } else if (strSelectedComboTxt.compare(QString("DEFORMED_CT3"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spDeformedCT3;
  } else if (strSelectedComboTxt.compare(QString("DEFORMED_CT_FINAL"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spDeformedCT_Final;
  } else if (strSelectedComboTxt.compare(QString("COR_CBCT"),
                                         Qt::CaseSensitive) == 0) {
    spTmpImg = m_cbctregistration->m_pParent->m_spScatCorrReconImg;
  }

  if (spTmpImg == nullptr) {
    return;
  }

  if (idx == 0) {
    m_spFixedImg = spTmpImg.GetPointer();

    whenFixedImgLoaded(); //commented out
  } else if (idx == 1) {
    m_spMovingImg = spTmpImg.GetPointer();
    // In Andreases code this does nothing so far and is therefore  outcommented (whenMovingImgLoaded())
    //whenMovingImgLoaded();
  }
  //if(scatterCorrectingIsDone == false){
      SLT_DrawImageWhenSliceChange();
  //}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by LoadImgFromComboBox()
void Scui::SLT_DrawImageWhenSliceChange() {
  if (m_spFixedImg == nullptr) {
    return;
  }
  // Added by us:
  if(m_cbctrecon->m_spRawReconImg == nullptr){
      return;
  }

  if(scatterCorrectingIsDone){
      m_spFixedImg = m_cbctregistration->m_pParent->m_spScatCorrReconImg.GetPointer();
      m_spMovingImg = m_cbctregistration->m_pParent->m_spScatCorrReconImg.GetPointer();
  }

  auto imgSize = m_spFixedImg->GetBufferedRegion().GetSize();
  const auto curPosZ = static_cast<int>(imgSize[2]/2);
  const auto curPosY = static_cast<int>(imgSize[1]/2);
  const auto curPosX = static_cast<int>(imgSize[0]/2);
  //

  int sliderPosIdxZ, sliderPosIdxY, sliderPosIdxX;
  switch (m_enViewArrange) {
  case AXIAL_FRONTAL_SAGITTAL:
    if(View ==0){
        sliderPosIdxZ = ui->verticalSlider->value();
        sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
        sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
    }else if (View ==1){
        sliderPosIdxZ = curPosZ;
        sliderPosIdxY = ui->verticalSlider->value();//this->ui.sliderPosDisp2->value();
        sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
    }else if(View ==2){
        sliderPosIdxZ = curPosZ;
        sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
        sliderPosIdxX = ui->verticalSlider->value();//this->ui.sliderPosDisp3->value();
    }
    break;
  case FRONTAL_SAGITTAL_AXIAL:
      if(View ==0){
          sliderPosIdxZ = ui->verticalSlider->value();
          sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
      }else if (View ==1){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = ui->verticalSlider->value();//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
      }else if(View ==2){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = ui->verticalSlider->value();//this->ui.sliderPosDisp3->value();
      }
    break;
  case SAGITTAL_AXIAL_FRONTAL:
      if(View ==0){
          sliderPosIdxZ = ui->verticalSlider->value();
          sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
      }else if (View ==1){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = ui->verticalSlider->value();//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
      }else if(View ==2){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = ui->verticalSlider->value();//this->ui.sliderPosDisp3->value();
      }
    break;
  default:
      if(View ==0){
          sliderPosIdxZ = ui->verticalSlider->value();
          sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
      }else if (View ==1){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = ui->verticalSlider->value();//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
      }else if(View ==2){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = ui->verticalSlider->value();//this->ui.sliderPosDisp3->value();
      }
    break;
  }

  //auto imgSize = m_spFixedImg->GetBufferedRegion().GetSize(); // 1016x1016 x z
  auto imgOrigin = m_spFixedImg->GetOrigin();
  auto imgSpacing = m_spFixedImg->GetSpacing();

  auto curPhysPos = std::array<double, 3>{{
      imgOrigin[2] + sliderPosIdxZ * imgSpacing[2], // Z in default setting
      imgOrigin[1] + sliderPosIdxY * imgSpacing[1], // Y
      imgOrigin[0] + sliderPosIdxX * imgSpacing[0]  // Z
  }};

  const auto refIdx = 3 - m_enViewArrange;
  // In Andreases code this checkbox was checked (checkBoxDrawCrosshair)
  if (false){//this->ui.checkBoxDrawCrosshair->isChecked()) {
    m_YKDisp[refIdx % 3].m_bDrawCrosshair = true;
    m_YKDisp[(refIdx + 1) % 3].m_bDrawCrosshair = true;
    m_YKDisp[(refIdx + 2) % 3].m_bDrawCrosshair = true;

    // m_YKDisp[0]// Left Top image, the largest

    m_YKDisp[refIdx % 3].m_ptCrosshair.setX(sliderPosIdxX); // axial
    m_YKDisp[refIdx % 3].m_ptCrosshair.setY(sliderPosIdxY);

    m_YKDisp[(refIdx + 1) % 3].m_ptCrosshair.setX(sliderPosIdxX); // Frontal
    m_YKDisp[(refIdx + 1) % 3].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                                  sliderPosIdxZ - 1);

    m_YKDisp[(refIdx + 2) % 3].m_ptCrosshair.setX(sliderPosIdxY); // Sagittal
    m_YKDisp[(refIdx + 2) % 3].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                                  sliderPosIdxZ - 1);

    m_YKImgFixed[0].m_bDrawCrosshair = true;
    m_YKImgFixed[1].m_bDrawCrosshair = true;
    m_YKImgFixed[2].m_bDrawCrosshair = true;

    m_YKImgFixed[0].m_ptCrosshair.setX(sliderPosIdxX); // sagittal slider
    m_YKImgFixed[0].m_ptCrosshair.setY(sliderPosIdxY);

    m_YKImgFixed[1].m_ptCrosshair.setX(sliderPosIdxX); // sagittal slider
    m_YKImgFixed[1].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                       sliderPosIdxZ - 1);

    m_YKImgFixed[2].m_ptCrosshair.setX(sliderPosIdxY); // sagittal slider
    m_YKImgFixed[2].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                       sliderPosIdxZ - 1);
  } else {
    m_YKDisp[0].m_bDrawCrosshair = false;
    m_YKDisp[1].m_bDrawCrosshair = false;
    m_YKDisp[2].m_bDrawCrosshair = false;

    m_YKImgFixed[0].m_bDrawCrosshair = false;
    m_YKImgFixed[1].m_bDrawCrosshair = false;
    m_YKImgFixed[2].m_bDrawCrosshair = false;
  }

  // center of the split value is passed by m_YKImgFixed;

  if (m_spMovingImg != nullptr) {
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spMovingImg, PLANE_AXIAL, curPhysPos[0], m_YKImgFixed[0],
        m_YKImgMoving[0]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spMovingImg, PLANE_FRONTAL, curPhysPos[1], m_YKImgFixed[1],
        m_YKImgMoving[1]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spMovingImg, PLANE_SAGITTAL, curPhysPos[2], m_YKImgFixed[2],
        m_YKImgMoving[2]);
    if (m_cbctregistration->dose_loaded) {
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spMovingDose, PLANE_AXIAL, curPhysPos[0],
          m_DoseImgFixed[0], m_DoseImgMoving[0]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spMovingDose, PLANE_FRONTAL, curPhysPos[1],
          m_DoseImgFixed[1], m_DoseImgMoving[1]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spMovingDose, PLANE_SAGITTAL, curPhysPos[2],
          m_DoseImgFixed[2], m_DoseImgMoving[2]);
    }
  } else {
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spFixedImg, PLANE_AXIAL, curPhysPos[0], m_YKImgFixed[0],
        m_YKImgMoving[0]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spFixedImg, PLANE_FRONTAL, curPhysPos[1], m_YKImgFixed[1],
        m_YKImgMoving[1]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixedImg, m_spFixedImg, PLANE_SAGITTAL, curPhysPos[2], m_YKImgFixed[2],
        m_YKImgMoving[2]);
    if (m_cbctregistration->dose_loaded) {
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spFixedDose, PLANE_AXIAL, curPhysPos[0],
          m_DoseImgFixed[0], m_DoseImgMoving[0]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spFixedDose, PLANE_FRONTAL, curPhysPos[1],
          m_DoseImgFixed[1], m_DoseImgMoving[1]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spFixedDose, m_spFixedDose, PLANE_SAGITTAL, curPhysPos[2],
          m_DoseImgFixed[2], m_DoseImgMoving[2]);
    }
  }

  // Update position lineEdit
  QString strPos1, strPos2, strPos3;
  strPos1.sprintf("%3.1f", curPhysPos[0]);
  strPos2.sprintf("%3.1f", curPhysPos[1]);
  strPos3.sprintf("%3.1f", curPhysPos[2]);
  // In Andreases code he sets these strings, but we don't. Therefore we outcomment them.
  /*
  this->ui.lineEditCurPosX->setText(strPos3);
  this->ui.lineEditCurPosY->setText(strPos2);
  this->ui.lineEditCurPosZ->setText(strPos1);
  */
  ////Update Origin text box
  auto imgOriginFixed = m_spFixedImg->GetOrigin();
  QString strOriFixed;
  strOriFixed.sprintf("%3.4f, %3.4f, %3.4f", imgOriginFixed[0], imgOriginFixed[1], imgOriginFixed[2]);
  // In Andreases code he set the text on the ui. Therefore we outcomment this (lineEditOriginFixed)
  //this->ui.lineEditOriginFixed->setText(strOriFixed);

  if (m_spMovingImg != nullptr) {
    const auto imgOriginMoving = m_spMovingImg->GetOrigin();
    QString strOriMoving;
    strOriMoving.sprintf("%3.4f, %3.4f, %3.4f", imgOriginMoving[0],
                         imgOriginMoving[1], imgOriginMoving[2]);
    // In Andreases code he sets these strings, but we don't. Therefore we outcomment them.
    //this->ui.lineEditOriginMoving->setText(strOriMoving);
  }
  // In Andreases code he have 3 labels on the registration ui, but we only have one. Therefor the below has been changed.
  /*
  auto arr_wnd = std::array<qyklabel *, 3>{{this->ui.labelOverlapWnd1,
                                            this->ui.labelOverlapWnd2,
                                            this->ui.labelOverlapWnd3}};
                                           */
  auto wnd = this->ui->labelImageCor;


  if (m_cbctregistration->cur_voi != nullptr) {
    const auto p_cur_voi = m_cbctregistration->cur_voi.get();

    if(View==0){
        set_points_by_slice<UShortImageType, PLANE_AXIAL, RED>(
            wnd, p_cur_voi, curPhysPos, imgSpacing,
            imgOriginFixed, imgSize);
    }else if (View ==1){
        set_points_by_slice<UShortImageType, PLANE_FRONTAL, RED>(
            wnd, p_cur_voi, curPhysPos, imgSpacing,
            imgOriginFixed, imgSize);
    }else if (View==2){
        set_points_by_slice<UShortImageType, PLANE_SAGITTAL, RED>(
            wnd, p_cur_voi, curPhysPos, imgSpacing,
            imgOriginFixed, imgSize);
    }

  }

  if (m_cbctregistration->WEPL_voi != nullptr) {
    const auto p_wepl_voi = m_cbctregistration->WEPL_voi.get();

    if(View==0){
        set_points_by_slice<UShortImageType, PLANE_AXIAL, BLUE>(
            wnd, p_wepl_voi, curPhysPos, imgSpacing,
            imgOriginFixed, imgSize);
    }else if(View ==1){
        set_points_by_slice<UShortImageType, PLANE_FRONTAL, BLUE>(
            wnd, p_wepl_voi, curPhysPos, imgSpacing,
            imgOriginFixed, imgSize);
    }else if(View==2){
        set_points_by_slice<UShortImageType, PLANE_SAGITTAL, BLUE>(
            wnd, p_wepl_voi, curPhysPos, imgSpacing,
            imgOriginFixed, imgSize);
    }
  }

  SLT_DrawImageInFixedSlice();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DrawImageWhenSliceChange()
void Scui::SLT_DrawImageInFixedSlice() const
// Display Swap here!
{
  // Constitute m_YKDisp from Fixed and Moving
  // In Andreases code this checkbox was checked, so we replace this with true (checkBoxDrawSplit)
  if (true){//this->ui.checkBoxDrawSplit->isChecked()) {
    for (auto i = 0; i < 3; i++) {
      auto idxAdd = m_enViewArrange; // m_iViewArrange = 0,1,2
      if (idxAdd + i >= 3) {
        idxAdd = idxAdd - 3;
      }

      m_YKDisp[i].SetSpacing(m_YKImgFixed[i + idxAdd].m_fSpacingX,
                             m_YKImgFixed[i + idxAdd].m_fSpacingY);

      m_YKDisp[i].SetSplitOption(PRI_LEFT_TOP);
      if (!m_YKDisp[i].ConstituteFromTwo(m_YKImgFixed[i + idxAdd],
                                         m_YKImgMoving[i + idxAdd])) {
          std::cout << "Image error " << i + 1 << " th view" << std::endl;
      }
    }
  } else {
    for (auto i = 0; i < 3; i++) {
      auto addedViewIdx = m_enViewArrange;
      if (i + addedViewIdx >= 3) {
        addedViewIdx = addedViewIdx - 3;
      }

      m_YKDisp[i].CloneImage(m_YKImgFixed[i + addedViewIdx]);
    }
  }

  // For dose overlay
  if (m_cbctregistration->dose_loaded) {
      // In Andreases code this checkbox was checked, so we replace this with true (checkBoxDrawSplit)
    if (false){//this->ui.checkBoxDrawSplit->isChecked()) {
      for (auto i = 0; i < 3; i++) {
        auto idxAdd = m_enViewArrange; // m_iViewArrange = 0,1,2
        if (idxAdd + i >= 3) {
          idxAdd = idxAdd - 3;
        }

        m_AGDisp_Overlay[i].SetSpacing(m_DoseImgFixed[i + idxAdd].m_fSpacingX,
                                       m_DoseImgFixed[i + idxAdd].m_fSpacingY);

        m_AGDisp_Overlay[i].SetSplitOption(PRI_LEFT_TOP);
        if (!m_AGDisp_Overlay[i].ConstituteFromTwo(
                m_DoseImgFixed[i + idxAdd], m_DoseImgMoving[i + idxAdd])) {
          std::cout << "Dose Image error " << i + 1 << " th view" << std::endl;
        }
      }
    } else {
      for (auto i = 0; i < 3; i++) {
        auto addedViewIdx = m_enViewArrange;
        if (i + addedViewIdx >= 3) {
          addedViewIdx = addedViewIdx - 3;
        }

        m_AGDisp_Overlay[i].CloneImage(m_DoseImgFixed[i + addedViewIdx]);
      }
    }
  }
  // In Andreases code this is set to 2000 (sliderFixedW)
  const auto sliderW1 = 2000;//this->ui.sliderFixedW->value();
  // In Andreases code this is set to 2000 (sliderMovingW)
  const auto sliderW2 = 2000;//this->ui.sliderMovingW->value();
  // In Andreases code this is set to 1024 (sliderFixedL)
  const auto sliderL1 = 1024;//this->ui.sliderFixedL->value();
  // In Andreases code this is set to 1024 (sliderMovingL)
  const auto sliderL2 = 1024;//this->ui.sliderMovingL->value();

  m_YKDisp[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
  m_YKDisp[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
  m_YKDisp[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);

  // In Andreases code he plots this on the registration ui, but we want to plot this on our ui

  if(View ==0){
      this->ui->labelImageCor->SetBaseImage(&m_YKDisp[0]);
  }else if(View==1){
      this->ui->labelImageCor->SetBaseImage(&m_YKDisp[1]);
  }else if(View==2){
      this->ui->labelImageCor->SetBaseImage(&m_YKDisp[2]);
  }

  /*
  this->ui.labelOverlapWnd2->SetBaseImage(&m_YKDisp[1]);
  this->ui.labelOverlapWnd3->SetBaseImage(&m_YKDisp[2]);
  */

  // here gPMC results could be checked for and displayed, possibly with
  // modification to the qyklabel class /AGA 02/08/2017
  if (m_cbctregistration->dose_loaded) {
    m_AGDisp_Overlay[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_AGDisp_Overlay[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_AGDisp_Overlay[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    // In Andreases code he plots this on the registration ui, but we want to plot this on our ui

    if(View==0){
        this->ui->labelImageCor->SetOverlayImage(&m_AGDisp_Overlay[0]);
    }else if(View==1){
        this->ui->labelImageCor->SetOverlayImage(&m_AGDisp_Overlay[1]);
    }else if(View==2){
        this->ui->labelImageCor->SetOverlayImage(&m_AGDisp_Overlay[2]);
    }

    /*
    this->ui.labelOverlapWnd2->SetOverlayImage(&m_AGDisp_Overlay[1]);
    this->ui.labelOverlapWnd3->SetOverlayImage(&m_AGDisp_Overlay[2]);
    */
  }
  // In Andreases code he plots this on the registration ui, but we only have one window
  this->ui->labelImageCor->update();
  /*
  this->ui.labelOverlapWnd2->update();
  this->ui.labelOverlapWnd3->update();
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by LoadImgFromComboBox()

// Display is not included here
void Scui::whenFixedImgLoaded() const {
  if (m_cbctrecon->m_spRawReconImg == nullptr) {
    return;
  }

  auto imgSize = m_cbctrecon->m_spRawReconImg->GetBufferedRegion().GetSize(); // 1016x1016 x z

  // In Andreases code this does nothing so far and is therefore  outcommented (initOverlapWndSize())
  //initOverlapWndSize();

  // In Andreases code he uses slider but we don't. Therefore this is outcommented
  /*
  this->ui.sliderPosDisp1->setMinimum(0);
  this->ui.sliderPosDisp1->setMaximum(static_cast<int>(imgSize[2] - 1));
  */
  const auto curPosZ = static_cast<int>(imgSize[2] / 2);
  /*
  this->ui.sliderPosDisp1->setValue(curPosZ);


  this->ui.sliderPosDisp2->setMinimum(0);
  this->ui.sliderPosDisp2->setMaximum(static_cast<int>(imgSize[1] - 1));
  */
  const auto curPosY = static_cast<int>(imgSize[1] / 2);
  /*
  this->ui.sliderPosDisp2->setValue(curPosY);

  this->ui.sliderPosDisp3->setMinimum(0);
  this->ui.sliderPosDisp3->setMaximum(static_cast<int>(imgSize[0] - 1));
  */
  const auto curPosX = static_cast<int>(imgSize[0] / 2);
  /*
  this->ui.sliderPosDisp3->setValue(curPosX);
  */
  std::cout << "sliderPosDisp1:" << curPosX << " th view" << std::endl;
  std::cout << "sliderPosDisp2:" << curPosY << " th view" << std::endl;
  std::cout << "sliderPosDisp2:" << curPosZ << " th view" << std::endl;

  auto x_split = QPoint(static_cast<int>(0), static_cast<int>(0));//QPoint(static_cast<int>(imgSize[0] / 2), static_cast<int>(imgSize[1] / 2));
  auto y_split = QPoint(static_cast<int>(0), static_cast<int>(0));//QPoint(static_cast<int>(imgSize[0] / 2), static_cast<int>(imgSize[2] / 2));
  auto z_split = QPoint(static_cast<int>(0), static_cast<int>(0));//QPoint(static_cast<int>(imgSize[1] / 2),static_cast<int>(imgSize[2] / 2));

  m_YKDisp[0].SetSplitCenter(x_split);
  m_YKDisp[1].SetSplitCenter(y_split);
  m_YKDisp[2].SetSplitCenter(z_split);

  const auto iSliderW = 2000;
  const auto iSliderL = 1024;
  // In Andreases code he uses slider but we don't. Therefore this is outcommented
  /*
  this->ui.sliderFixedW->setValue(iSliderW);
  this->ui.sliderMovingW->setValue(iSliderW);

  this->ui.sliderFixedL->setValue(iSliderL);
  this->ui.sliderMovingL->setValue(iSliderL);
  */
  SLT_DrawImageInFixedSlice();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::ImageManualMoveOneShot(
    const float shiftX, const float shiftY,
    const float shiftZ) // DICOM coordinate
{
  if (m_spMovingImg == nullptr) {
    return;
  }

  // USHORT_ImageType::SizeType imgSize =
  // m_spMovingImg->GetRequestedRegion().GetSize(); //1016x1016 x z
  using USPointType = UShortImageType::PointType;
  auto imgOrigin = m_spMovingImg->GetOrigin();
  // USHORT_ImageType::SpacingType imgSpacing = m_spFixedImg->GetSpacing();
  imgOrigin[0] = imgOrigin[0] - static_cast<USPointType::ValueType>(shiftX);
  imgOrigin[1] = imgOrigin[1] - static_cast<USPointType::ValueType>(shiftY);
  imgOrigin[2] = imgOrigin[2] - static_cast<USPointType::ValueType>(shiftZ);

  m_spMovingImg->SetOrigin(imgOrigin);

  SLT_DrawImageWhenSliceChange();

  // Display relative movement
  // Starting point? RefCT image
  // Only Valid when Moving image is the ManualMove
  auto imgOriginRef = this->m_cbctrecon->m_spRefCTImg->GetOrigin(); //m_pParent replaced with this

  printf("delta(mm): %3.1f, %3.1f, %3.1f", imgOrigin[0] - imgOriginRef[0],
      imgOrigin[1] - imgOriginRef[1], imgOrigin[2] - imgOriginRef[2]);
  //this->ui.lineEditOriginChanged->setText(strDelta);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_PassFixedImgForAnalysis(QString cur_fixed) {
  if (m_cbctrecon->m_spRawReconImg != nullptr) {
    this->UpdateReconImage(m_cbctrecon->m_spRawReconImg, cur_fixed);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_WEPLcalc(QString structure) {
  //Get VIO
  const auto voi_name = structure.toStdString();

  const auto gantry_angle = 0;//this->ui.spinBox_GantryAngle->value();
  const auto couch_angle = 0;//this->ui.spinBox_CouchAngle->value();

  const auto ct_type = get_ctType("COR_CBCT");//ui.comboBoxImgMoving->currentText());
  const auto ss = m_cbctrecon->m_structures->get_ss(ct_type);
  m_cbctregistration->cur_voi = ss->get_roi_by_name(voi_name);

  const auto wepl_voi =
      CalculateWEPLtoVOI(m_cbctregistration->cur_voi.get(), gantry_angle,
                         couch_angle, m_spMovingImg, m_spFixedImg);
  m_cbctregistration->WEPL_voi = std::make_unique<Rtss_roi_modern>(*wepl_voi);
  // Draw WEPL
  SLT_DrawImageWhenSliceChange();
}

void Scui::on_comboBoxWEPL_currentIndexChanged(const QString &structure)
{ 
    if(scatterCorrectingIsDone){
        Structure = structure;
        SLT_StartWEPLThread();
    }

}

void Scui::SLT_GetCBCTPath(){
    CBCTPath = QFileDialog::getExistingDirectory(
        this, tr("Open Directory with CBCT"), this->m_cbctrecon->m_strPathDirDefault,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}
void Scui::SLT_GetCTPath(){
    CTPath = QFileDialog::getExistingDirectory(
        this, tr("Open CT DICOM Directory"), this->m_cbctrecon->m_strPathDirDefault,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}


void Scui::on_comboBoxPlanView_currentIndexChanged(const QString &planView)
{
    if(scatterCorrectingIsDone){

    if(planView.compare("Axial") == 0){
        View = 0;       
      }
    else if (planView.compare("Frontal") == 0){
        View = 1;
    }
    else if (planView.compare("Sagital") == 0){
        View = 2;
    }
    SLT_SliderValueChanged();
    }
}

void Scui::SLT_DrawReconImageInSlices(){
    // init fixed and moving images
    //m_spFixedImg = m_cbctregistration->m_pParent->m_spRawReconImg.GetPointer();
    //m_spMovingImg = m_cbctregistration->m_pParent->m_spRawReconImg.GetPointer() ;
    m_spRawFixedImg = m_cbctregistration->m_pParent->m_spRawReconImg.GetPointer();
    m_spRawMovingImg = m_cbctregistration->m_pParent->m_spRawReconImg.GetPointer();

    if(m_spRawFixedImg == nullptr){
        return;
    }
    auto imgSize = m_spRawFixedImg->GetBufferedRegion().GetSize();
    const auto curPosZ = static_cast<int>(imgSize[2]/2);
    const auto curPosY = static_cast<int>(imgSize[1]/2);
    const auto curPosX = static_cast<int>(imgSize[0]/2);
    //

    int sliderPosIdxZ, sliderPosIdxY, sliderPosIdxX;
    switch (m_enViewArrangeRaw) {
    case AXIAL_FRONTAL_SAGITTAL:
      if(View ==0){
          sliderPosIdxZ = ui->verticalSlider->value();
          sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
      }else if(View ==1){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = ui->verticalSlider->value();//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
      } else if(View==2){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
          sliderPosIdxX = ui->verticalSlider->value();//this->ui.sliderPosDisp3->value();
      }
      break;
    case FRONTAL_SAGITTAL_AXIAL:
        if(View ==0){
            sliderPosIdxZ = ui->verticalSlider->value();
            sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
        }else if(View ==1){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = ui->verticalSlider->value();//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
        } else if(View==2){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = ui->verticalSlider->value();//this->ui.sliderPosDisp3->value();
        }
      break;
    case SAGITTAL_AXIAL_FRONTAL:
        if(View ==0){
            sliderPosIdxZ = ui->verticalSlider->value();
            sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
        }else if(View ==1){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = ui->verticalSlider->value();//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
        } else if(View==2){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = ui->verticalSlider->value();//this->ui.sliderPosDisp3->value();
        }
      break;
    default:
        if(View ==0){
            sliderPosIdxZ = ui->verticalSlider->value();
            sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
        }else if(View ==1){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = ui->verticalSlider->value();//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = curPosX;//this->ui.sliderPosDisp3->value();
        } else if(View==2){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = curPosY;//this->ui.sliderPosDisp2->value();
            sliderPosIdxX = ui->verticalSlider->value();//this->ui.sliderPosDisp3->value();
        }
      break;
    }

    //auto imgSize = m_spFixedImg->GetBufferedRegion().GetSize(); // 1016x1016 x z
    auto imgOrigin = m_spRawFixedImg->GetOrigin();
    auto imgSpacing = m_spRawFixedImg->GetSpacing();

    auto curPhysPos = std::array<double, 3>{{
        imgOrigin[2] + sliderPosIdxZ * imgSpacing[2], // Z in default setting
        imgOrigin[1] + sliderPosIdxY * imgSpacing[1], // Y
        imgOrigin[0] + sliderPosIdxX * imgSpacing[0]  // Z
    }};

    const auto refIdx = 3 - m_enViewArrangeRaw;
    // In Andreases code this checkbox was checked (checkBoxDrawCrosshair)
    if (false){//this->ui.checkBoxDrawCrosshair->isChecked()) {
      m_YKDispRaw[refIdx % 3].m_bDrawCrosshair = true;
      m_YKDispRaw[(refIdx + 1) % 3].m_bDrawCrosshair = true;
      m_YKDispRaw[(refIdx + 2) % 3].m_bDrawCrosshair = true;

      // m_YKDisp[0]// Left Top image, the largest

      m_YKDispRaw[refIdx % 3].m_ptCrosshair.setX(sliderPosIdxX); // axial
      m_YKDispRaw[refIdx % 3].m_ptCrosshair.setY(sliderPosIdxY);

      m_YKDispRaw[(refIdx + 1) % 3].m_ptCrosshair.setX(sliderPosIdxX); // Frontal
      m_YKDispRaw[(refIdx + 1) % 3].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                                    sliderPosIdxZ - 1);

      m_YKDispRaw[(refIdx + 2) % 3].m_ptCrosshair.setX(sliderPosIdxY); // Sagittal
      m_YKDispRaw[(refIdx + 2) % 3].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                                    sliderPosIdxZ - 1);

      m_YKImgRawFixed[0].m_bDrawCrosshair = true;
      m_YKImgRawFixed[1].m_bDrawCrosshair = true;
      m_YKImgRawFixed[2].m_bDrawCrosshair = true;

      m_YKImgRawFixed[0].m_ptCrosshair.setX(sliderPosIdxX); // sagittal slider
      m_YKImgRawFixed[0].m_ptCrosshair.setY(sliderPosIdxY);

      m_YKImgRawFixed[1].m_ptCrosshair.setX(sliderPosIdxX); // sagittal slider
      m_YKImgRawFixed[1].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                         sliderPosIdxZ - 1);

      m_YKImgRawFixed[2].m_ptCrosshair.setX(sliderPosIdxY); // sagittal slider
      m_YKImgRawFixed[2].m_ptCrosshair.setY(static_cast<int>(imgSize[2]) -
                                         sliderPosIdxZ - 1);
    } else {
      m_YKDispRaw[0].m_bDrawCrosshair = false;
      m_YKDispRaw[1].m_bDrawCrosshair = false;
      m_YKDispRaw[2].m_bDrawCrosshair = false;

      m_YKImgRawFixed[0].m_bDrawCrosshair = false;
      m_YKImgRawFixed[1].m_bDrawCrosshair = false;
      m_YKImgRawFixed[2].m_bDrawCrosshair = false;
    }

    // center of the split value is passed by m_YKImgFixed;

    if (m_spRawFixedImg != nullptr) {
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spRawFixedImg, m_spRawMovingImg, PLANE_AXIAL, curPhysPos[0], m_YKImgRawFixed[0],
          m_YKImgRawMoving[0]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spRawFixedImg, m_spRawMovingImg, PLANE_FRONTAL, curPhysPos[1], m_YKImgRawFixed[1],
          m_YKImgRawMoving[1]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spRawFixedImg, m_spRawMovingImg, PLANE_SAGITTAL, curPhysPos[2], m_YKImgRawFixed[2],
          m_YKImgRawMoving[2]);
      if (false){//m_cbctregistration->dose_loaded) {
        this->m_cbctrecon->Draw2DFrom3DDouble(
            m_spFixedDose, m_spMovingDose, PLANE_AXIAL, curPhysPos[0],
            m_DoseImgFixed[0], m_DoseImgMoving[0]);
        this->m_cbctrecon->Draw2DFrom3DDouble(
            m_spFixedDose, m_spMovingDose, PLANE_FRONTAL, curPhysPos[1],
            m_DoseImgFixed[1], m_DoseImgMoving[1]);
        this->m_cbctrecon->Draw2DFrom3DDouble(
            m_spFixedDose, m_spMovingDose, PLANE_SAGITTAL, curPhysPos[2],
            m_DoseImgFixed[2], m_DoseImgMoving[2]);
      }
    } else {
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spRawFixedImg, m_spRawFixedImg, PLANE_AXIAL, curPhysPos[0], m_YKImgRawFixed[0],
          m_YKImgRawMoving[0]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spRawFixedImg, m_spRawFixedImg, PLANE_FRONTAL, curPhysPos[1], m_YKImgRawFixed[1],
          m_YKImgRawMoving[1]);
      this->m_cbctrecon->Draw2DFrom3DDouble(
          m_spRawFixedImg, m_spRawFixedImg, PLANE_SAGITTAL, curPhysPos[2], m_YKImgRawFixed[2],
          m_YKImgRawMoving[2]);
      if (false){//m_cbctregistration->dose_loaded) {
        this->m_cbctrecon->Draw2DFrom3DDouble(
            m_spFixedDose, m_spFixedDose, PLANE_AXIAL, curPhysPos[0],
            m_DoseImgFixed[0], m_DoseImgMoving[0]);
        this->m_cbctrecon->Draw2DFrom3DDouble(
            m_spFixedDose, m_spFixedDose, PLANE_FRONTAL, curPhysPos[1],
            m_DoseImgFixed[1], m_DoseImgMoving[1]);
        this->m_cbctrecon->Draw2DFrom3DDouble(
            m_spFixedDose, m_spFixedDose, PLANE_SAGITTAL, curPhysPos[2],
            m_DoseImgFixed[2], m_DoseImgMoving[2]);
      }
    }

    // Update position lineEdit
    QString strPos1, strPos2, strPos3;
    strPos1.sprintf("%3.1f", curPhysPos[0]);
    strPos2.sprintf("%3.1f", curPhysPos[1]);
    strPos3.sprintf("%3.1f", curPhysPos[2]);
    // In Andreases code he sets these strings, but we don't. Therefore we outcomment them.
    /*
    this->ui.lineEditCurPosX->setText(strPos3);
    this->ui.lineEditCurPosY->setText(strPos2);
    this->ui.lineEditCurPosZ->setText(strPos1);
    */
    ////Update Origin text box
    auto imgOriginFixed = m_spRawFixedImg->GetOrigin();
    QString strOriFixed;
    strOriFixed.sprintf("%3.4f, %3.4f, %3.4f", imgOriginFixed[0], imgOriginFixed[1], imgOriginFixed[2]);
    // In Andreases code he set the text on the ui. Therefore we outcomment this (lineEditOriginFixed)
    //this->ui.lineEditOriginFixed->setText(strOriFixed);

    if (m_spRawMovingImg != nullptr) {
      const auto imgOriginMoving = m_spRawMovingImg->GetOrigin();
      QString strOriMoving;
      strOriMoving.sprintf("%3.4f, %3.4f, %3.4f", imgOriginMoving[0],
                           imgOriginMoving[1], imgOriginMoving[2]);
      // In Andreases code he sets these strings, but we don't. Therefore we outcomment them.
      //this->ui.lineEditOriginMoving->setText(strOriMoving);
    }
    // In Andreases code he have 3 labels on the registration ui, but we only have one. Therefor the below has been changed.
    /*
    auto arr_wnd = std::array<qyklabel *, 3>{{this->ui.labelOverlapWnd1,
                                              this->ui.labelOverlapWnd2,
                                              this->ui.labelOverlapWnd3}};
                                             */
    auto wnd = this->ui->labelImageRaw;


    if (m_cbctregistration->cur_voi != nullptr) {
      const auto p_cur_voi = m_cbctregistration->cur_voi.get();

      if(View==0){
          set_points_by_slice<UShortImageType, PLANE_AXIAL, RED>(
              wnd, p_cur_voi, curPhysPos, imgSpacing,
              imgOriginFixed, imgSize);
      }else if(View==1){
          set_points_by_slice<UShortImageType, PLANE_FRONTAL, RED>(
              wnd, p_cur_voi, curPhysPos, imgSpacing,
              imgOriginFixed, imgSize);
      }else if(View==2){
          set_points_by_slice<UShortImageType, PLANE_SAGITTAL, RED>(
              wnd, p_cur_voi, curPhysPos, imgSpacing,
              imgOriginFixed, imgSize);
      }

    }

    if (m_cbctregistration->WEPL_voi != nullptr) {
      const auto p_wepl_voi = m_cbctregistration->WEPL_voi.get();

      if(View==0){
          set_points_by_slice<UShortImageType, PLANE_AXIAL, BLUE>(
              wnd, p_wepl_voi, curPhysPos, imgSpacing,
              imgOriginFixed, imgSize);
      }else if(View==1){
          set_points_by_slice<UShortImageType, PLANE_FRONTAL, BLUE>(
              wnd, p_wepl_voi, curPhysPos, imgSpacing,
              imgOriginFixed, imgSize);
      }else if(View==2){
          set_points_by_slice<UShortImageType, PLANE_SAGITTAL, BLUE>(
              wnd, p_wepl_voi, curPhysPos, imgSpacing,
              imgOriginFixed, imgSize);
      }
    }

    // Constitute m_YKDisp from Fixed and Moving
    // In Andreases code this checkbox was checked, so we replace this with true (checkBoxDrawSplit)
    if (true){//this->ui.checkBoxDrawSplit->isChecked()) {
      for (auto i = 0; i < 3; i++) {
        auto idxAdd = m_enViewArrangeRaw; // m_iViewArrange = 0,1,2
        if (idxAdd + i >= 3) {
          idxAdd = idxAdd - 3;
        }

        m_YKDispRaw[i].SetSpacing(m_YKImgRawFixed[i + idxAdd].m_fSpacingX,
                               m_YKImgRawFixed[i + idxAdd].m_fSpacingY);

        m_YKDispRaw[i].SetSplitOption(PRI_LEFT_TOP);
        if (!m_YKDispRaw[i].ConstituteFromTwo(m_YKImgRawFixed[i + idxAdd],
                                           m_YKImgRawMoving[i + idxAdd])) {
            std::cout << "Image error " << i + 1 << " th view" << std::endl;
        }
      }
    } else {
      for (auto i = 0; i < 3; i++) {
        auto addedViewIdx = m_enViewArrangeRaw;
        if (i + addedViewIdx >= 3) {
          addedViewIdx = addedViewIdx - 3;
        }

        m_YKDispRaw[i].CloneImage(m_YKImgRawFixed[i + addedViewIdx]);
      }
    }

    // For dose overlay
    if (false){//m_cbctregistration->dose_loaded) {
        // In Andreases code this checkbox was checked, so we replace this with true (checkBoxDrawSplit)
      if (false){//this->ui.checkBoxDrawSplit->isChecked()) {
        for (auto i = 0; i < 3; i++) {
          auto idxAdd = m_enViewArrangeRaw; // m_iViewArrange = 0,1,2
          if (idxAdd + i >= 3) {
            idxAdd = idxAdd - 3;
          }

          m_AGDisp_Overlay[i].SetSpacing(m_DoseImgFixed[i + idxAdd].m_fSpacingX,
                                         m_DoseImgFixed[i + idxAdd].m_fSpacingY);

          m_AGDisp_Overlay[i].SetSplitOption(PRI_LEFT_TOP);
          if (!m_AGDisp_Overlay[i].ConstituteFromTwo(
                  m_DoseImgFixed[i + idxAdd], m_DoseImgMoving[i + idxAdd])) {
            std::cout << "Dose Image error " << i + 1 << " th view" << std::endl;
          }
        }
      } else {
        for (auto i = 0; i < 3; i++) {
          auto addedViewIdx = m_enViewArrangeRaw;
          if (i + addedViewIdx >= 3) {
            addedViewIdx = addedViewIdx - 3;
          }

          m_AGDisp_Overlay[i].CloneImage(m_DoseImgFixed[i + addedViewIdx]);
        }
      }
    }
    // In Andreases code this is set to 2000 (sliderFixedW)
    const auto sliderW1 = 2000;//this->ui.sliderFixedW->value();
    // In Andreases code this is set to 2000 (sliderMovingW)
    const auto sliderW2 = 2000;//this->ui.sliderMovingW->value();
    // In Andreases code this is set to 1024 (sliderFixedL)
    const auto sliderL1 = 1024;//this->ui.sliderFixedL->value();
    // In Andreases code this is set to 1024 (sliderMovingL)
    const auto sliderL2 = 1024;//this->ui.sliderMovingL->value();

    m_YKDispRaw[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_YKDispRaw[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_YKDispRaw[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);

    // In Andreases code he plots this on the registration ui, but we want to plot this on our ui

    if(View ==0){
        this->ui->labelImageRaw->SetBaseImage(&m_YKDispRaw[0]);
    }else if(View==1){
        this->ui->labelImageRaw->SetBaseImage(&m_YKDispRaw[1]);
    }else if(View ==2){
        this->ui->labelImageRaw->SetBaseImage(&m_YKDispRaw[2]);
    }

    /*
    this->ui.labelOverlapWnd2->SetBaseImage(&m_YKDisp[1]);
    this->ui.labelOverlapWnd3->SetBaseImage(&m_YKDisp[2]);
    */

    // here gPMC results could be checked for and displayed, possibly with
    // modification to the qyklabel class /AGA 02/08/2017
    if (false){//m_cbctregistration->dose_loaded) {
      m_AGDisp_Overlay[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
      m_AGDisp_Overlay[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
      m_AGDisp_Overlay[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
      // In Andreases code he plots this on the registration ui, but we want to plot this on our ui

      if(View==0){
          this->ui->labelImageRaw->SetOverlayImage(&m_AGDisp_Overlay[0]);
      }else if(View==1){
          this->ui->labelImageRaw->SetOverlayImage(&m_AGDisp_Overlay[1]);
      }else if(View ==2){
          this->ui->labelImageRaw->SetOverlayImage(&m_AGDisp_Overlay[2]);
      }

      /*
      this->ui.labelOverlapWnd2->SetOverlayImage(&m_AGDisp_Overlay[1]);
      this->ui.labelOverlapWnd3->SetOverlayImage(&m_AGDisp_Overlay[2]);
      */
    }
    // In Andreases code he plots this on the registration ui, but we only have one window
    this->ui->labelImageRaw->update();
    /*
    this->ui.labelOverlapWnd2->update();
    this->ui.labelOverlapWnd3->update();
    */
}

FDK_options Scui::getFDKoptions() const {
  FDK_options fdk_options;
  // In Andreases code this was initialized as 1.0
  fdk_options.TruncCorFactor = 1.0;//this->ui.lineEdit_Ramp_TruncationCorrection->text().toDouble();
  // In Andreases code this was initialized as 0.5
  fdk_options.HannCutX = 0.5;//this->ui.lineEdit_Ramp_HannCut->text().toDouble();
  // In Andreases code this was initialized as 0.5
  fdk_options.HannCutY = 0.5;//this->ui.lineEdit_Ramp_HannCutY->text().toDouble();
  // In Andreases code this was initialized as 0.0
  fdk_options.CosCut = 0.0;//this->ui.lineEdit_Ramp_CosineCut->text().toDouble();
  // In Andreases code this was initialized as 0.0
  fdk_options.HammCut = 0.0;//this->ui.lineEdit_Ramp_Hamming->text().toDouble();
  // In Andreases code this was aldready checked so we replace with true (checkBox_UseDDF)
  fdk_options.displacedDetectorFilter = true;//this->ui.checkBox_UseDDF->isChecked();
  // In Andreases code this was not checked so we replace with false (checkBox_UpdateAfterFiltering)
  fdk_options.updateAfterDDF = false;//this->ui.checkBox_UpdateAfterFiltering->isChecked();
  // In Andreases code this was aldready checked so we replace with true (checkBox_UsePSSF)
  fdk_options.ParkerShortScan = true;//this->ui.checkBox_UsePSSF->isChecked();

  // In Andreases code thesse three was initialized as 1
  fdk_options.ct_spacing[0] = 1;//this->ui.lineEdit_outImgSp_AP->text().toDouble();
  fdk_options.ct_spacing[1] = 1;//this->ui.lineEdit_outImgSp_SI->text().toDouble();
  fdk_options.ct_spacing[2] = 1;//this->ui.lineEdit_outImgSp_LR->text().toDouble();

  // In Andreases code thesse three was initialized as 400
  fdk_options.ct_size[0] = 400;//this->ui.lineEdit_outImgDim_AP->text().toInt();
  // In Andreases code thesse three was initialized as 200
  fdk_options.ct_size[1] = 200;//this->ui.lineEdit_outImgDim_SI->text().toInt();
  // In Andreases code thesse three was initialized as 400
  fdk_options.ct_size[2] = 200;//this->ui.lineEdit_outImgDim_LR->text().toInt();

  // In Andreases these three is set earlier in the code but the UI standard is implemented. Hope this works
  fdk_options.medianRadius[0] = 0;//this->ui.lineEdit_PostMedSizeX->text().toInt(); // radius along x
  fdk_options.medianRadius[1] = 0;//this->ui.lineEdit_PostMedSizeY->text().toInt(); // radius along y
  fdk_options.medianRadius[2] = 1;//this->ui.lineEdit_PostMedSizeZ->text().toInt(); // radius along z

  // In Andreases code this was aldready checked so we replace with true (checkBox_PostMedianOn)
  fdk_options.medianFilter = true;//this->ui.checkBox_PostMedianOn->isChecked();

  fdk_options.outputFilePath = QString("");// this->ui.lineEdit_OutputFilePath->text(); // In Andreases UI it says that this is optional

  return fdk_options;
}
