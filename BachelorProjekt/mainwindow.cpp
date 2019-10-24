#include "mainwindow.h"
#include "ui_mainwindow.h"

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

//For loading CBCTRecon
#include <iostream>
#include <shellapi.h>

enum enCOLOR {
  RED,
  GREEN,
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   
    ui->setupUi(this);
    //this->ui->cbCT->setStyleSheet("QCheckBox{background-color: #FF0000;}");
    ui->comboBox_region->addItem("Head-Neck");
    ui->comboBox_region->addItem("Pelvis");
    ui->comboBox_region->addItem("Thorax");

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

    // Icon for Exit
    QPixmap pixmapExit("C:\\Users\\ct-10\\OneDrive - Aarhus universitet\\7 Semester ST\\Bachelor\\UI_Kode\\BachelorProject\\pictures\\exit.png");
    QIcon ButtonExit(pixmapExit);
    ui->btnExit->setIcon(ButtonExit);
    ui->btnExit->setIconSize(QSize(30,30));

    //Color for combobox
    QPalette p = ui->comboBox_region->palette();
    p.setColor(QPalette::Highlight, Qt::transparent);
    ui->comboBox_region->setPalette(p);
    /*
    QPixmap pixmapCombobox("C:\\Users\\ct-10\\OneDrive - Aarhus universitet\\7 Semester ST\\Bachelor\\UI_Kode\\BachelorProject\\pictures\\dropdownarrow.png");
    QIcon ComboboxIcon(pixmapCombobox);
    ui->comboBox_region->set
    ui->btnExit->setIconSize(QSize(30,30));

image: url(':/../../pictures/dropdownarrow.png');
    width: 14px;
    height: 14px;
    */


    this->m_cbctrecon = std::make_unique<CbctRecon>();
    this->m_cbctregistration =
        std::make_unique<CbctRegistration>(this->m_cbctrecon.get());

    m_YKImgFixed = &m_cbctregistration->m_YKImgFixed[0];
    m_YKImgMoving = &m_cbctregistration->m_YKImgMoving[0];
    m_YKDisp = &m_cbctregistration->m_YKDisp[0];

