#include "scui.h"
#include "ui_scui.h"

#include "qyklabel.h"
#include "progressbar.h"
#include "informationwindow.h"

// QT
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>

// ITK
#ifdef OF
#undef OF
#endif
#include "itkGDCMSeriesFileNames.h"

// PLM
#ifdef OF
#undef OF
#endif
#include "mha_io.h"

//Local
#include "cbctrecon_io.h"
#include "loadingthread.h"
#include "scattercorrectingthread.h"
#include "weplthread.h"

//For loading CBCTRecon
#include <iostream>
#include <shellapi.h>

enum enCOLOR { // Colors for WEPL display
  RED,
  BLUE,
};

Scui::Scui(QWidget *parent) // Constructor
    : QMainWindow(parent)
    , ui(new Ui::Scui)

{   
    ui->setupUi(this); // Sets up the ui

    // Adding ellements to the threatment region combobox
    ui->comboBox_region->addItem("Head-Neck");
    ui->comboBox_region->addItem("Pelvis");
    ui->comboBox_region->addItem("Thorax");

    // Initialising the structure combobox
    ui->comboBoxWEPL->addItem("No structures");

    // Adding elements to the plan view combobox
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

    //Color for combobox // Maybe not used..
    QPalette p = ui->comboBox_region->palette();
    p.setColor(QPalette::Highlight, Qt::transparent);
    ui->comboBox_region->setPalette(p);

    this->m_cbctrecon = std::make_unique<CbctRecon>(); // Creates a unique object of cbctrecon
    this->m_cbctregistration =
        std::make_unique<CbctRegistration>(this->m_cbctrecon.get()); // Creates a unique object of cbctregistration

    m_YKImgFixed = &m_cbctregistration->m_YKImgFixed[0]; // Pointer for display only
    m_YKImgMoving = &m_cbctregistration->m_YKImgMoving[0];// Pointer for display only
    m_YKDisp = &m_cbctregistration->m_YKDisp[0];// Pointer for display only

    m_YKDispRaw = &m_cbctregistration->m_YKDisp[0];
    m_YKImgRawFixed = &m_cbctregistration->m_YKImgFixed[0];
    m_YKImgRawMoving = &m_cbctregistration->m_YKImgMoving[0];

    m_DoseImgFixed = &m_cbctregistration->m_DoseImgFixed[0]; // Pointer for display only
    m_DoseImgMoving = &m_cbctregistration->m_DoseImgMoving[0]; // Pointer for display only
    m_AGDisp_Overlay = &m_cbctregistration->m_AGDisp_Overlay[0]; // Pointer for display only

    //Loading thread and signals:
    lThread = new LoadingThread(this);
    connect(lThread,SIGNAL(SignalMessageBox(int,QString,QString)), this, SLOT(ShowMessageBox(int, QString, Qstring)));
    connect(lThread,SIGNAL(Signal_SetButtonsAfterLoad()),this, SLOT(SLT_SetButtonsAfterLoad()));
    connect(lThread,SIGNAL(Signal_UpdateSlider(int)), this, SLOT(SLT_UpdateSlider(int)));
    connect(lThread,SIGNAL(Signal_DisconnectSlider()), this, SLOT(SLT_DisconnectSlider()));
    connect(lThread,SIGNAL(Signal_ReConnectSlider(int)),this,SLOT(SLT_ReConnectSlider(int)));
    connect(lThread,SIGNAL(Signal_UpdateProgressBarLoad(int)),this,SLOT(SLT_UpdateProgressBarLoad(int)));
    connect(lThread,SIGNAL(Signal_LThreadIsDone()),this,SLOT(SLT_LThreadIsDone()));

    //Scatter correcting thread and signals:
    scThread = new ScatterCorrectingThread(this);
    connect(scThread,SIGNAL(Signal_UpdateLabelRaw(QString)), this, SLOT(SLT_UpdateLabelRaw(QString)));
    connect(scThread,SIGNAL(Signal_UpdateLabelCor(QString)), this, SLOT(SLT_UpdateLabelCor(QString)));
    connect(scThread,SIGNAL(Signal_PassFixedImg()),this, SLOT(SLT_PassFixedImgForAnalysis()));
    connect(scThread,SIGNAL(SignalDrawImageInFixedSlice()),this,SLOT(SLT_DrawImageInFixedSlice()));
    connect(scThread,SIGNAL(SignalDrawImageWhenSliceChange()),this,SLOT(SLT_DrawImageWhenSliceChange()));
    connect(scThread,SIGNAL(Signal_UpdateProgressBarSC(int)),this,SLOT(SLT_UpdateProgressBarSC(int)));
    connect(scThread,SIGNAL(Signal_SCThreadIsDone()),this,SLOT(SLT_SCThreadIsDone()));
    connect(scThread,SIGNAL(Signal_UpdateVOICombobox(ctType)),this,SLOT(UpdateVOICombobox(const ctType)));
    connect(scThread,SIGNAL(Signal_FixedImageSelected(QString)),this,SLOT(SLT_FixedImageSelected(QString)));
    connect(scThread,SIGNAL(Signal_MovingImageSelected(QString)),this,SLOT(SLT_MovingImageSelected(QString)));
    connect(scThread,SIGNAL(Signal_ImageManualMoveOneShot(float,float,float)),this,SLOT(ImageManualMoveOneShot(const float,const float,const float)));

    //WEPL thread and signals:
    weplThread = new WEPLThread(this);
    connect(weplThread,SIGNAL(Signal_DrawWEPL()),this,SLOT(SLT_DrawImageWhenSliceChange()));
    connect(weplThread,SIGNAL(Signal_UpdateProgressBarWEPL(int)),this,SLOT(SLT_UpdateProgressBarWEPL(int)));
}