    m_DoseImgFixed = &m_cbctregistration->m_DoseImgFixed[0];
    m_DoseImgMoving = &m_cbctregistration->m_DoseImgMoving[0];
    m_AGDisp_Overlay = &m_cbctregistration->m_AGDisp_Overlay[0];
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::SLT_OpenInfo()
{
    InformationWindow infoWindow;
    infoWindow.setModal(true);
    infoWindow.exec();
}
void MainWindow::SLT_OpenAdvancedMode()
{
    //Link: https://stackoverflow.com/questions/15435994/how-do-i-open-an-exe-from-another-c-exe
    ShellExecute(NULL, "open", "C:\\Users\\ct-10\\CbctRecon\\build-vs19-mt\\bin\\CbctRecon.exe", NULL, NULL, SW_MAXIMIZE);
}
void MainWindow::SLT_Exit()
{
    delete ui;
    delete ui;
}
void MainWindow::SLT_IncreaseSliderValue()
{
    auto curValue = ui->verticalSlider->value();
    ui->verticalSlider->setValue(curValue+1);
}
void MainWindow::SLT_DecreaseSliderValue()
{
    auto curValue = ui->verticalSlider->value();
    ui->verticalSlider->setValue(curValue-1);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void MainWindow::foo()
{
    //this->ui->btnLoadCT->setText("Hello");
    //this ->ui->cbCT->setChecked(true);
    //this ->ui->cbCT->setStyleSheet("background-color: yellow");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// External method implemented from DlgRegistration
void MainWindow::init_DlgRegistration(QString &str_dcm_uid) const// init dlgRegistrations
{
    QString strDCMUID = str_dcm_uid;
    m_cbctregistration->SetPlmOutputDir(strDCMUID);

    const UShortImageType::Pointer spNull;
    // unlink all of the pointers
    auto &p_parent = m_cbctrecon;
    p_parent->m_spRefCTImg = spNull;
    p_parent->m_spManualRigidCT =
        spNull; // copied from RefCTImg; ID: RefCT --> Moving Img, cloned
    p_parent->m_spAutoRigidCT = spNull; // ID: AutoRigidCT
    p_parent->m_spDeformedCT1 = spNull; // Deformmation will be carried out based
                                        // on Moving IMage of GUI //AutoDeformCT1
    p_parent->m_spDeformedCT2 = spNull; // AutoDeformCT2
    p_parent->m_spDeformedCT3 = spNull; // AutoDeformCT3
    p_parent->m_spDeformedCT_Final = spNull; // AutoDeformCT3

    //this->ui.checkBoxKeyMoving->setChecked(false);

    // There seems to be a problem with the cropping function:
    // this->ui.checkBoxCropBkgroundCT->setChecked(false);

    //this->ui.lineEditOriginChanged->setText("");

    //UpdateListOfComboBox(0);
    //UpdateListOfComboBox(1);
    // if not found, just skip
    //SelectComboExternal(0, REGISTER_RAW_CBCT);     // will call fixedImageSelected
    //SelectComboExternal(1, REGISTER_MANUAL_RIGID); // WILL BE IGNORED

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Is called when the "Load Data" button is pushed
void MainWindow::SLT_SetHisDir() // Initialize all image buffer
{
  ui->progressBarLoad->setValue(2);

  // Initializing..

   // When we are testing we don't want to use file dialogs and this has therefore been commented out.
  auto dirPath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\2019-07-04_084333_2019-07-04 06-43-22-2985\\1ba28724-69b3-4963-9736-e8ab0788c31f\\Acquisitions\\781076550");
  /*
  // Set folder --> then use RTK HIS Reader
  auto dirPath = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), "C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\2019-07-04_084333_2019-07-04 06-43-22-2985\\1ba28724-69b3-4963-9736-e8ab0788c31f\\Acquisitions\\781076550",//this->m_cbctrecon->m_strPathDirDefault,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
      */

  if (dirPath.length() <= 1) {
    return;
  }

  //this ->ui->cbCT->setChecked(true);//this->ui.lineEdit_HisDirPath->setText(dirPath);

  this->m_cbctrecon->SetProjDir(dirPath);
  init_DlgRegistration(this->m_cbctrecon->m_strDCMUID);

  //this->ui.lineEdit_ElektaGeomPath->setText(this->m_cbctrecon->m_strPathGeomXML);

  float kVp = 0.0;
  float mA = 0.0;
  float ms = 0.0;
  GetXrayParamFromINI(this->m_cbctrecon->m_strPathElektaINI, kVp, mA, ms);

  if (fabs(kVp * mA * ms) > 0.001) {
    // update GUI
    std::cout << "Updating current mAs setting from INI file: "
              << "kVp= " << kVp << ", mA= " << mA << ", ms= " << ms
              << std::endl;
  }
  //this->ui.lineEdit_CurmAs->setText(QString("%1, %2").arg(mA).arg(ms));

  VEC3D couch_trans = {-999, -999,
                       -999}; // mm. In the text file, these values are in cm.
  VEC3D couch_rot = {-999, -999,
                     -999}; // mm. In the text file, these values are in cm.

  const auto res = GetCouchShiftFromINIXVI(
      this->m_cbctrecon->m_strPathElektaINIXVI2, &couch_trans, &couch_rot);

  if (res) {
    const auto strTransX = QString::number(couch_trans.x, 'f', 1);
    const auto strTransY = QString::number(couch_trans.y, 'f', 1);
    const auto strTransZ = QString::number(couch_trans.z, 'f', 1);
    const auto strTransAll = strTransX + "," + strTransY + "," + strTransZ;

    const auto strRotX = QString::number(couch_rot.x, 'f', 1);
    const auto strRotY = QString::number(couch_rot.y, 'f', 1);
    const auto strRotZ = QString::number(couch_rot.z, 'f', 1);

    const auto strRotAll = strRotX + "," + strRotY + "," + strRotZ;

    //this->ui.lineEdit_CouchTrans->setText(strTransAll);
    //this->ui.lineEdit_CouchRot->setText(strRotAll);
  } else {
    //this->ui.lineEdit_CouchTrans->setText("Not available");
    //this->ui.lineEdit_CouchRot->setText("Not available");
  }

  this->m_cbctrecon->m_vSelectedFileNames.clear();

  std::cout << "Push Load button to load projection images" << std::endl;
  SLT_LoadSelectedProjFiles(dirPath);
}
//Is needed for the next method SLT_LoadSelectedProjFiles()
std::tuple<bool, bool> MainWindow::probeUser(const QString &guessDir) {
  // When we are testing we don't want to use file dialogs and this has therefore been commented out.
  auto dirPath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\Plan CT\\E_PT1 plan");
  /*
  auto dirPath = QFileDialog::getExistingDirectory(
      this, tr("Open CT DICOM Directory"), guessDir,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
      */

  auto dcm_success = false;
  if (!(dirPath.length() <= 1)) {

    if (this->m_cbctrecon->ReadDicomDir(dirPath)) {

      //m_dlgRegistration->UpdateVOICombobox(PLAN_CT);
      // UpdateReconImage(m_spRefCTImg, QString("DICOM reference image"));

      this->m_cbctrecon->RegisterImgDuplication(REGISTER_REF_CT,
                                                REGISTER_MANUAL_RIGID);
      dcm_success = true;
    }
  }
  // We comment out this part
  /*
  auto instaRecon = false;

  const auto reply =
      QMessageBox::question(this, "Instant Recon?",
                            "Do you want to reconstruct projections as soon as "
                            "they are loaded?\n(Using the current settings)",
                            QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    instaRecon = true;
  }
  */
  auto instaRecon = true;

  return std::make_tuple(instaRecon, dcm_success);
}

QString getBowtiePath(QWidget *parent, const QDir &calDir) {
  return QFileDialog::getOpenFileName(
      parent, "Find air(+bowtie) filter image for subtraction",
      calDir.absolutePath(), "Projection (*.xim)", nullptr, nullptr);
}

FilterReaderType::Pointer
MainWindow::ReadBowtieFileWhileProbing(const QString &proj_path,
                                            std::tuple<bool, bool> &answers) {

  auto bowtiereader =
      FilterReaderType::New(); // we use is because we need the projections to
                               // be in the same unit (order of magnitude)

  QDir guessDir(proj_path + QString("/../"));

  const auto calDir(proj_path + QString("/Calibrations/"));

  QString bowtiePath;
  ui->progressBarLoad->setValue(20);
  switch (this->m_cbctrecon->m_projFormat) {
  case XIM_FORMAT:
    bowtiePath = QString("C:\\Users\\ct-10\\Desktop\\PatientWithPlan\\2019-07-04_084333_2019-07-04 06-43-22-2985\\1ba28724-69b3-4963-9736-e8ab0788c31f\\Calibrations\\AIR-Full-Bowtie-100KV-Scattergrid-SAD-SID_0\\Current\\FilterBowtie.xim");//getBowtiePath(this, calDir);
    if (bowtiePath.length() > 1) {
      progressbar = new Progressbar(this);
      std::cout << "loading bowtie-filter..." << std::endl;
      std::vector<std::string> filepath;
      filepath.push_back(bowtiePath.toStdString());
      bowtiereader->SetFileNames(filepath);
      // std::thread calc_thread_bowtie(read_bowtie_projection, bowtiereader);
      std::thread calc_thread_bowtie(
          [&bowtiereader] { bowtiereader->Update(); });
      answers = probeUser(guessDir.absolutePath());
      calc_thread_bowtie.join();

    } else {
      answers = probeUser(
          guessDir.absolutePath()); // looks ugly, but allows threading
    }
    break;
  default:
    answers = probeUser(guessDir.absolutePath()); // ^^^
    break;
  }
  if (bowtiePath.length() > 1) {
    return bowtiereader;
  }
  return nullptr;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_SetHisDir()
void MainWindow::SLT_LoadSelectedProjFiles(QString &path) // main loading fuction for projection images
{
  ui->progressBarLoad->setValue(40);
  //this->ui.pushButton_DoRecon->setDisabled(true);
  // 1) Get all projection file names

  auto dirPath = path;
  //.toLocal8Bit().constData();

  if (!QFile::exists(dirPath)) {
    std::cout << "Projection file directory was not found. Retry." << std::endl;
    return;
  }

  auto names = this->m_cbctrecon->GetProjFileNames(dirPath);

  if (this->m_cbctrecon->m_projFormat == HIS_FORMAT &&
      !this->m_cbctrecon->IsFileNameOrderCorrect(names)) {
    std::cout << "Check the file name order" << std::endl;
    QMessageBox::warning(this, "warning", "Error on File Name Sorting!");
    return;
  }

  std::cout << "File name order was cross-checked and found to be OK!"
            << std::endl;

  const auto fullCnt = names.size();
  if (fullCnt <= 0) {
    std::cout << "No projection file was found. Retry." << std::endl;
    return;
  }

  std::cout << fullCnt << "  projection files were found." << std::endl;

  // 2) Elekta Geometry file
  const auto geomPath = this->m_cbctrecon->m_strPathGeomXML;//this->ui.lineEdit_ElektaGeomPath->text();
  QFileInfo geomFileInfo(geomPath);
  //! QFile::exists(geomPath)

  if (!this->m_cbctrecon->LoadGeometry(geomFileInfo, names)) {
    if (!this->m_cbctrecon->m_strError.isEmpty()) {
      QMessageBox::critical(this, "LoadXVIGeometryFile",
                            this->m_cbctrecon->m_strError, QMessageBox::Ok);
    }
  }
  const auto &p_geometry = this->m_cbctrecon->m_spFullGeometry;
  const auto iFullGeoDataSize = p_geometry->GetGantryAngles().size();
  if (iFullGeoDataSize < 1) {
    std::cout << "Not enough projection image (should be > 0)" << std::endl;
    return;
  }

  if (iFullGeoDataSize != fullCnt) {
    if (this->m_cbctrecon->m_projFormat != XIM_FORMAT) {
      std::cout << "Size of geometry data and file numbers are not same! Check "
                   "and retry"
                << std::endl;
      return;
    }

    const auto reply = QMessageBox::question(
        this, "Mismatch in number of files and Geometry information!",
        "Mismatch in number of files and Geometry information!\nHowever, Xim "
        "detected, so it may be safe to continue anyway?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      std::cout << "continuing despite warning..." << std::endl;
    } else {
      return;
    }
  }

  auto angle_gaps = p_geometry->GetAngularGaps(p_geometry->GetSourceAngles());

  auto sum_gap =
      std::accumulate(std::begin(angle_gaps), std::end(angle_gaps), 0.0);
  sum_gap /= itk::Math::pi * 180.0;
  const auto mean_gap = sum_gap / angle_gaps.size();

  std::cout << "AngularGaps Sum (deg):" << sum_gap
            << ", Mean (deg): " << mean_gap << std::endl;
  double gantryAngelIntervalValue = 30;
  const auto gantryAngleInterval = gantryAngelIntervalValue; //this->ui.lineEdit_ManualProjAngleGap->text().toDouble();

  // In Andreases code this was not checked so we outcommet this one (Radio_KeepOriginalAngles)
  // if (this->ui.Radio_KeepOriginalAngles->isChecked())
  /*
  if (this->ui.Radio_ManualProjAngleGap->isChecked()) {
    // bManualGap = true;
    // std::cout << "Input angle gap in deg: " ;
    // cin >> gantryAngleInterval;

    if (gantryAngleInterval < mean_gap) {
      std::cout << "Angle gap size is too small. Terminating the app"
                << std::endl;
      return;
      // bManualGap = false;
    }
  }
  */

  const auto exclude_ids = this->m_cbctrecon->GetExcludeProjFiles(false, gantryAngleInterval);
      //this->ui.Radio_ManualProjAngleGap->isChecked(), gantryAngleInterval);

  this->m_cbctrecon->LoadSelectedProj(exclude_ids, names);

  // Reads the cone beam projections
  using ReaderType = rtk::ProjectionsReader<FloatImageType>;
  auto reader = ReaderType::New();
  reader->SetFileNames(this->m_cbctrecon->m_vSelectedFileNames);
  // TRY_AND_EXIT_ON_ITK_EXCEPTION(
  // std::thread calc_thread(read_projections, reader);
  std::thread calc_thread([&reader]() { reader->Update(); });
  // calc_thread.detach();

  std::cout << "Reader detached from main thread" << std::endl;

  // After reading the whole file,
  // HIS header should be saved
  this->m_cbctrecon->saveHisHeader();

  //  Insta Recon, Dcm read
  const auto geopath = geomFileInfo.absolutePath();
  std::tuple<bool, bool> answers;
  auto bowtie_reader = ReadBowtieFileWhileProbing(geopath, answers);

  calc_thread.join();
  std::cout << "Reader re-attached to main thread" << std::endl;

  this->m_cbctrecon->m_spProjImg3DFloat =
      reader->GetOutput(); // 1024 1024, line integ image
  auto &proj = this->m_cbctrecon->m_spProjImg3DFloat;
  const auto bowtie_proj = bowtie_reader->GetOutput();
  if (bowtie_reader != nullptr) {
    ApplyBowtie(proj, bowtie_proj);
  }
  if (this->m_cbctrecon->m_projFormat == HND_FORMAT) {
    std::cout << "Fitted bowtie-filter correction ongoing..." << std::endl;
    SLT_DoBowtieCorrection();
  }

  saveImageAsMHA<FloatImageType>(this->m_cbctrecon->m_spProjImg3DFloat);
  auto res_factor = 0.5;//this->ui.lineEdit_DownResolFactor->text().toDouble();
  if (!this->m_cbctrecon->ResampleProjections(res_factor)) { // 0.5
    // reset factor if image was not resampled
    auto res_factor =1.0;//this->ui.lineEdit_DownResolFactor->setText("1.0");
  }

  this->m_cbctrecon->m_spProjImgRaw3D =
      this->m_cbctrecon->ConvertLineInt2Intensity(
          this->m_cbctrecon->m_spProjImg3DFloat);
  // if X not 1024 == input size: out_offset =
  // in_offset + (1024*res_f - X*res_f)*out_spacing     <- will still
  // break down at fw_projection

  //this->ui.pushButton_DoRecon->setEnabled(true);

  //this->ui.spinBoxImgIdx->setMinimum(0);
  //this->ui.spinBoxImgIdx->setMaximum(this->m_cbctrecon->m_vSelectedFileNames.size() - 1);
  //this->ui.spinBoxImgIdx->setValue(0); // it doesn't call Draw Event .. don't
                                       // know why.

  this->m_cbctrecon
      ->SetMaxAndMinValueOfProjectionImage(); // update min max projection image

  SLT_InitializeGraphLim();

  this->SLT_DrawProjImages(); // Update Table is called

  if (!std::get<0>(answers)) { // instaRecon
    std::cout
        << "FINISHED!: Loading projection files. Proceed to reconstruction"
        << std::endl;
  } else {
    SLT_DoReconstruction();
  }

  if (std::get<0>(answers) && std::get<1>(answers)) { // CT DCM dir was found
    SLT_ViewRegistration();
  }
}

// Is implemented in SLT_LoadSelectedProjFiles()
void MainWindow::SLT_DoBowtieCorrection() {
  if (this->m_cbctrecon->m_spProjImg3DFloat == nullptr) {
    return;
  }

  if (this->m_cbctrecon->m_projFormat != HND_FORMAT) {
    std::cout
        << "Bow tie filtering should not be used for His data or Xim data!!"
        << std::endl;
    return;
  }
  // In Andreases UI he uses this string. Hope this works
  QString comboBox_fBTcor_String = "1.4571;2.4506;2.6325;0.0095;4.1181";
  const auto strList = comboBox_fBTcor_String.split(';'); //this->ui.comboBox_fBTcor->currentText().split(';');

  this->m_cbctrecon->BowtieByFit(false,strList);//this->ui.checkBox_Fullfan->isChecked(),strList);

  this->m_cbctrecon->SetMaxAndMinValueOfProjectionImage();
  SLT_DrawProjImages();
  std::cout << "Bow-tie correction done." << std::endl;
}

// Is implemented in SLT_LoadSelectedProjFiles()
void MainWindow::SLT_InitializeGraphLim() const {
  // In Andreases code this was checked so we keep this one (radioButton_graph_proj)
  // Set Max Min at graph
  if (true){//this->ui.radioButton_graph_proj->isChecked()) {
    if (this->m_cbctrecon->m_iImgCnt > 0) // if indep raw his images are loaded
    {
      const auto horLen = this->m_cbctrecon->m_dspYKImgProj->m_iWidth;
      // int verLen = m_dspYKImgProj->m_iHeight;

      // set edit maxium min
      const auto strXMin = QString("%1").arg(horLen);
      //this->ui.lineEditXMin->setText("0");
      //this->ui.lineEditXMax->setText(strXMin);

      const auto strYMin =
          QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMin, 0, 'f', 1);
      const auto strYMax =
          QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMax, 0, 'f', 1);

      //this->ui.lineEditYMin->setText(strYMin);
      //this->ui.lineEditYMax->setText(strYMax);
    }

    if (this->m_cbctrecon->m_spProjImg3DFloat == nullptr) {
      return;
    }

    const auto horLen =
        this->m_cbctrecon->m_spProjImg3DFloat->GetBufferedRegion().GetSize()[0];
    // int verLen = m_spProjImg3DFloat->GetBufferedRegion().GetSize()[1];

    // set edit maxium min
    const auto strXMin = QString("%1").arg(horLen);
    //this->ui.lineEditXMin->setText("0");
    //this->ui.lineEditXMax->setText(strXMin);

    const auto strYMin =
        QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMin, 0, 'f', 1);
    const auto strYMax =
        QString("%1").arg(this->m_cbctrecon->m_fProjImgValueMax, 0, 'f', 1);

    //this->ui.lineEditYMin->setText(strYMin);
    //this->ui.lineEditYMax->setText(strYMax);

    // In Andreases code this was not checked so we outcommet this one (radioButton_graph_recon)
  } /*else if (this->ui.radioButton_graph_recon->isChecked()) {
    if (this->m_cbctrecon->m_spCrntReconImg == nullptr) {
      return;
    }

    const auto horLen =
        this->m_cbctrecon->m_spCrntReconImg->GetBufferedRegion().GetSize()[0];
    // int verLen = m_spCrntReconImg->GetBufferedRegion().GetSize()[1];

    // set edit maxium min

    const auto strXMax = QString("%1").arg(horLen);
    this->ui.lineEditXMin->setText("0");
    this->ui.lineEditXMax->setText(strXMax);

    const auto strYMin = QString("%1").arg(0.0, 0, 'f', 1);
    const auto strYMax = QString("%1").arg(2000.0, 0, 'f', 1);

    this->ui.lineEditYMin->setText(strYMin);
    this->ui.lineEditYMax->setText(strYMax);
  }
  */
}
// Is implemented in SLT_LoadSelectedProjFiles()
void MainWindow::SLT_DrawProjImages() {
  if (this->m_cbctrecon->m_dspYKImgProj == nullptr) {
    return;
  }

  if (this->m_cbctrecon->m_iImgCnt > 0) {
    SLT_DrawRawImages();
    //		SLT_DrawGraph();
    SLT_UpdateTable();
    return;
  }

  // int iReqSlice = this->ui.spinBoxImgIdx->value();
  // In Andreases code this was 0 (spinBoxImgIdx)
  if (!this->m_cbctrecon->FillProjForDisplay(0)){//this->ui.spinBoxImgIdx->value())) {
    return;
  }
  // In Andreases code it looks like min was 0 and max was 65535 (sliderRawMin, sliderRawMax)
  this->m_cbctrecon->m_dspYKImgProj->FillPixMapMinMax(0,65535);//this->ui.sliderRawMin->value(), this->ui.sliderRawMax->value());
  // In Andreases code he plots the raw image. Maybe we need to use this later but for now we out comment it.
  this->ui->labelImageRaw->SetBaseImage(this->m_cbctrecon->m_dspYKImgProj.get());
  this->ui->labelImageRaw->update();

  SLT_UpdateTable();
}

// Is implemented in SLT_DrawProjImages()
void MainWindow::SLT_UpdateTable() {

  // In Andreases code this was aldready checked so we outcommet this one (radioButton_graph_proj)
    /*
  if (this->m_cbctrecon->m_spCrntReconImg == nullptr) {
    this->ui.radioButton_graph_proj->setChecked(true);
  }
  */

  // std::cout << "check 1" << std::endl;
  YK16GrayImage *pYKImg;
  auto fMultiPlyFactor = 1.0;
  auto fMinValue = 0.0;

  // In Andreases code this was aldready checked so we replace with true (radioButton_graph_proj)
  if (true){//this->ui.radioButton_graph_proj->isChecked()) {
    pYKImg = this->m_cbctrecon->m_dspYKImgProj
                 .get(); // you may look, but no touching!

    if (this->m_cbctrecon->m_iImgCnt > 0) { // if indep image
      fMultiPlyFactor = 1.0;
    } else {
      fMultiPlyFactor = this->m_cbctrecon->m_multiplyFactor;
      fMinValue = this->m_cbctrecon->m_fProjImgValueMin;
    }
  } else {
    pYKImg = this->m_cbctrecon->m_dspYKReconImage.get();
    fMinValue = 0.0;
  }
  if (pYKImg == nullptr) {
    return;
  }

  // std::cout << "check 2" << std::endl;

  // std::cout << "check 3" << std::endl;
  auto columnSize = 2;
  auto rowSize = pYKImg->m_iHeight;

  /// int rowSize = pYKImg->m_iWidth;
  // In Andreases code this was aldready checked so we replace with true (radioButton_Profile_Hor)
  if (true){//this->ui.radioButton_Profile_Hor->isChecked()) {
    // columnSize = 2;
    rowSize = pYKImg->m_iWidth;
  }

  // std::cout << "check 4" << std::endl;
  m_pTableModel = std::make_unique<QStandardItemModel>(
      rowSize, columnSize, this); // 2 Rows and 3 Columns

  // for (int i = 0 ; i<columnSize ; i++)
  //{
  // QFileInfo tmpInfo = QFileInfo(m_arrYKImage[i].m_strFilePath);
  // m_pTableModel->setHorizontalHeaderItem(0, new
  // QStandardItem(QString("Index"))); m_pTableModel->setHorizontalHeaderItem(0,
  // new QStandardItem(QString("Profile")));
  auto pos_item = std::make_unique<QStandardItem>(QString("Position(mm)"));
  auto val_item = std::make_unique<QStandardItem>(QString("Value"));

  m_pTableModel->setHorizontalHeaderItem(0, pos_item.release());
  m_pTableModel->setHorizontalHeaderItem(1, val_item.release());
  //}

  // std::cout << "check 5" << std::endl;
  // int width = pYKImg->m_iWidth;
  // int height = pYKImg->m_iHeight;
  // int fixedY = qRound(height / 2.0);

  auto originX = 0.0;
  auto originY = 0.0;
  auto spacingX = 1.0;
  auto spacingY = 1.0;
  // In Andreases code this is used for the graph so therefore we out comment this section
  /*
  if (!this->ui.radioButton_graph_proj->isChecked()) {
    if (this->m_cbctrecon->m_spCrntReconImg != nullptr) {
      auto tmpOrigin = this->m_cbctrecon->m_spCrntReconImg->GetOrigin();
      auto tmpSpacing = this->m_cbctrecon->m_spCrntReconImg->GetSpacing();
      originX = tmpOrigin[0];
      originY = tmpOrigin[1];
      spacingX = tmpSpacing[0];
      spacingY = tmpSpacing[1];
    }
  }
  */

  // std::cout << "check 6" << std::endl;
  // In Andreases code this was aldready checked so we replace with true (radioButton_Profile_Hor)
  QVector<qreal> vPos;
  if (true){//this->ui.radioButton_Profile_Hor->isChecked()) {
    for (auto i = 0; i < rowSize; i++) {
      vPos.push_back(originX + i * spacingX);
    }
  } else {
    for (auto i = 0; i < rowSize; i++) {
      vPos.push_back(originY + i * spacingY);
    }
  }
  // In Andreases code this was aldready checked so we replace with true (radioButton_Profile_Hor)
  QVector<qreal> vProfile;
  if (true){//this->ui.radioButton_Profile_Hor->isChecked()) {
    pYKImg->GetProfileData(vProfile, DIRECTION_HOR);
  } else {
    pYKImg->GetProfileData(vProfile, DIRECTION_VER);
  }

  // int i = fixedY;
  for (auto i = 0; i < rowSize; i++) {
    const auto tmpVal1 = vPos[i];
    auto xpos_item =
        std::make_unique<QStandardItem>(QString("%1").arg(tmpVal1));
    m_pTableModel->setItem(i, 0, xpos_item.release());

    const auto tmpVal2 = vProfile[i] / fMultiPlyFactor + fMinValue;
    auto profval_item =
        std::make_unique<QStandardItem>(QString("%1").arg(tmpVal2));
    m_pTableModel->setItem(i, 1, profval_item.release());
  }
  // In Andreases code this is for visualising dose and this is therefore out commented
  /*
  this->ui.tableViewReconImgProfile->setModel(
      m_pTableModel.get()); // also for proj
      */

  // std::cout << "check 7" << std::endl;

  // In Andreases code this is for visualising dose and this is therefore out commented
  //SLT_DrawGraph();
}

// In Andreases code this is for visualising dose and this is therefore out commented
// Is implemented in SLT_UpdateTable()
/*
void MainWindow::SLT_DrawGraph() const
// based on profile
{
  if (m_pTableModel == nullptr) {
    return;
  }

  // Draw only horizontal, center

  QVector<double> vAxisX; // can be rows or columns
  QVector<double> vAxisY;

  // QStandardItemModel 	m_pTableModel.item()
  const auto dataLen = m_pTableModel->rowCount();

  if (dataLen < 1) {
    return;
  }

  // std::cout << "check graph 1" << std::endl;
  this->ui.customPlot->clearGraphs();

  auto minX = 9999.0;
  auto maxX = -1.0;

  for (auto i = 0; i < dataLen; i++) {
    const auto tableItem1 = m_pTableModel->item(i, 0);
    const auto tableItem2 = m_pTableModel->item(i, 1);
    auto tableVal1 = tableItem1->text().toDouble();
    auto tableVal2 = tableItem2->text().toDouble();

    if (minX > tableVal1) {
      minX = tableVal1;
    }
    if (maxX < tableVal1) {
      maxX = tableVal1;
    }

    vAxisX.push_back(tableVal1);
    vAxisY.push_back(tableVal2);
  }

  // std::cout << "check graph 2" << std::endl;

  this->ui.customPlot->addGraph();
  auto p_graph = this->ui.customPlot->graph(0);
  p_graph->setData(vAxisX, vAxisY, true);
  p_graph->setPen(QPen(Qt::blue));
  p_graph->setName("Image profile");

  this->ui.lineEditXMin->setText(QString("%1").arg(minX));
  this->ui.lineEditXMax->setText(QString("%1").arg(maxX));

  const auto tmpXMin = this->ui.lineEditXMin->text().toDouble();
  const auto tmpXMax = this->ui.lineEditXMax->text().toDouble();
  const auto tmpYMin = this->ui.lineEditYMin->text().toDouble();
  const auto tmpYMax = this->ui.lineEditYMax->text().toDouble();

  // std::cout << "check graph 3" << std::endl;

  this->ui.customPlot->xAxis->setRange(tmpXMin, tmpXMax);
  this->ui.customPlot->yAxis->setRange(tmpYMin, tmpYMax);

  this->ui.customPlot->xAxis->setLabel("mm");
  this->ui.customPlot->yAxis->setLabel("Intensity");
  this->ui.customPlot->setWindowTitle("Image Profile");

  // std::cout << "check graph 4" << std::endl;

  this->ui.customPlot->legend->setVisible(false);
  auto legendFont = font();   // start out with MainWindow's font..
  legendFont.setPointSize(9); // and make a bit smaller for legend
  this->ui.customPlot->legend->setFont(legendFont);
  this->ui.customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200)));

  // std::cout << "check graph 5" << std::endl;
  this->ui.customPlot->replot();

  // SLT_UpdateTable();
}
*/

// Is implemented in SLT_LoadSelectedProjFiles()
void MainWindow::SLT_DoReconstruction() {
  const auto fdk_options = getFDKoptions();
  ui->progressBarLoad->setValue(60);
  itk::TimeProbe reconTimeProbe;
  reconTimeProbe.Start();
  // In Andreases UI he uses Cuda, but I think we use OpenCL. Hope this works
  this->m_cbctrecon->DoReconstructionFDK<OPENCL_DEVT>(REGISTER_RAW_CBCT, fdk_options);
  /*
  if (this->ui.radioButton_UseCUDA->isChecked()) {
    this->m_cbctrecon->DoReconstructionFDK<CUDA_DEVT>(REGISTER_RAW_CBCT,
                                                      fdk_options);
  } else if (this->ui.radioButton_UseOpenCL->isChecked()) {
    this->m_cbctrecon->DoReconstructionFDK<OPENCL_DEVT>(REGISTER_RAW_CBCT,
                                                        fdk_options);
  } else {
    this->m_cbctrecon->DoReconstructionFDK<CPU_DEVT>(REGISTER_RAW_CBCT,                                                     fdk_options);
  }
  */

  reconTimeProbe.Stop();
  std::cout << "It took " << reconTimeProbe.GetMean() << ' '
            << reconTimeProbe.GetUnit() << std::endl;
  //this->ui.lineEdit_ReconstructionTime->setText(QString("%1").arg(reconTimeProbe.GetMean()));

  this->ui->verticalSlider->setMinimum(0);//this->ui.spinBoxReconImgSliceNo->setMinimum(0);
  this->ui->verticalSlider->setMaximum(fdk_options.ct_size[1] - 1);//this->ui.spinBoxReconImgSliceNo->setMaximum(fdk_options.ct_size[1] - 1);
  this->ui->labelSliderIdx->setText(QString("Slice: ") + QString::number(qRound(fdk_options.ct_size[1] / 2.0)));
  this->ui->verticalSlider->setValue(qRound(fdk_options.ct_size[1] / 2.0));

  //this->ui.spinBoxReconImgSliceNo->setValue(qRound(fdk_options.ct_size[1] / 2.0)); // DrawReconImage is called automatically

  SLT_DrawProjImages();

  QString update_text("RAW_CBCT");
  UpdateReconImage(this->m_cbctrecon->m_spCrntReconImg, update_text);

  m_dlgRegistration->UpdateListOfComboBox(0); // combo selection
                                              // signalis called
  m_dlgRegistration->UpdateListOfComboBox(1);
  // m_pDlgRegistration->SelectComboExternal(0, REGISTER_RAW_CBCT); // will call
  // fixedImageSelected  m_pDlgRegistration->SelectComboExternal(1,
  // REGISTER_RAW_CBCT );

  // After first reconstruction, set Median size to 0 0 1 for scatter corrected
  // solution
  /* this->ui.lineEdit_PostMedSizeX->setText(QString("%1").arg(0.0));
  this->ui.lineEdit_PostMedSizeY->setText(QString("%1").arg(0.0));
  this->ui.lineEdit_PostMedSizeZ->setText(QString("%1").arg(1.0));*/

  ui->btnLoadData->setEnabled(false);
  ui->btnLoadData->setStyleSheet("QPushButton{background-color: rgba(47,212,75,60%);color: rgba(255,255,255,60%);font-size: 18px;border-width: 1.4px; border-color: rgba(0,0,0,60%);border-style: solid; border-radius: 7px;}");

  ui->progressBarLoad->setValue(100);
  ui->btnScatterCorrect->setEnabled(true);
  ui->btnScatterCorrect->setStyleSheet("QPushButton{background-color: #1367AB; color: #ffffff;font-size: 18px;border-width: 1.4px;border-color: #000000;border-style: solid;border-radius: 7px;}");
  //SLT_PreProcessCT(); // Is added by us. Added for later use
}

// Is called by SLT_DoReconstruction()
FDK_options MainWindow::getFDKoptions() const {
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

// Is called by SLT_DoReconstruction()
void MainWindow::UpdateReconImage(UShortImageType::Pointer &spNewImg,
                                       QString &fileName) {
  this->m_cbctrecon->m_spCrntReconImg = spNewImg;

  const auto &p_curimg = this->m_cbctrecon->m_spCrntReconImg;
  const auto origin_new = p_curimg->GetOrigin();
  const auto spacing_new = p_curimg->GetSpacing();
  const auto size_new = p_curimg->GetBufferedRegion().GetSize();

  std::cout << "New Origin" << origin_new << std::endl;
  std::cout << "New spacing" << spacing_new << std::endl;
  std::cout << "New size" << size_new << std::endl;

  //this->ui.lineEdit_Cur3DFileName->setText(fileName);

  auto size = p_curimg->GetBufferedRegion().GetSize();

  this->m_cbctrecon->m_dspYKReconImage->CreateImage(size[0], size[1], 100);

  disconnect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));
  this->ui->verticalSlider->setMinimum(0);
  this->ui->verticalSlider->setMaximum(size[2] - 1);

  const auto initVal = qRound((size[2] - 1) / 2.0);
  // SLT_DrawReconImage(); //Update Table, Update Graph

  SLT_InitializeGraphLim(); // What about this???


  this->ui->verticalSlider->setValue(initVal);
  //this->ui.radioButton_graph_recon->setChecked(true);

  connect(this->ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));