Scui::~Scui() // Destructor
{
    delete ui;
    delete lThread;
    delete scThread;
    delete weplThread;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------UI methods ----------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_OpenInfo() // Is called when the info button is pushed
{
    InformationWindow infoWindow;
    infoWindow.setModal(true);
    infoWindow.exec();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_OpenAdvancedMode() // Is called when the advanced button is pushed
{
    //Link: https://stackoverflow.com/questions/15435994/how-do-i-open-an-exe-from-another-c-exe
    ShellExecute(NULL, "open", "C:\\Users\\ct-10\\CbctRecon\\build-vs19-mt\\bin\\CbctRecon.exe", NULL, NULL, SW_SHOW);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_Exit() // Is called when the exit button is pushed
{
    delete ui;
    delete ui; // Needs to delete to times before this is working...
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_IncreaseSliderValue() // Is called when the + button is pushed
{
    auto curValue = ui->verticalSlider->value();
    ui->verticalSlider->setValue(curValue+1);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_DecreaseSliderValue() // Is called when the - button is pushed
{
    auto curValue = ui->verticalSlider->value();
    ui->verticalSlider->setValue(curValue-1);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_SliderValueChanged(){ // Is called when the slider value changes    
    if(scatterCorrectingIsDone){
        SLT_DrawReconImage();//SLT_DrawReconImageInSlices(); // Add this for future use
        SLT_DrawImageWhenSliceChange();
    }else{
        SLT_DrawReconImage();
        SLT_DrawImageWhenSliceChange();
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::on_comboBoxPlanView_currentIndexChanged(const QString &planView) // Is called when an item is chosen in the plan view
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
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::on_comboBox_region_currentIndexChanged(const QString &region)
{
 if(region.compare("Head-Neck")==0){
     RegionChosen = 0;
 }
 else if(region.compare("Pelvis")==0){
     RegionChosen = 1;
 }
 else if(region.compare("Thorax")==0){
     RegionChosen = 2;
 }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::on_comboBoxWEPL_currentIndexChanged(const QString &structure) // Is called when a new structure is chosen
{
    if(scatterCorrectingIsDone){
        Structure = structure;
        SLT_StartWEPLThread();
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------Loading methods ---------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
QString get_dcm_uid(QString &dcm_path){ // Small method for extracting the DICOM ID.
    using NamesGeneratorType = itk::GDCMSeriesFileNames;
    auto nameGenerator = NamesGeneratorType::New();

    nameGenerator->SetUseSeriesDetails(true);
    nameGenerator->AddSeriesRestriction("0008|0021");
    nameGenerator->SetGlobalWarningDisplay(false);
    nameGenerator->SetDirectory(dcm_path.toStdString());

    const auto &seriesUID = nameGenerator->GetSeriesUIDs();
    auto seriesItr = seriesUID.begin();
return QString(seriesItr->c_str());
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
QString getStructureFile(QString path){ // Small method for finding the structurefile automatic
    QDir directory(path);
    QStringList images = directory.entryList(QStringList() << "*.dcm" << "*.DCM",QDir::Files);
    foreach(QString filename, images) {
    //do whatever you need to do
    bool a = filename.contains(QString("RN"));
            if(a==true){
                std::cout << filename.toStdString() << std::endl;
                return filename;
            }
    }
    std::cout << "No structure file found" << std::endl;
    return QString("");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_StartLoadingThread(){
    /*
    SLT_GetCBCTPath();
    SLT_GetCTPath();
    */
    // Only used for testing. Comment in the two upper methods
    CBCTPath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\2019-07-04_084333_2019-07-04 06-43-22-2985\\1ba28724-69b3-4963-9736-e8ab0788c31f\\Acquisitions\\781076550");
    CTPath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\Plan CT\\E_PT1 plan");
    //
    auto dcm_uid_str = get_dcm_uid(CTPath); // Get the unique DICOM ID from the CT images
    ui->label_Id->setText("ID: "+dcm_uid_str); // Sets the ID based on the unique ID
    //m_cbctrecon->m_strDCMUID = dcm_uid_str; //Maybe incomment this later
    ui->btnLoadData->setEnabled(false);
    QString structureFilename = getStructureFile(CTPath);
    StructurePath = CTPath + "\\" + structureFilename;
    if(CBCTPath != QString("") && CTPath != QString("") && StructurePath != QString("")){
        lThread->start();
    }else{
        std::cout << "An Error occured. Try restart the program" << std::endl;
    }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_GetCBCTPath(){ // Opens file dialog for CBCT-projections
    CBCTPath = QFileDialog::getExistingDirectory(
        this, tr("Open Directory with CBCT"), this->m_cbctrecon->m_strPathDirDefault,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_GetCTPath(){
    CTPath = QFileDialog::getExistingDirectory( // Opens file dialog for CT-projections
        this, tr("Open CT DICOM Directory"), this->m_cbctrecon->m_strPathDirDefault,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_UpdateProgressBarLoad(int progress){ // Updates the loading progressbar
    ui->progressBarLoad->setValue(progress);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_UpdateSlider(int max){ // Sets minimum and maximum on the slider
    this->ui->verticalSlider->setMinimum(1);
    this->ui->verticalSlider->setMaximum(max-1);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_DisconnectSlider(){ // disconnect the slider
    disconnect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_ReConnectSlider(int initVal){ // Connects the slider again
    this->ui->verticalSlider->setValue(initVal);
    connect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));
    SLT_DrawReconImage();
    ui->btnScatterCorrect->setEnabled(true);
    ui->btnScatterCorrect->setStyleSheet("QPushButton{background-color: #1367AB; color: #ffffff;font-size: 18px;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::UpdateReconImage(UShortImageType::Pointer &spNewImg) {// Updates the image on the left. by seeting the input image as the current image
  m_cbctrecon->m_spCrntReconImg = spNewImg;

  const auto &p_curimg = this->m_cbctrecon->m_spCrntReconImg;
  const auto origin_new = p_curimg->GetOrigin();
  const auto spacing_new = p_curimg->GetSpacing();
  const auto size_new = p_curimg->GetBufferedRegion().GetSize();

  std::cout << "New Origin" << origin_new << std::endl;
  std::cout << "New spacing" << spacing_new << std::endl;
  std::cout << "New size" << size_new << std::endl;

  auto size = p_curimg->GetBufferedRegion().GetSize();

  this->m_cbctrecon->m_dspYKReconImage->CreateImage(size[0], size[1], 0);

  disconnect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));
  this->ui->verticalSlider->setMinimum(1);
  this->ui->verticalSlider->setMaximum(size[2] - 1);

  const auto initVal = qRound((size[2] - 1) / 2.0);

  this->ui->verticalSlider->setValue(initVal);

  connect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));

  SLT_DrawReconImage();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_DrawReconImage() { //Draws the image on the left by using the current image.

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
  const auto iSliceNumber = this->ui->verticalSlider->value();
  this->ui->labelSliderIdx->setText(QString("Slice: ") + QString::number(iSliceNumber));
  start[2] = iSliceNumber; // 60

  const auto originZ = m_cbctrecon->m_spCrntReconImg->GetOrigin()[2];
  const auto spacingZ = m_cbctrecon->m_spCrntReconImg->GetSpacing()[2];
  const auto posZ = originZ + iSliceNumber * spacingZ;

  const auto strPosZ = QString("%1").arg(posZ, 0, 'f', 2);

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
      std::move(m_cbctrecon->m_dspYKReconImage)); // dimension should be same automatically.

  const auto physPosX = 0; //Hardcoded value (from lineEdit_PostFOV_X)
  const auto physPosY = 0;//Hardcoded value (from lineEdit_PostFOV_Y)
  const auto physRadius = 190;//Hardcoded value (from lineEdit_PostFOV_R)
  const auto physTablePosY = 120;//Hardcoded value (from lineEdit_PostTablePosY)
  m_cbctrecon->PostApplyFOVDispParam(physPosX, physPosY, physRadius,
                                           physTablePosY);

  auto p_dspykimg = m_cbctrecon->m_dspYKReconImage.get();

  p_dspykimg->m_bDrawFOVCircle = false;
  p_dspykimg->m_bDrawTableLine = false;

  p_dspykimg->FillPixMapMinMax(0,2031);//Hardcoded value (from sliderReconImgMin and sliderReconImgMax)
  this->ui->labelImageRaw->SetBaseImage(p_dspykimg);
  this->ui->labelImageRaw->update();

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_SetButtonsAfterLoad(){ // Sets the button after loading has finished
    ui->labelRawImgTitle->setText("Raw CBCT");
    ui->btnLoadData->setEnabled(false);
    ui->btnLoadData->setStyleSheet("QPushButton{color: rgba(255,255,255,60%);font-size: 18px;border-width: 1.4px; border-color: rgba(0,0,0,60%);border-style: solid; border-radius: 7px;}");
    ui->btnScatterCorrect->setEnabled(true);
    ui->btnScatterCorrect->setStyleSheet("QPushButton{background-color: #1367AB; color: #ffffff;font-size: 18px;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QPushButton:pressed{background-color: #E4A115}");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_LThreadIsDone(){ // Is called when the loading thread has finished. Sets buttons.
    ui->labelRawImgTitle->setText("Raw CBCT");
    ui->btnLoadData->setEnabled(false);
    ui->btnLoadData->setStyleSheet("QPushButton{color: rgba(255,255,255,60%);font-size: 18px;border-width: 1.4px; border-color: rgba(0,0,0,60%);border-style: solid; border-radius: 7px;}");
    ui->btnScatterCorrect->setEnabled(true);
    ui->btnScatterCorrect->setStyleSheet("QPushButton{background-color: #1367AB; color: #ffffff;font-size: 18px;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QPushButton:pressed{background-color: #E4A115}");
    ui->comboBox_region->setEnabled(true);
    ui->comboBox_region->setStyleSheet("QComboBox{font-weight: bold;font-size: 18px;background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));color: #ffffff;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QComboBox QAbstractItemView{selection-background-color: rgba(255,190,56,100%);}QComboBox::drop-down{border: 0px;}QComboBox::down-arrow {image: url(/Users/ct-10/Desktop/down.png);width: 14px;height: 14px;}");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------WEPL methods ----------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Scui::SLT_StartWEPLThread(){ // Starts the WEPL thread
    weplThread->start();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_UpdateProgressBarWEPL(int progress){ // Updates the progressbar for WEPL
    ui->progressBarWEPL->setValue(progress);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Scui::SLT_WEPLcalc(QString structure) { // Calculates the WEPL
  //Get VIO
  const auto voi_name = structure.toStdString();

  const auto gantry_angle = 0;//Hardcoded value (from spinBox_GantryAngle)
  const auto couch_angle = 0;//Hardcoded value (from spinBox_CouchAngle)

  const auto ct_type = get_ctType("COR_CBCT");
  const auto ss = m_cbctrecon->m_structures->get_ss(ct_type);
  m_cbctregistration->cur_voi = ss->get_roi_by_name(voi_name);

  const auto wepl_voi =
      CalculateWEPLtoVOI(m_cbctregistration->cur_voi.get(), gantry_angle,
                         couch_angle, m_spMovingImg, m_spFixedImg);
  m_cbctregistration->WEPL_voi = std::make_unique<Rtss_roi_modern>(*wepl_voi);
  // Draw WEPL
  SLT_DrawImageWhenSliceChange();
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
  ui->comboBoxWEPL->clear();
  for (const auto &voi : struct_set->slist) {
    this->ui->comboBoxWEPL->addItem(QString(voi.name.c_str()));
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by set_points_by_slice(). Is used to visualize WEPL
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
template <typename ImageBase, enPLANE plane, enCOLOR color> // Is also used to visualize WEPL
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
//----------------------------------------------------------Scatter correcting methods -----------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void Scui::SLT_StartScatterCorrectingThread(){ // Set buttons and starts the scatter correcting thread
    ui->comboBox_region->setEnabled(false);
    ui->comboBox_region->setStyleSheet("QComboBox{font-weight: bold;font-size: 18px;background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));color: rgba(255,255,255,60%);border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}QComboBox QAbstractItemView{selection-background-color: rgba(255,190,56,100%);}QComboBox::drop-down{border: 0px;}QComboBox::down-arrow {image: url(/Users/ct-10/Desktop/down.png);width: 14px;height: 14px;}");
    ui->btnScatterCorrect->setEnabled(false);
    scThread->start();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_UpdateLabelRaw(QString string){ // Sets the title on the image on the left
        ui->labelRawImgTitle->setText(string);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_UpdateLabelCor(QString string){ // Sets the title on the image on the right
    ui->labelCorImgTitle->setText(string);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_UpdateProgressBarSC(int progress){ // Updates the scatter correcting progressbar
    ui->progressBarSC->setValue(progress);
}
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
void Scui::SLT_FixedImageSelected(QString selText) {
  LoadImgFromComboBox(0, selText); // here, m_spMovingImg and Fixed images are determined
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_MovingImageSelected(QString selText) {
  LoadImgFromComboBox(1, selText);
  const auto cur_ct = get_ctType(selText);
  UpdateVOICombobox(cur_ct);
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

    whenFixedImgLoaded();
  } else if (idx == 1) {
    m_spMovingImg = spTmpImg.GetPointer();
  }
  SLT_DrawImageWhenSliceChange();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by LoadImgFromComboBox()
void Scui::SLT_DrawImageWhenSliceChange() {
  if (m_spFixedImg == nullptr || m_cbctrecon->m_spRawReconImg == nullptr) {
    return;
  }

  if(scatterCorrectingIsDone){ // Sets the fixed and scatter corrected image to fixed and moving after scatter correction is done
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
        sliderPosIdxY = curPosY;
        sliderPosIdxX = curPosX;
    }else if (View ==1){
        sliderPosIdxZ = curPosZ;
        sliderPosIdxY = ui->verticalSlider->value();
        sliderPosIdxX = curPosX;
    }else if(View ==2){
        sliderPosIdxZ = curPosZ;
        sliderPosIdxY = curPosY;
        sliderPosIdxX = ui->verticalSlider->value();
    }
    break;
  case FRONTAL_SAGITTAL_AXIAL:
      if(View ==0){
          sliderPosIdxZ = ui->verticalSlider->value();
          sliderPosIdxY = curPosY;
          sliderPosIdxX = curPosX;
      }else if (View ==1){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = ui->verticalSlider->value();
          sliderPosIdxX = curPosX;
      }else if(View ==2){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = curPosY;
          sliderPosIdxX = ui->verticalSlider->value();
      }
    break;
  case SAGITTAL_AXIAL_FRONTAL:
      if(View ==0){
          sliderPosIdxZ = ui->verticalSlider->value();
          sliderPosIdxY = curPosY;
          sliderPosIdxX = curPosX;
      }else if (View ==1){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = ui->verticalSlider->value();
          sliderPosIdxX = curPosX;
      }else if(View ==2){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = curPosY;
          sliderPosIdxX = ui->verticalSlider->value();
      }
    break;
  default:
      if(View ==0){
          sliderPosIdxZ = ui->verticalSlider->value();
          sliderPosIdxY = curPosY;
          sliderPosIdxX = curPosX;
      }else if (View ==1){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = ui->verticalSlider->value();
          sliderPosIdxX = curPosX;
      }else if(View ==2){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = curPosY;
          sliderPosIdxX = ui->verticalSlider->value();
      }
    break;
  }
  auto imgOrigin = m_spFixedImg->GetOrigin();
  auto imgSpacing = m_spFixedImg->GetSpacing();

  auto curPhysPos = std::array<double, 3>{{
      imgOrigin[2] + sliderPosIdxZ * imgSpacing[2], // Z in default setting
      imgOrigin[1] + sliderPosIdxY * imgSpacing[1], // Y
      imgOrigin[0] + sliderPosIdxX * imgSpacing[0]  // Z
  }};

  const auto refIdx = 3 - m_enViewArrange;
  if (false){//false because we don't want yellow lines on the images (from checkBoxDrawCrosshair)
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

  ////Update Origin text box
  auto imgOriginFixed = m_spFixedImg->GetOrigin();
  QString strOriFixed;
  strOriFixed.sprintf("%3.4f, %3.4f, %3.4f", imgOriginFixed[0], imgOriginFixed[1], imgOriginFixed[2]);

  if (m_spMovingImg != nullptr) {
    const auto imgOriginMoving = m_spMovingImg->GetOrigin();
    QString strOriMoving;
    strOriMoving.sprintf("%3.4f, %3.4f, %3.4f", imgOriginMoving[0],
                         imgOriginMoving[1], imgOriginMoving[2]);
  }
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
  if (true){// true because we want to split (from checkBoxDrawSplit)
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
    if (false){// false because we don't want to split (from checkBoxDrawSplit)
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
  const auto sliderW1 = 2000;//Hardcoded value (from sliderFixedW)
  const auto sliderW2 = 2000;//Hardcoded value (from sliderMovingW)
  const auto sliderL1 = 1024;//Hardcoded value (from sliderFixedL)
  const auto sliderL2 = 1024;//Hardcoded value (from sliderMovingL)

  m_YKDisp[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
  m_YKDisp[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
  m_YKDisp[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);

  if(View ==0){ // Based on chosen view, diffrent image is shown
      this->ui->labelImageCor->SetBaseImage(&m_YKDisp[0]);
  }else if(View==1){
      this->ui->labelImageCor->SetBaseImage(&m_YKDisp[1]);
  }else if(View==2){
      this->ui->labelImageCor->SetBaseImage(&m_YKDisp[2]);
  }
  // here gPMC results could be checked for and displayed, possibly with
  // modification to the qyklabel class /AGA 02/08/2017
  if (m_cbctregistration->dose_loaded) {
    m_AGDisp_Overlay[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_AGDisp_Overlay[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_AGDisp_Overlay[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);

    if(View==0){// Based on chosen view, diffrent overlay is shown
        this->ui->labelImageCor->SetOverlayImage(&m_AGDisp_Overlay[0]);
    }else if(View==1){
        this->ui->labelImageCor->SetOverlayImage(&m_AGDisp_Overlay[1]);
    }else if(View==2){
        this->ui->labelImageCor->SetOverlayImage(&m_AGDisp_Overlay[2]);
    }
  }
  this->ui->labelImageCor->update();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by LoadImgFromComboBox()

// Display is not included here
void Scui::whenFixedImgLoaded() const {
  if (m_cbctrecon->m_spRawReconImg == nullptr) {
    return;
  }

  auto imgSize = m_cbctrecon->m_spRawReconImg->GetBufferedRegion().GetSize(); // 1016x1016 x z
  const auto curPosZ = static_cast<int>(imgSize[2] / 2);
  const auto curPosY = static_cast<int>(imgSize[1] / 2);
  const auto curPosX = static_cast<int>(imgSize[0] / 2);

  std::cout << "sliderPosDisp1:" << curPosX << " th view" << std::endl;
  std::cout << "sliderPosDisp2:" << curPosY << " th view" << std::endl;
  std::cout << "sliderPosDisp2:" << curPosZ << " th view" << std::endl;

  auto x_split = QPoint(static_cast<int>(0), static_cast<int>(0));
  auto y_split = QPoint(static_cast<int>(0), static_cast<int>(0));
  auto z_split = QPoint(static_cast<int>(0), static_cast<int>(0));

  m_YKDisp[0].SetSplitCenter(x_split);
  m_YKDisp[1].SetSplitCenter(y_split);
  m_YKDisp[2].SetSplitCenter(z_split);

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
  auto imgOriginRef = this->m_cbctrecon->m_spRefCTImg->GetOrigin();

  printf("delta(mm): %3.1f, %3.1f, %3.1f", imgOrigin[0] - imgOriginRef[0],
      imgOrigin[1] - imgOriginRef[1], imgOrigin[2] - imgOriginRef[2]);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_PassFixedImgForAnalysis() { // Is passing the image from the right to the left on the ui.
  if (m_cbctrecon->m_spRawReconImg != nullptr) {
    this->UpdateReconImage(m_cbctrecon->m_spRawReconImg);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_SCThreadIsDone(){ // Is called when the scatter correction thread has finished. Sets buttons
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
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------N/A methods------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
FDK_options Scui::getFDKoptions() const {
  FDK_options fdk_options;
  fdk_options.TruncCorFactor = 1.0;//Hardcoded value(from trail and error, see lineEdit_Ramp_TruncationCorrection)
  fdk_options.HannCutX = 5.0;//Hardcoded value (from trail and error, see lineEdit_Ramp_HannCut)
  fdk_options.HannCutY = 5.0;//Hardcoded value (from trail and error, see lineEdit_Ramp_HannCutY)
  fdk_options.CosCut = 0.0;//Hardcoded value (from trail and error, see lineEdit_Ramp_CosineCut)
  fdk_options.HammCut = 0.0;//Hardcoded value (from trail and error, see lineEdit_Ramp_Hamming)
  fdk_options.displacedDetectorFilter = true;//Hardcoded value (from trail and error, see checkBox_UseDDF)
  fdk_options.updateAfterDDF = false;//Hardcoded value (from trail and error, see checkBox_UpdateAfterFiltering)
  fdk_options.ParkerShortScan = true;//Hardcoded value (from trail and error, see checkBox_UsePSSF)
  fdk_options.ct_spacing[0] = 1;//Hardcoded value (from trail and error, see lineEdit_outImgSp_AP)
  fdk_options.ct_spacing[1] = 1;//Hardcoded value (from trail and error, see lineEdit_outImgSp_SI)
  fdk_options.ct_spacing[2] = 1;//Hardcoded value (from trail and error, see lineEdit_outImgSp_LR)
  fdk_options.ct_size[0] = 400;//Hardcoded value (from trail and error, see lineEdit_outImgDim_AP)
  fdk_options.ct_size[1] = 200;//Hardcoded value (from trail and error, see lineEdit_outImgDim_SI)
  fdk_options.ct_size[2] = 400;//Hardcoded value (from trail and error, see lineEdit_outImgDim_LR)
  fdk_options.medianRadius[0] = 0;//Hardcoded value, radius along x (from trail and error, see lineEdit_PostMedSizeX)
  fdk_options.medianRadius[1] = 0;//Hardcoded value, radius along y (from trail and error, see lineEdit_PostMedSizeY)
  fdk_options.medianRadius[2] = 1;//Hardcoded value, radius along z (from trail and error, see lineEdit_PostMedSizeZ)
  fdk_options.medianFilter = true;//Hardcoded value (from trail and error, see checkBox_PostMedianOn)
  fdk_options.outputFilePath = QString("");//Hardcoded value (from trail and error, see lineEdit_OutputFilePath)
  return fdk_options;
}


void Scui::SLT_ShowMessageBox(int idx, QString header,QString message){ // Not used at the moment..

    if(idx == 1){
        QMessageBox::warning(this, header, message);
        return;
    }
    QMessageBox::warning(this, "warning", "Error on File Name Sorting!");
    return;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//Is called by SLT_LoadSelectedProjFiles
void Scui::SLT_OpenElektaGeomFile() { // I don't think this is used at the moment
  auto strPath = QFileDialog::getOpenFileName(
      this, "Select a single file to open",
      this->m_cbctrecon->m_strPathDirDefault, "Geometry file (*.xml)");
  if (strPath.length() <= 1) {
    return;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void Scui::SLT_DrawReconImageInSlices(){
    // init fixed and moving images
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
          sliderPosIdxY = curPosY;
          sliderPosIdxX = curPosX;
      }else if(View ==1){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = ui->verticalSlider->value();
          sliderPosIdxX = curPosX;
      } else if(View==2){
          sliderPosIdxZ = curPosZ;
          sliderPosIdxY = curPosY;
          sliderPosIdxX = ui->verticalSlider->value();
      }
      break;
    case FRONTAL_SAGITTAL_AXIAL:
        if(View ==0){
            sliderPosIdxZ = ui->verticalSlider->value();
            sliderPosIdxY = curPosY;
            sliderPosIdxX = curPosX;
        }else if(View ==1){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = ui->verticalSlider->value();
            sliderPosIdxX = curPosX;
        } else if(View==2){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = curPosY;
            sliderPosIdxX = ui->verticalSlider->value();
        }
      break;
    case SAGITTAL_AXIAL_FRONTAL:
        if(View ==0){
            sliderPosIdxZ = ui->verticalSlider->value();
            sliderPosIdxY = curPosY;
            sliderPosIdxX = curPosX;
        }else if(View ==1){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = ui->verticalSlider->value();
            sliderPosIdxX = curPosX;
        } else if(View==2){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = curPosY;
            sliderPosIdxX = ui->verticalSlider->value();
        }
      break;
    default:
        if(View ==0){
            sliderPosIdxZ = ui->verticalSlider->value();
            sliderPosIdxY = curPosY;
            sliderPosIdxX = curPosX;
        }else if(View ==1){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = ui->verticalSlider->value();
            sliderPosIdxX = curPosX;
        } else if(View==2){
            sliderPosIdxZ = curPosZ;
            sliderPosIdxY = curPosY;
            sliderPosIdxX = ui->verticalSlider->value();
        }
      break;
    }
    auto imgOrigin = m_spRawFixedImg->GetOrigin();
    auto imgSpacing = m_spRawFixedImg->GetSpacing();

    auto curPhysPos = std::array<double, 3>{{
        imgOrigin[2] + sliderPosIdxZ * imgSpacing[2], // Z in default setting
        imgOrigin[1] + sliderPosIdxY * imgSpacing[1], // Y
        imgOrigin[0] + sliderPosIdxX * imgSpacing[0]  // Z
    }};

    const auto refIdx = 3 - m_enViewArrangeRaw;
    if (false){// False, because we don't want yellow lines on the images (from checkBoxDrawCrosshair)
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
      if (false){ //False, because we don't want to display dose on the left image
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
      if (false){//False, because we don't want to display dose on the left image
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

    ////Update Origin text box
    auto imgOriginFixed = m_spRawFixedImg->GetOrigin();
    QString strOriFixed;
    strOriFixed.sprintf("%3.4f, %3.4f, %3.4f", imgOriginFixed[0], imgOriginFixed[1], imgOriginFixed[2]);

    if (m_spRawMovingImg != nullptr) {
      const auto imgOriginMoving = m_spRawMovingImg->GetOrigin();
      QString strOriMoving;
      strOriMoving.sprintf("%3.4f, %3.4f, %3.4f", imgOriginMoving[0],
                           imgOriginMoving[1], imgOriginMoving[2]);
    }
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
    if (true){// True, because we want to split (from checkBoxDrawSplit)
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
    if (false){//False, because we doesnøt want to display dose on the left image
      if (false){//False (from checkBoxDrawSplit)
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
    const auto sliderW1 = 2000;//Hardcoded value (from sliderFixedW)
    const auto sliderW2 = 2000;//Hardcoded value (from sliderMovingW)
    const auto sliderL1 = 1024;//Hardcoded value (from sliderFixedL)
    const auto sliderL2 = 1024;//Hardcoded value (from sliderMovingL)

    m_YKDispRaw[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_YKDispRaw[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
    m_YKDispRaw[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);

    if(View ==0){
        this->ui->labelImageRaw->SetBaseImage(&m_YKDispRaw[0]);
    }else if(View==1){
        this->ui->labelImageRaw->SetBaseImage(&m_YKDispRaw[1]);
    }else if(View ==2){
        this->ui->labelImageRaw->SetBaseImage(&m_YKDispRaw[2]);
    }

    // here gPMC results could be checked for and displayed, possibly with
    // modification to the qyklabel class /AGA 02/08/2017
    if (false){ //False, because we don't want to display dose on the left image
      m_AGDisp_Overlay[0].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
      m_AGDisp_Overlay[1].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);
      m_AGDisp_Overlay[2].FillPixMapDual(sliderL1, sliderL2, sliderW1, sliderW2);

      if(View==0){
          this->ui->labelImageRaw->SetOverlayImage(&m_AGDisp_Overlay[0]);
      }else if(View==1){
          this->ui->labelImageRaw->SetOverlayImage(&m_AGDisp_Overlay[1]);
      }else if(View ==2){
          this->ui->labelImageRaw->SetOverlayImage(&m_AGDisp_Overlay[2]);
      }
    }
    this->ui->labelImageRaw->update();
}