  SLT_DrawReconImage();
}

// Is implemented in SLT_LoadSelectedProjFiles()
void MainWindow::SLT_ViewRegistration() const
// default showing function
{
  m_dlgRegistration->UpdateListOfComboBox(0); // combo selection
                                              // signalis called
  m_dlgRegistration->UpdateListOfComboBox(1);
  //m_dlgRegistration->show();// Don't know if this should be used in the code
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void MainWindow::SLT_DrawReconImage() {
  if (this->m_cbctrecon->m_dspYKReconImage == nullptr) {
    return;
  }

  if (this->m_cbctrecon->m_spCrntReconImg == nullptr) {
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
  duplicator->SetInputImage(this->m_cbctrecon->m_spCrntReconImg);
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

  const auto originZ = this->m_cbctrecon->m_spCrntReconImg->GetOrigin()[2];
  const auto spacingZ = this->m_cbctrecon->m_spCrntReconImg->GetSpacing()[2];
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
  this->m_cbctrecon->m_dspYKReconImage = YK16GrayImage::CopyItkImage2YKImage(
      pCrnt2D,
      std::move(this->m_cbctrecon->m_dspYKReconImage)); // dimension should be
                                                        // same automatically.

  // m_dspYKReconImage->SaveDataAsRaw("D:\\RawFile.raw"); //410 410 OK

  const auto physPosX = 0;//this->ui.lineEdit_PostFOV_X->text().toFloat();
  const auto physPosY = 0;//this->ui.lineEdit_PostFOV_Y->text().toFloat();
  const auto physRadius = 190;//this->ui.lineEdit_PostFOV_R->text().toFloat();
  const auto physTablePosY = 120;//this->ui.lineEdit_PostTablePosY->text().toFloat();
  this->m_cbctrecon->PostApplyFOVDispParam(physPosX, physPosY, physRadius,
                                           physTablePosY);
  // SLT_UpdatePostProcDispObj();
  auto p_dspykimg = this->m_cbctrecon->m_dspYKReconImage.get();
  if (false){//this->ui.checkBox_PostDispObjOn->isChecked()) {
    p_dspykimg->m_bDrawFOVCircle = true;
    p_dspykimg->m_bDrawTableLine = true;
  }

  else {
    p_dspykimg->m_bDrawFOVCircle = false;
    p_dspykimg->m_bDrawTableLine = false;
  }

  p_dspykimg->FillPixMapMinMax(0,1500);//this->ui.sliderReconImgMin->value(),
                               //this->ui.sliderReconImgMax->value());
  this->ui->labelImageRaw->SetBaseImage(p_dspykimg);
  this->ui->labelImageRaw->update();

  // SLT_DrawGraph();
  SLT_UpdateTable();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//Is called by SLT_LoadSelectedProjFiles
void MainWindow::SLT_OpenElektaGeomFile() {
  auto strPath = QFileDialog::getOpenFileName(
      this, "Select a single file to open",
      this->m_cbctrecon->m_strPathDirDefault, "Geometry file (*.xml)");

  if (strPath.length() <= 1) {
    return;
  }
  //this->ui.lineEdit_ElektaGeomPath->setText(strPath);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void MainWindow::SLT_PreProcessCT() {
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
    if (m_spMoving == nullptr || m_spFixed == nullptr) {
      return;
    }
    const auto fixed_size = m_spFixed->GetLargestPossibleRegion().GetSize();
    const auto moving_size = m_spMoving->GetLargestPossibleRegion().GetSize();
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
      m_cbctregistration->autoPreprocessCT(iAirThresholdShort, m_spFixed,
                                           m_spMoving);
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
// External method implemented from DlgRegistration
// Is called when the "Scatter Correct" button is pushed

void MainWindow::SLT_DoRegistrationRigid() // plastimatch auto registration
{
  // 1) Save current image files
   // m_spFixed replaced with m_spRawReconImg
    // m_spMoving replaced with m_spRefCTImg
  if (m_cbctrecon->m_spRawReconImg == nullptr || m_cbctrecon->m_spRefCTImg == nullptr) {
    return;
  }

  // Before the registration, remove background and fill the air-bubble of the
  // cbct  1) calculate manual shift value  PreProcess_CBCT(); // remove skin
  // and fill bubble before registration

  if (this->m_cbctrecon->m_spRefCTImg == nullptr ||
      this->m_cbctrecon->m_spManualRigidCT == nullptr) {
    return;
  }
 // In Andreases code this was not checked so we make it false (checkBoxKeyMoving) This is moving the pictures with the keyboard
  if (false){// this->ui.checkBoxKeyMoving->isChecked()) {
    SLT_KeyMoving(false);
  }

  /*- Make a synthetic std::vector field according to the translation
        plastimatch synth-vf --fixed [msk_skin.mha] --output
  [xf_manual_trans.mha] --xf-trans "[origin diff (raw - regi)]" plastimatch
  synth-vf --fixed E:\PlastimatchData\DicomEg\OLD\msk_skin.mha --output
  E:\PlastimatchData\DicomEg\OLD\xf_manual_trans.mha --xf-trans "21 -29 1"

        - Move the skin contour according to the std::vector field
        plastimatch warp --input [msk_skin.mha] --output-img
  [msk_skin_manRegi.mha] --xf [xf_manual_trans.mha] plastimatch warp --input
  E:\PlastimatchData\DicomEg\OLD\msk_skin.mha --output-img
  E:\PlastimatchData\DicomEg\OLD\msk_skin_manRegi.mha --xf
  E:\PlastimatchData\DicomEg\OLD\xf_manual_trans.mha*/

  std::cout << "1: writing temporary files" << std::endl;

  // Both image type: Unsigned Short
  auto filePathFixed =
      m_cbctregistration->m_strPathPlastimatch + "/" + "fixed_rigid.mha";
  auto filePathMoving =
      m_cbctregistration->m_strPathPlastimatch + "/" + "moving_rigid.mha";
  auto filePathOutput =
      m_cbctregistration->m_strPathPlastimatch + "/" + "output_rigid.mha";
  const auto filePathXform =
      m_cbctregistration->m_strPathPlastimatch + "/" + "xform_rigid.txt";
  auto filePathROI = m_cbctregistration->m_strPathPlastimatch + "/" +
                     "fixed_roi_rigid.mha"; // optional

  using writerType = itk::ImageFileWriter<UShortImageType>;

  auto writer1 = writerType::New();
  writer1->SetFileName(filePathFixed.toLocal8Bit().constData());
  writer1->SetUseCompression(true);
  writer1->SetInput(m_cbctrecon->m_spRawReconImg);

  auto writer2 = writerType::New();
  writer2->SetFileName(filePathMoving.toLocal8Bit().constData());
  writer2->SetUseCompression(true);
  writer2->SetInput(m_cbctrecon->m_spRefCTImg);

  writer1->Update();
  writer2->Update();
  // In Andreases code this was checked so we make it true (checkBoxUseROIForRigid)
  if (true){//this->ui.checkBoxUseROIForRigid->isChecked()) {
    std::cout << "Creating a ROI mask for Rigid registration " << std::endl;
    // In Andreases code the value was set (lineEditFOVPos)
    auto strFOVGeom = QString("0.0,0.0,190.0"); //this->ui.lineEditFOVPos->text();

    auto strListFOV = strFOVGeom.split(",");
    if (strListFOV.count() == 3) {
      const auto FOV_DcmPosX = strListFOV.at(0).toFloat(); // mm
      const auto FOV_DcmPosY = strListFOV.at(1).toFloat();
      const auto FOV_Radius = strListFOV.at(2).toFloat();

      // Create Image using FixedImage sp

      // Image Pointer here
      UShortImageType::Pointer spRoiMask;
      AllocateByRef<UShortImageType, UShortImageType>(m_cbctrecon->m_spRawReconImg, spRoiMask);
      this->m_cbctrecon->GenerateCylinderMask(spRoiMask, FOV_DcmPosX,
                                                   FOV_DcmPosY, FOV_Radius);

      auto writer3 = writerType::New();
      writer3->SetFileName(filePathROI.toLocal8Bit().constData());
      writer3->SetUseCompression(true);
      writer3->SetInput(spRoiMask);
      writer3->Update();
    }
    std::cout << "2.A:  ROI-based Rigid body registration will be done"
              << std::endl;
  } else {
    filePathROI = QString("");
  }

  // Preprocessing
  // 2) move CT-based skin mask on CBCT based on manual shift
  //  if (m_strPathCTSkin)

  // m_strPathCTSkin is prepared after PreProcessCT()

  std::cout << "2: Creating a plastimatch command file" << std::endl;

  const auto fnCmdRegisterRigid = QString("cmd_register_rigid.txt");
  // QString fnCmdRegisterDeform = "cmd_register_deform.txt";
  auto pathCmdRegister =
      m_cbctregistration->m_strPathPlastimatch + "/" + fnCmdRegisterRigid;

  const auto strDummy = QString("");

  // In Andreases code this was not checked so we make it false (radioButton_mse)
  const auto mse = false;//this->ui.radioButton_mse->isChecked();
  // In Andreases code this was checked but we do not use cuda, so we make it false (radioButton_UseCUDA)
  const auto cuda = false;//m_pParent->ui.radioButton_UseCUDA->isChecked();
  // In Andreases code this was initialized (lineEditGradOption)
  auto GradOptionStr = QString("0.7,0.7,0.7");//this->ui.lineEditGradOption->text();
  // For Cropped patients, FOV mask is applied.

  m_cbctregistration->GenPlastiRegisterCommandFile(
      pathCmdRegister, filePathFixed, filePathMoving, filePathOutput,
      filePathXform, PLAST_RIGID, strDummy, strDummy, strDummy, filePathROI,
      mse, cuda, GradOptionStr);

  std::string str_command_filepath = pathCmdRegister.toLocal8Bit().constData();

  m_cbctregistration->CallingPLMCommand(str_command_filepath);

  std::cout << "5: Reading output image-CT" << std::endl;

  auto reader = itk::ImageFileReader<UShortImageType>::New();
  reader->SetFileName(filePathOutput.toLocal8Bit().constData());
  reader->Update();
  this->m_cbctrecon->m_spAutoRigidCT = reader->GetOutput();

  std::cout << "6: Reading is completed" << std::endl;

  const QFile xform_file(filePathXform);

  // If rigid structure already exists copy it to plan-ct structure before
  // applying rigid again.
  if (!m_cbctregistration->m_pParent->m_structures->is_ss_null<RIGID_CT>()) {
    auto rigid_ss = std::make_unique<Rtss_modern>(
        *(m_cbctregistration->m_pParent->m_structures->get_ss(RIGID_CT)));
    m_cbctregistration->m_pParent->m_structures->set_planCT_ss(
        std::move(rigid_ss));
  }

  const auto transform_success =
      m_cbctregistration->m_pParent->m_structures->ApplyTransformTo<PLAN_CT>(
          xform_file);
  if (transform_success) {
    std::cout << "7: Contours registered" << std::endl;
  }
  // We don't use UpdateListOfComboBox
  /*
  UpdateListOfComboBox(0); // combo selection signalis called
  UpdateListOfComboBox(1);
  */
  /*
  SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  SelectComboExternal(1, REGISTER_AUTO_RIGID);
  */

  // This is used instead of SelectComboExternal
  SLT_FixedImageSelected("RAW_CBCT");
  SLT_MovingImageSelected("MANUAL_RIGID_CT");


  m_cbctregistration->m_strPathXFAutoRigid = filePathXform; // for further use

  SLT_DoRegistrationDeform(); // Is added by us. Defrom registration
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DoRegistrationRegid
void MainWindow::SLT_KeyMoving(const bool bChecked) // Key Moving check box
{
  // In Andreases code he uses checkboxes therefore this is outcommented (radioButton_mse)
  //this->ui.lineEditMovingResol->setDisabled(bChecked);
  if (bChecked) {
    SelectComboExternal(1,REGISTER_MANUAL_RIGID); // should be
  }
  /*
  this->ui.comboBoxImgFixed->setDisabled(bChecked);
  this->ui.comboBoxImgMoving->setDisabled(bChecked);
  this->ui.pushButtonRestoreOriginal->setDisabled(bChecked);
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DoRegistrationRegid
void MainWindow::UpdateListOfComboBox(const int idx) const {
// In Andreases code he uses comboboxes, but we don't, therefore we outcomment these(comboBoxImgFixed, comboBoxImgMoving)
    /*
QComboBox *crntCombo;

  if (idx == 0) {
    crntCombo = this->ui.comboBoxImgFixed;
  } else {
    crntCombo = this->ui.comboBoxImgMoving;
  }

  // remove all the list
  crntCombo->clear();
  */
  /*
  const auto p_parent = m_cbctregistration->m_pParent;

  if (p_parent->m_spRawReconImg != nullptr) {
    crntCombo->addItem("RAW_CBCT");
  }

  if (p_parent->m_spRefCTImg != nullptr) {
    crntCombo->addItem("REF_CT");
  }

  if (p_parent->m_spManualRigidCT != nullptr) {
    crntCombo->addItem("MANUAL_RIGID_CT");
  }

  if (p_parent->m_spAutoRigidCT != nullptr) {
    crntCombo->addItem("AUTO_RIGID_CT");
  }

  if (p_parent->m_spDeformedCT1 != nullptr) {
    crntCombo->addItem("DEFORMED_CT1");
  }

  if (p_parent->m_spDeformedCT2 != nullptr) {
    crntCombo->addItem("DEFORMED_CT2");
  }

  if (p_parent->m_spDeformedCT3 != nullptr) {
    crntCombo->addItem("DEFORMED_CT3");
  }

  if (p_parent->m_spDeformedCT_Final != nullptr) {
    crntCombo->addItem("DEFORMED_CT_FINAL");
  }

  if (p_parent->m_spScatCorrReconImg != nullptr) {
    crntCombo->addItem("COR_CBCT");
  }
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DoRegistrationRegid

// externally change  combo box value
void MainWindow::SelectComboExternal(const int idx, const enREGI_IMAGES iImage) {
  // In Andreases code he uses comboboxes, but we don't, therefore we outcomment these(comboBoxImgFixed, comboBoxImgMoving)
  /*
  QComboBox *crntCombo;

  if (idx == 0) {
    crntCombo = this->ui.comboBoxImgFixed;
  } else if (idx == 1) {
    crntCombo = this->ui.comboBoxImgMoving;
  } else {
    std::cerr << "What did you do to get here?" << std::endl;
    return;
  }

  auto findIndx = -1;
  switch (iImage) {
  case REGISTER_RAW_CBCT:
    findIndx = crntCombo->findText("RAW_CBCT");
    break;
  case REGISTER_REF_CT:
    findIndx = crntCombo->findText("REF_CT");
    break;
  case REGISTER_MANUAL_RIGID:
    findIndx = crntCombo->findText("MANUAL_RIGID_CT");
    break;
  case REGISTER_AUTO_RIGID:
    findIndx = crntCombo->findText("AUTO_RIGID_CT");
    break;
  case REGISTER_DEFORM1:
    findIndx = crntCombo->findText("DEFORMED_CT1");
    break;
  case REGISTER_DEFORM2:
    findIndx = crntCombo->findText("DEFORMED_CT2");
    break;
  case REGISTER_DEFORM3:
    findIndx = crntCombo->findText("DEFORMED_CT3");
    break;
  case REGISTER_DEFORM_FINAL:
    findIndx = crntCombo->findText("DEFORMED_CT_FINAL");
    break;
  case REGISTER_COR_CBCT:
    findIndx = crntCombo->findText("COR_CBCT");
    break;
  case REGISTER_DEFORM_SKIP_AUTORIGID:
    findIndx = crntCombo->findText("REGISTER_DEFORM_SKIP_AUTORIGID");
    break;
  }
  // std::cout << "setCurrentIndx " << findIndx << std::endl;

  if (findIndx < 0) { //-1 if not found
    return;
  }

  crntCombo->setCurrentIndex(findIndx); // will call "SLT_FixedImageSelected"

  // Below are actually redendency. don't know why setCurrentIndex sometimes
  // doesn't trigger the SLT_MovingImageSelected slot func.

  const auto crntStr = crntCombo->currentText();
  if (idx == 0) {
    SLT_FixedImageSelected(crntStr);
  } else if (idx == 1) {
    SLT_MovingImageSelected(crntStr);
  }
  // If it is called inside the Dlg, SLT seemed not to conneced
  */
    /*
  SLT_FixedImageSelected("RAW_CBCT");
  SLT_MovingImageSelected("MANUAL_RIGID_CT"); // This is added by us
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SelectComboExternal
void MainWindow::SLT_FixedImageSelected(QString selText) {
  // QString strCrntText = this->ui.comboBoxImgFixed->currentText();
  LoadImgFromComboBox(0, selText); // here, m_spMoving and Fixed images are determined
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SelectComboExternal
void MainWindow::SLT_MovingImageSelected(QString selText) {
  // QString strCrntText = this->ui.comboBoxImgMoving->currentText();
  // std::cout << "SLT_MovingImageSelected" << std::endl;
  LoadImgFromComboBox(1, selText);
  const auto cur_ct = get_ctType(selText);
  UpdateVOICombobox(cur_ct);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_MovingImageSelected
ctType MainWindow::get_ctType(const QString &selText) {
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
//Is called by SLT_MovingImageSelected
void MainWindow::UpdateVOICombobox(const ctType ct_type) const {
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
  /*
  this->ui.comboBox_VOI->clear();
  for (const auto &voi : struct_set->slist) {
    this->ui.comboBox_VOI->addItem(QString(voi.name.c_str()));
    this->ui.comboBox_VOItoCropBy->addItem(QString(voi.name.c_str()));
    this->ui.comboBox_VOItoCropBy_copy->addItem(QString(voi.name.c_str()));
  }
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_FixedImageSelected and SLT_MovingImageSelected
void MainWindow::LoadImgFromComboBox(const int idx, QString &strSelectedComboTxt) // -->when fixed image loaded will be called here!
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

    //whenFixedImgLoaded(); commented out
  } else if (idx == 1) {
    m_spMovingImg = spTmpImg.GetPointer();
    // In Andreases code this does nothing so far and is therefore  outcommented (whenMovingImgLoaded())
    //whenMovingImgLoaded();
  }

  SLT_DrawImageWhenSliceChange();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by set_points_by_slice()
template <enCOLOR color> auto get_qtpoint_vector(qyklabel *window) {
  switch (color) {
  case RED:
    return &window->m_vPt;
  case GREEN:
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
//Is called by LoadImgFromComboBox()
void MainWindow::SLT_DrawImageWhenSliceChange() {
  if (m_spFixedImg == nullptr) {
    return;
  }

  int sliderPosIdxZ, sliderPosIdxY, sliderPosIdxX;
  // In Andreases code it's hard to tell what the slider values should be, and therefor will they be initialized as 0.
  switch (m_enViewArrange) {
  case AXIAL_FRONTAL_SAGITTAL:
    sliderPosIdxZ = 0;//this->ui.sliderPosDisp1->value(); // Z corresponds to axial, Y to frontal, X to sagittal
    sliderPosIdxY = 0;//this->ui.sliderPosDisp2->value();
    sliderPosIdxX = 0;//this->ui.sliderPosDisp3->value();
    break;
  case FRONTAL_SAGITTAL_AXIAL:
    sliderPosIdxY = 0;//this->ui.sliderPosDisp1->value();
    sliderPosIdxX = 0;//this->ui.sliderPosDisp2->value();
    sliderPosIdxZ = 0;//this->ui.sliderPosDisp3->value();

    break;
  case SAGITTAL_AXIAL_FRONTAL:
    sliderPosIdxX = 0;//this->ui.sliderPosDisp1->value();
    sliderPosIdxZ = 0;//this->ui.sliderPosDisp2->value();
    sliderPosIdxY = 0;//this->ui.sliderPosDisp3->value();
    break;
  default:
    sliderPosIdxZ = 0;//this->ui.sliderPosDisp1->value(); // Z corresponds to axial, Y to frontal, X to sagittal
    sliderPosIdxY = 0;//this->ui.sliderPosDisp2->value();
    sliderPosIdxX = 0;//this->ui.sliderPosDisp3->value();
    break;
  }

  auto imgSize = m_spFixedImg->GetBufferedRegion().GetSize(); // 1016x1016 x z
  auto imgOrigin = m_spFixedImg->GetOrigin();
  auto imgSpacing = m_spFixedImg->GetSpacing();

  auto curPhysPos = std::array<double, 3>{{
      imgOrigin[2] + sliderPosIdxZ * imgSpacing[2], // Z in default setting
      imgOrigin[1] + sliderPosIdxY * imgSpacing[1], // Y
      imgOrigin[0] + sliderPosIdxX * imgSpacing[0]  // Z
  }};

  const auto refIdx = 3 - m_enViewArrange;
  // In Andreases code this checkbox was checked (checkBoxDrawCrosshair)
  if (true){//this->ui.checkBoxDrawCrosshair->isChecked()) {
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

    set_points_by_slice<UShortImageType, PLANE_AXIAL, RED>(
        wnd, p_cur_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);

  }

  if (m_cbctregistration->WEPL_voi != nullptr) {
    const auto p_wepl_voi = m_cbctregistration->WEPL_voi.get();

    set_points_by_slice<UShortImageType, PLANE_AXIAL, GREEN>(
        wnd, p_wepl_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);

  }

  SLT_DrawImageInFixedSlice();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DrawImageWhenSliceChange()
void MainWindow::SLT_DrawImageInFixedSlice() const
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
      // m_YKDisp[i].SetSplitCenter(QPoint dataPt);//From mouse event
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
  this->ui->labelImageCor->SetBaseImage(&m_YKDisp[0]); //this->ui.labelOverlapWnd1->SetBaseImage(&m_YKDisp[0]);
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
    this->ui->labelImageCor->SetOverlayImage(&m_AGDisp_Overlay[0]); //this->ui.labelOverlapWnd1->SetOverlayImage(&m_AGDisp_Overlay[0]);
    /*
    this->ui.labelOverlapWnd2->SetOverlayImage(&m_AGDisp_Overlay[1]);
    this->ui.labelOverlapWnd3->SetOverlayImage(&m_AGDisp_Overlay[2]);
    */
  }
  // In Andreases code he plots this on the registration ui, but we only have one window
  this->ui->labelImageCor->update();//this->ui.labelOverlapWnd1->update();
  /*
  this->ui.labelOverlapWnd2->update();
  this->ui.labelOverlapWnd3->update();
  */
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by LoadImgFromComboBox()

// Display is not included here
void MainWindow::whenFixedImgLoaded() const {
  if (m_cbctrecon->m_spRawReconImg == nullptr) {
    return;
  }

  auto imgSize = m_cbctrecon->m_spRawReconImg->GetBufferedRegion().GetSize(); // 1016x1016 x z

  /*
  // to avoid first unnecessary action.
  // In Andreases code the slider was initiated as 0 (sliderPosDisp1)
  disconnect(0, SIGNAL(valueChanged(int)), this,SLOT(SLT_DrawImageWhenSliceChange()));
  // In Andreases code the slider was initiated as 0 (sliderPosDisp2)
  disconnect(0, SIGNAL(valueChanged(int)), this,SLOT(SLT_DrawImageWhenSliceChange()));
  // In Andreases code the slider was initiated as 0 (sliderPosDisp3)
  disconnect(0, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawImageWhenSliceChange()));
  disconnect(nullptr, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawImageInFixedSlice()));
  // In Andreases code the slider was initiated as 1024 (sliderFixedL)
  disconnect(nullptr, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawImageInFixedSlice()));
  disconnect(nullptr, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawImageInFixedSlice()));
  disconnect(nullptr, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawImageInFixedSlice()));
   */
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
  auto x_split = QPoint(static_cast<int>(imgSize[0] / 2),
                        static_cast<int>(imgSize[1] / 2));
  auto y_split = QPoint(static_cast<int>(imgSize[0] / 2),
                        static_cast<int>(imgSize[2] / 2));
  auto z_split = QPoint(static_cast<int>(imgSize[1] / 2),
                        static_cast<int>(imgSize[2] / 2));

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
  // In Andreases code he uses slider but we don't. Therefore we replace them with nullptr
  /*
  connect(nullptr, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageWhenSliceChange()));
  connect(nullptr, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageWhenSliceChange()));
  connect(nullptr, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageWhenSliceChange()));

  connect(nullptr, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageInFixedSlice()));
  connect(nullptr, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageInFixedSlice()));
  connect(nullptr, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageInFixedSlice()));
  connect(nullptr, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageInFixedSlice()));
  */
  SLT_DrawImageInFixedSlice();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code this method is called when deform registraion button is pushed. Find out where to implement this!

void MainWindow::SLT_DoRegistrationDeform() {
  if (m_cbctrecon->m_spRawReconImg == nullptr || m_cbctrecon->m_spRefCTImg == nullptr) {
    return;
  }
  // In Andreases code this was checked, so we replace this with true (checkBoxCropBkgroundCT)
  const auto bPrepareMaskOnly = !true;//this->ui.checkBoxCropBkgroundCT->isChecked();

  std::cout << "0: DoRegistrationDeform: writing temporary files" << std::endl;

  // Both image type: Unsigned Short
  auto filePathFixed =
      m_cbctregistration->m_strPathPlastimatch + "/" + "fixed_deform.mha";
  auto filePathMoving =
      m_cbctregistration->m_strPathPlastimatch + "/" + "moving_deform.mha";
  auto filePathROI = m_cbctregistration->m_strPathPlastimatch + "/" +
                     "fixed_roi_DIR.mha"; // optional

  const auto filePathOutput =
      m_cbctregistration->m_strPathPlastimatch + "/" + "output_deform.mha";
  const auto filePathXform =
      m_cbctregistration->m_strPathPlastimatch + "/" + "xform_deform.txt";

  auto filePathOutputStage1 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage1.mha";
  auto filePathOutputStage2 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage2.mha";
  auto filePathOutputStage3 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage3.mha";

  using writerType = itk::ImageFileWriter<UShortImageType>;

  auto writer1 = writerType::New();
  writer1->SetFileName(filePathFixed.toLocal8Bit().constData());
  writer1->SetUseCompression(true);
  writer1->SetInput(m_cbctrecon->m_spRawReconImg);

  auto writer2 = writerType::New();
  writer2->SetFileName(filePathMoving.toLocal8Bit().constData());
  writer2->SetUseCompression(true);
  writer2->SetInput(m_cbctrecon->m_spRefCTImg);
  writer1->Update();
  writer2->Update();

  // Create a mask image based on the fixed sp image
  // In Andreases code this was checked, so we replace this with true (checkBoxUseROIForDIR)
  if (true){//this->ui.checkBoxUseROIForDIR->isChecked()) {
    std::cout << "Creating a ROI mask for DIR.. " << std::endl;

    auto strFOVGeom = QString("0.0,0.0,190.0"); //this->ui.lineEditFOVPos->text();

    auto strListFOV = strFOVGeom.split(",");
    if (strListFOV.count() == 3) {
      const auto FOV_DcmPosX = strListFOV.at(0).toFloat(); // mm
      const auto FOV_DcmPosY = strListFOV.at(1).toFloat();
      const auto FOV_Radius = strListFOV.at(2).toFloat();

      // Create Image using FixedImage sp

      // Image Pointer here
      UShortImageType::Pointer spRoiMask;
      AllocateByRef<UShortImageType, UShortImageType>(m_cbctrecon->m_spRawReconImg, spRoiMask);
      m_cbctregistration->m_pParent->GenerateCylinderMask(
          spRoiMask, FOV_DcmPosX, FOV_DcmPosY, FOV_Radius);

      auto writer3 = writerType::New();
      writer3->SetFileName(filePathROI.toLocal8Bit().constData());
      writer3->SetUseCompression(true);
      writer3->SetInput(spRoiMask);
      writer3->Update();
    }
  }

  auto filePathFixed_proc = filePathFixed;

  std::cout << "1: DoRegistrationDeform: CBCT pre-processing before deformable "
               "registration"
            << std::endl;
  // std::cout << "Air region and bubble will be removed" << std::endl;

  QFileInfo info1(m_cbctregistration->m_strPathCTSkin_autoRegi);
  QFileInfo info2(m_cbctregistration->m_strPathXFAutoRigid);

  if (!info1.exists() || !info2.exists()) {
    std::cout << "Fatal error! no CT skin is found or no XF auto file found. "
                 "Preprocessing will not be done. Proceeding."
              << std::endl;
  } else {
    filePathFixed_proc = m_cbctregistration->m_strPathPlastimatch + "/" +
                         "fixed_deform_proc.mha";
    // skin removal and bubble filling : output file = filePathFixed_proc
    // In Andreases code this was not checked, so we replace this with false (checkBoxFillBubbleCT)
    const auto bBubbleRemoval = false;//this->ui.checkBoxFillBubbleCT->isChecked();
    const auto skinExp = 8.0;//this->ui.lineEditCBCTSkinCropBfDIR->text().toDouble();

    const auto iBubThresholdUshort = 500;//this->ui.spinBoxBkDetectCT->value(); // 500
    const auto iBubFillUshort = 1000;//this->ui.spinBoxBubFillCT->value(); // 1000

    m_cbctregistration->ProcessCBCT_beforeDeformRegi(
        filePathFixed, m_cbctregistration->m_strPathCTSkin_autoRegi,
        filePathFixed_proc, m_cbctregistration->m_strPathXFAutoRigid,
        bBubbleRemoval, bPrepareMaskOnly, skinExp, iBubThresholdUshort,
        iBubFillUshort); // bubble filling yes

    if (bPrepareMaskOnly) {
      filePathFixed_proc = filePathFixed;
    }
  }

  std::cout << "2: DoRegistrationDeform: Creating a plastimatch command file"
            << std::endl;

  const auto fnCmdRegisterRigid = QString("cmd_register_deform.txt");
  // QString fnCmdRegisterDeform = "cmd_register_deform.txt";
  auto pathCmdRegister =
      m_cbctregistration->m_strPathPlastimatch + "/" + fnCmdRegisterRigid;

  auto strDeformableStage1 = QString("2,2,1,30,0.00001,0.005,5");//this->ui.lineEditArgument1->text(); // original param: 7, add output path
  auto strDeformableStage2 = QString("");//this->ui.lineEditArgument2->text();
  auto strDeformableStage3 = QString("");//this->ui.lineEditArgument3->text();

  strDeformableStage1.append(", ").append(filePathOutputStage1);
  strDeformableStage2.append(", ").append(filePathOutputStage2);
  strDeformableStage3.append(", ").append(filePathOutputStage3);

  const auto mse = true;//this->ui.radioButton_mse->isChecked();
  const auto cuda = false;//m_pParent->ui.radioButton_UseCUDA->isChecked();
  auto GradOptionStr = QString("0.7,0.7,0.7");//this->ui.lineEditGradOption->text();
  // For Cropped patients, FOV mask is applied.
  if (true){//this->ui.checkBoxUseROIForDIR->isChecked()) {
    m_cbctregistration->GenPlastiRegisterCommandFile(
        pathCmdRegister, filePathFixed_proc, filePathMoving, filePathOutput,
        filePathXform, PLAST_BSPLINE, strDeformableStage1, strDeformableStage2,
        strDeformableStage3, filePathROI, mse, cuda, GradOptionStr);
  } else {
    m_cbctregistration->GenPlastiRegisterCommandFile(
        pathCmdRegister, filePathFixed_proc, filePathMoving, filePathOutput,
        filePathXform, PLAST_BSPLINE, strDeformableStage1, strDeformableStage2,
        strDeformableStage3, QString(), mse, cuda, GradOptionStr);
  }

  std::string str_command_filepath = pathCmdRegister.toLocal8Bit().constData();

  m_cbctregistration->CallingPLMCommand(str_command_filepath);

  std::cout << "5: DoRegistrationDeform: Registration is done" << std::endl;
  std::cout << "6: DoRegistrationDeform: Reading output image" << std::endl;

  using readerType = itk::ImageFileReader<UShortImageType>;

  auto readerDefSt1 = readerType::New();

  auto tmpFileInfo = QFileInfo(filePathOutputStage1);
  if (tmpFileInfo.exists()) {
    readerDefSt1->SetFileName(filePathOutputStage1.toLocal8Bit().constData());
    readerDefSt1->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT1 = readerDefSt1->GetOutput();
  }

  auto readerDefSt2 = readerType::New();
  tmpFileInfo = QFileInfo(filePathOutputStage2);
  if (tmpFileInfo.exists()) {
    readerDefSt2->SetFileName(filePathOutputStage2.toLocal8Bit().constData());
    readerDefSt2->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT2 = readerDefSt2->GetOutput();
  }

  auto readerDefSt3 = readerType::New();
  tmpFileInfo = QFileInfo(filePathOutputStage3);
  if (tmpFileInfo.exists()) {
    readerDefSt3->SetFileName(filePathOutputStage3.toLocal8Bit().constData());
    readerDefSt3->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT3 = readerDefSt3->GetOutput();
  }

  auto readerFinCBCT = readerType::New();
  tmpFileInfo = QFileInfo(filePathFixed_proc); // cropped image Or not
  if (tmpFileInfo.exists()) {
    readerFinCBCT->SetFileName(filePathFixed_proc.toLocal8Bit().constData());
    readerFinCBCT->Update();
    m_cbctregistration->m_pParent->m_spRawReconImg = readerFinCBCT->GetOutput();
  } else {
    std::cout << "No filePathFixed_proc is available. Exit the function"
              << std::endl;
    return;
  }

  // Puncturing should be done for final Deform image

  auto strPathDeformCTFinal = filePathOutput;

  QFileInfo tmpBubFileInfo(m_cbctregistration->m_strPathMskCBCTBubble);

  if (false && tmpBubFileInfo.exists()) {// this->ui.checkBoxFillBubbleCT->isChecked() && tmpBubFileInfo.exists()) {
    std::cout << "6B: final puncturing according to the CBCT bubble"
              << std::endl;

    /*Mask_parms parms_fill;
    strPathDeformCTFinal = m_strPathPlastimatch + "/deformCTpuncFin.mha";

    parms_fill.mask_operation = MASK_OPERATION_FILL;
    parms_fill.input_fn = filePathOutput.toLocal8Bit().constData();
    parms_fill.mask_fn = m_strPathMskCBCTBubble.toLocal8Bit().constData();
    parms_fill.output_fn = strPathDeformCTFinal.toLocal8Bit().constData();*/

    strPathDeformCTFinal =
        m_cbctregistration->m_strPathPlastimatch + "/deformCTpuncFin.mha";

    const auto enMaskOp = MASK_OPERATION_FILL;
    auto input_fn = filePathOutput;
    auto mask_fn = m_cbctregistration->m_strPathMskCBCTBubble;
    auto output_fn = strPathDeformCTFinal;

    // int iBubblePunctureVal = this->ui.lineEditBkFillCT->text().toInt(); //0 =
    // soft tissue
    const auto iBubblePunctureVal =
        0; // 0 = air. deformed CT is now already a USHORT image
    const auto mask_value = iBubblePunctureVal;
    m_cbctregistration->plm_mask_main(enMaskOp, input_fn, mask_fn, output_fn,
                                      static_cast<float>(mask_value));
  }

  auto readerDeformFinal = readerType::New();
  tmpFileInfo = QFileInfo(strPathDeformCTFinal);
  if (tmpFileInfo.exists()) {
    readerDeformFinal->SetFileName(
        strPathDeformCTFinal.toLocal8Bit().constData());
    readerDeformFinal->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT_Final =
        readerDeformFinal->GetOutput();
  } else {
    std::cout << "No final output is available. Exit the function" << std::endl;
    return;
  }

  std::cout << "7: DoRegistrationDeform: Reading is completed" << std::endl;

  const QFile xform_file(filePathXform);
  m_cbctregistration->m_pParent->m_structures->ApplyTransformTo<RIGID_CT>(
      xform_file);

  std::cout << "8: Contours deformed" << std::endl;

  /* Theese does nothing at the moment
  UpdateListOfComboBox(0); // combo selection signalis called
  UpdateListOfComboBox(1);


  SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  SelectComboExternal(1, REGISTER_DEFORM_FINAL);
  */
  std::cout << "FINISHED!: Deformable image registration. Proceed to scatter "
               "correction"
            << std::endl;
  // This is added by us:
  auto FirstString = QString("RAW_CBCT");
  auto SecondString = QString("DEFORM_FINAL");
  LoadImgFromComboBox(0, FirstString);
  LoadImgFromComboBox(1, SecondString);
  //

  SLT_IntensityNormCBCT();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code this method is called when norm CBCT button is pushed. Find out where to implement this!
void MainWindow::SLT_IntensityNormCBCT() {
  const auto fROI_Radius = 30;//this->ui.lineEditNormRoiRadius->text().toFloat();

  std::cout << "Intensity is being analyzed...Please wait." << std::endl;

  float intensitySDFix = 0.0;
  float intensitySDMov = 0.0;
  const auto meanIntensityFix = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spRawReconImg, fROI_Radius, &intensitySDFix);
  const auto meanIntensityMov = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spDeformedCT_Final, fROI_Radius, &intensitySDMov);

  std::cout << "Mean/SD for Fixed = " << meanIntensityFix << "/"
            << intensitySDFix << std::endl;
  std::cout << "Mean/SD for Moving = " << meanIntensityMov << "/"
            << intensitySDMov << std::endl;

  AddConstHU(m_cbctrecon->m_spRawReconImg, static_cast<int>(meanIntensityMov - meanIntensityFix));
  // SLT_PassMovingImgForAnalysis();

  std::cout << "Intensity shifting is done! Added value = "
            << static_cast<int>(meanIntensityMov - meanIntensityFix)
            << std::endl;
  auto update_message =
      QString("Added_%1")
          .arg(static_cast<int>(meanIntensityMov - meanIntensityFix));

  //this->UpdateReconImage(m_cbctrecon->m_spRawReconImg, update_message);//m_pParent->UpdateReconImage(m_spFixed, update_message);
  SelectComboExternal(0, REGISTER_RAW_CBCT);

  SLT_DoScatterCorrection_APRIORI();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void MainWindow::SLT_IntensityNormCBCT_COR_CBCT() {
  const auto fROI_Radius = 30;//this->ui.lineEditNormRoiRadius->text().toFloat();

  std::cout << "Intensity is being analyzed...Please wait." << std::endl;

  float intensitySDFix = 0.0;
  float intensitySDMov = 0.0;
  const auto meanIntensityFix = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spScatCorrReconImg, fROI_Radius, &intensitySDFix);
  const auto meanIntensityMov = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spDeformedCT_Final, fROI_Radius, &intensitySDMov);

  std::cout << "Mean/SD for Fixed = " << meanIntensityFix << "/"
            << intensitySDFix << std::endl;
  std::cout << "Mean/SD for Moving = " << meanIntensityMov << "/"
            << intensitySDMov << std::endl;

  AddConstHU(m_cbctrecon->m_spScatCorrReconImg, static_cast<int>(meanIntensityMov - meanIntensityFix));
  // SLT_PassMovingImgForAnalysis();

  std::cout << "Intensity shifting is done! Added value = "
            << static_cast<int>(meanIntensityMov - meanIntensityFix)
            << std::endl;
  auto update_message =
      QString("Added_%1")
          .arg(static_cast<int>(meanIntensityMov - meanIntensityFix));
  // Maybe this
  SLT_FixedImageSelected("COR_CBCT");
  //SLT_MovingImageSelected("DEFORMED_CT_FINAL");
  //
  //this->UpdateReconImage(m_cbctrecon->m_spRawReconImg, update_message);//m_pParent->UpdateReconImage(m_spFixed, update_message);
  SelectComboExternal(0, REGISTER_COR_CBCT);
  ui->btnScatterCorrect->setStyleSheet("QPushButton{background-color: rgba(47,212,75,60%);color: rgba(255,255,255,60%);font-size: 18px;border-width: 1.4px; border-color: rgba(0,0,0,60%);border-style: solid; border-radius: 7px;}");
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code this method is called when Scatter correct button is pushed. Find out where to implement this!
void MainWindow::SLT_DoScatterCorrection_APRIORI() {

  if ((this->m_cbctrecon->m_spRefCTImg == nullptr &&
       m_cbctrecon->m_spRefCTImg == nullptr) ||//m_dlgRegistration->m_spMoving == nullptr) ||
      m_cbctrecon->m_spRawReconImg ==nullptr){//m_dlgRegistration->m_spFixed == nullptr) {
    std::cerr
        << "Error!: No ref or no fixed image for forward projection is found."
        << "\n";
    return;
  }

  const auto bExportProj_Fwd = false;// this->ui.checkBox_ExportFwd->isChecked();
  const auto bExportProj_Scat = false;//this->ui.checkBox_ExportScat->isChecked();
  const auto bExportProj_Cor = false;//this->ui.checkBox_ExportCor->isChecked();

  // ForwardProjection(m_spRefCTImg, m_spCustomGeometry, m_spProjImgCT3D,
  // false); //final moving image
  if (bExportProj_Fwd) {
    this->m_cbctrecon->m_strPathPatientDir = QFileDialog::getExistingDirectory(
        this, tr("Open Directory"), ".",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  }

  // Overwrite the raw projections by re-projecting them from the potentially
  // modified CBCT
  std::cerr << "Custom Geometry: \n";
  m_cbctrecon->m_spCustomGeometry->Print(std::cerr);

  auto spProjImg3DFloat =
      this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
          m_cbctrecon->m_spRawReconImg, this->m_cbctrecon->m_spCustomGeometry, //m_dlgRegistration->m_spFixed, this->m_cbctrecon->m_spCustomGeometry,
          bExportProj_Fwd, false);//this->ui.radioButton_UseCUDA->isChecked());

  FloatImageType::Pointer p_projimg;
  if (m_cbctrecon->m_spRefCTImg != nullptr){//m_dlgRegistration->m_spMoving != nullptr) {
    p_projimg = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
        m_cbctrecon->m_spRefCTImg, this->m_cbctrecon->m_spCustomGeometry,//m_dlgRegistration->m_spMoving, this->m_cbctrecon->m_spCustomGeometry,
        bExportProj_Fwd,
        false);//this->ui.radioButton_UseCUDA->isChecked()); // final moving image
  } else if (this->m_cbctrecon->m_spRefCTImg != nullptr) {
    std::cerr << "No Moving image in Registration is found. Ref CT image will "
                 "be used instead"
              << "\n";
    p_projimg = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
        this->m_cbctrecon->m_spRefCTImg, this->m_cbctrecon->m_spCustomGeometry,
        bExportProj_Fwd,
        false);//this->ui.radioButton_UseCUDA->isChecked()); // final moving image
  }

  // YKTEMP
  auto projsize = p_projimg->GetBufferedRegion().GetSize();
  std::cout << "ProjImgCT Size = " << projsize[0] << ", " << projsize[1] << ", "
            << projsize[2] << "\n";
  std::cout << "ProjImgCT origin = " << p_projimg->GetOrigin()[0] << ", "
            << p_projimg->GetOrigin()[1] << ", " << p_projimg->GetOrigin()[2]
            << "\n";
  std::cout << "ProjImgCT spacing = " << p_projimg->GetSpacing()[0] << ", "
            << p_projimg->GetSpacing()[1] << ", " << p_projimg->GetSpacing()[2]
            << std::endl;

  // double scaResam = this->ui.lineEdit_scaResam->text().toDouble();
  const auto scaMedian = 12.0;//this->ui.lineEdit_scaMedian->text().toDouble();
  const auto scaGaussian = 0.05;//this->ui.lineEdit_scaGaussian->text().toDouble();

  std::cout << "Generating scatter map is ongoing..." << std::endl;

  this->m_cbctrecon->GenScatterMap_PriorCT(
      spProjImg3DFloat, p_projimg, this->m_cbctrecon->m_spProjImgScat3D,
      scaMedian, scaGaussian,
      bExportProj_Scat); // void GenScatterMap2D_PriorCT()

  std::cout << "To account for the mAs values, the intensity scale factor of "
            << GetRawIntensityScaleFactor(this->m_cbctrecon->m_strRef_mAs,
                                          this->m_cbctrecon->m_strCur_mAs)
            << "was multiplied during scatter correction to avoid negative "
               "scatter"
            << std::endl;
  // In Andreases code he sets text on the ui but we don't. Therefoe this is outcommented
  /*
  this->ui.lineEdit_CurmAs->setText(this->m_cbctrecon->m_strCur_mAs);
  this->ui.lineEdit_RefmAs->setText(this->m_cbctrecon->m_strRef_mAs);
  */

  p_projimg->Initialize(); // memory saving

  std::cout << "Scatter correction is in progress..." << std::endl;

  const auto postScatMedianSize = 3;//this->ui.lineEdit_scaPostMedian->text().toInt();

  this->m_cbctrecon->ScatterCorr_PrioriCT(spProjImg3DFloat,
                                          this->m_cbctrecon->m_spProjImgScat3D,
                                          this->m_cbctrecon->m_spProjImgCorr3D,
                                          postScatMedianSize, bExportProj_Cor);

  this->m_cbctrecon->m_spProjImgScat3D->Initialize(); // memory saving

  std::cout << "AfterCorrectionMacro is ongoing..." << std::endl;

  // In Andreases code he sets elements on his ui. therefore this is outcommented:
  // Update UI
  /*
  this->ui.pushButton_DoRecon->setEnabled(true);
  this->ui.spinBoxImgIdx->setMinimum(0);
  */
  const auto iSizeZ = this->m_cbctrecon->m_spProjImg3DFloat->GetBufferedRegion().GetSize()[2];
  /*
  this->ui.spinBoxImgIdx->setMaximum(iSizeZ - 1);
  this->ui.spinBoxImgIdx->setValue(0);
  */
  this->m_cbctrecon->SetMaxAndMinValueOfProjectionImage(); // update min max projection image
  SLT_InitializeGraphLim();
  SLT_DrawProjImages(); // Update Table is called

  auto fdk_options = getFDKoptions();

  this->m_cbctrecon->AfterScatCorrectionMacro(false,true,false,fdk_options);
              /*
this->ui.radioButton_UseCUDA->isChecked(),
      this->ui.radioButton_UseOpenCL->isChecked(),
      this->ui.checkBox_ExportVolDICOM->isChecked(), fdk_options);
  */

  // Skin removal (using CT contour w/ big margin)
  std::cout
      << "Post  FDK reconstruction is done. Moving on to post skin removal"
      << std::endl;

  const auto voi_name = QString("");//this->m_dlgRegistration->ui.comboBox_VOItoCropBy->currentText();
  // m_cbctregistration->PostSkinRemovingCBCT(
  //    this->m_cbctrecon->m_spScatCorrReconImg, voi_name.toStdString());

  // 20151208 Removal of high intensity skin mask
  // Main issue: raw CBCT projection includes mask, deformed CT doesn't include
  // mask. In case of weight loss, mask signal is independent from skin contour,
  // but deformed CT cannot have that signal.  Therefore, after the subtraction
  // (CBCTcor projections), there is always a big peak. DIR quality doesn't
  // matter because it cannot 'create' mask signal anyway.  Assumption: near the
  // skin contour, this kind of discrepancy is not expected.
  // m_pDlgRegistration->ThermoMaskRemovingCBCT(m_spRawReconImg,
  // m_spScatCorrReconImg, threshold_HU);

  m_dlgRegistration->UpdateListOfComboBox(0); // combo selection signalis
                                              // called
  m_dlgRegistration->UpdateListOfComboBox(1);

  // Methods down below has been changed due to comboboxes
  // SLT_FixedImageSelected("RAW_CBCT");//m_dlgRegistration->SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  //m_dlgRegistration->SelectComboExternal(1, REGISTER_COR_CBCT);

  // m_dlgRegistration->SLT_DoLowerMaskIntensity(); // it will check the check
  // button.

  SLT_DrawProjImages();

  std::cout << "Updating ReconImage..";
  auto updated_text = QString("Scatter corrected CBCT");
  UpdateReconImage(this->m_cbctrecon->m_spScatCorrReconImg,
                   updated_text); // main GUI update

  std::cout << "FINISHED!Scatter correction: CBCT DICOM files are saved"
            << std::endl;
  SLT_IntensityNormCBCT_COR_CBCT();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code he goes back to the registration UI. selects COR_CBCT and DEFORMED and then he clicks pass.
// Automatise that




