#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qyklabel.h"
#include "progressbar.h"
#include "informationwindow.h"
//#include <qcombobox.h>

// QT
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>

// PLM
#include "mha_io.h"

//Local
#include "cbctrecon_io.h"

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
    ui->comboBox->addItem("Head-Neck");
    ui->comboBox->addItem("Pelvis");
    ui->comboBox->addItem("Thorax");


    this->m_cbctrecon = std::make_unique<CbctRecon>();
    this->m_cbctregistration =
        std::make_unique<CbctRegistration>(this->m_cbctrecon.get());

}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::on_btnLoadCT_clicked()
{
    Progressbar progressbar;
    progressbar.setModal(true);
    progressbar.exec();
}

void MainWindow::on_btnInfo_clicked()
{
    InformationWindow infoWindow;
    infoWindow.setModal(true);
    infoWindow.exec();
}
*/

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
// Is called when the "Load CT" button is pushed
void MainWindow::SLT_SetHisDir() // Initialize all image buffer
{
  // Initializing..

  // Set folder --> then use RTK HIS Reader
  auto dirPath = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), this->m_cbctrecon->m_strPathDirDefault,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (dirPath.length() <= 1) {
    return;
  }

  this ->ui->cbCT->setChecked(true);//this->ui.lineEdit_HisDirPath->setText(dirPath);

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

  auto dirPath = QFileDialog::getExistingDirectory(
      this, tr("Open CT DICOM Directory"), guessDir,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

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
  auto instaRecon = false;
  const auto reply =
      QMessageBox::question(this, "Instant Recon?",
                            "Do you want to reconstruct projections as soon as "
                            "they are loaded?\n(Using the current settings)",
                            QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    instaRecon = true;
  }

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

  switch (this->m_cbctrecon->m_projFormat) {
  case XIM_FORMAT:
    bowtiePath = getBowtiePath(this, calDir);
    if (bowtiePath.length() > 1) {
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

  const auto exclude_ids = this->m_cbctrecon->GetExcludeProjFiles(true, gantryAngleInterval);
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
  //this->ui.labelImageRaw->SetBaseImage(this->m_cbctrecon->m_dspYKImgProj.get());
  //this->ui.labelImageRaw->update();

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

  itk::TimeProbe reconTimeProbe;
  reconTimeProbe.Start();
  // In Andreases UI he uses Cuda, but I think we use OpenCL. Hope this works
  this->m_cbctrecon->DoReconstructionFDK<OPENCL_DEVT>(REGISTER_RAW_CBCT,
                                                      fdk_options);
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

  //this->ui.spinBoxReconImgSliceNo->setMinimum(0);
  //this->ui.spinBoxReconImgSliceNo->setMaximum(fdk_options.ct_size[1] - 1);
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

  fdk_options.outputFilePath = this->ui.lineEdit_OutputFilePath->text(); // In Andreases UI it says that this is optional

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

  this->m_cbctrecon->m_dspYKReconImage->CreateImage(size[0], size[1], 0);
  // In Andreases code this was initialized as 0 (spinBoxReconImgSliceNo)
  disconnect(0, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));
  // In Andreases code this was used to view image slices. Therefore this is outcommented (spinBoxReconImgSliceNo)
  /*
  this->ui.spinBoxReconImgSliceNo->setMinimum(0);
  this->ui.spinBoxReconImgSliceNo->setMaximum(size[2] - 1);

  const auto initVal = qRound((size[2] - 1) / 2.0);
  */
  // SLT_DrawReconImage(); //Update Table, Update Graph

  // m_dspYKReconImage->CreateImage(size_trans[0], size_trans[1],0);
  SLT_InitializeGraphLim();
  // In Andreases code this was used to view image slices. Therefore this is outcommented (spinBoxReconImgSliceNo)
  /*
  this->ui.spinBoxReconImgSliceNo->setValue(initVal);
  */
  //this->ui.radioButton_graph_recon->setChecked(true);

  // In Andreases code this was initialized as 0 (spinBoxReconImgSliceNo)
  connect(0, SIGNAL(valueChanged(int)), this, SLOT(SLT_DrawReconImage()));

  SLT_DrawReconImage();
}

// Is implemented in SLT_LoadSelectedProjFiles()
void MainWindow::SLT_ViewRegistration() const
// default showing function
{
  m_dlgRegistration->UpdateListOfComboBox(0); // combo selection
                                              // signalis called
  m_dlgRegistration->UpdateListOfComboBox(1);
  m_dlgRegistration->show();
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
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// External method implemented from DlgRegistration
// Is called when the "Scatter Correct" button is pushed

void MainWindow::SLT_DoRegistrationRigid() // plastimatch auto registration
{
  // 1) Save current image files
  if (m_spFixed == nullptr || m_spMoving == nullptr) {
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
  writer1->SetInput(m_spFixed);

  auto writer2 = writerType::New();
  writer2->SetFileName(filePathMoving.toLocal8Bit().constData());
  writer2->SetUseCompression(true);
  writer2->SetInput(m_spMoving);

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
      AllocateByRef<UShortImageType, UShortImageType>(m_spFixed, spRoiMask);
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

  UpdateListOfComboBox(0); // combo selection signalis called
  UpdateListOfComboBox(1);

  SelectComboExternal(0, REGISTER_RAW_CBCT); // will call fixedImageSelected
  SelectComboExternal(1, REGISTER_AUTO_RIGID);

  m_cbctregistration->m_strPathXFAutoRigid = filePathXform; // for further use
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
QComboBox *crntCombo;

  if (idx == 0) {
    crntCombo = this->ui.comboBoxImgFixed;
  } else {
    crntCombo = this->ui.comboBoxImgMoving;
  }

  // remove all the list
  crntCombo->clear();
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
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DoRegistrationRegid

// externally change  combo box value
void MainWindow::SelectComboExternal(const int idx, const enREGI_IMAGES iImage) {
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
    m_spFixed = spTmpImg.GetPointer();

    whenFixedImgLoaded();
  } else if (idx == 1) {
    m_spMoving = spTmpImg.GetPointer();
    // In Andreases code this does nothing so far and is therefore  outcommented (whenMovingImgLoaded())
    //whenMovingImgLoaded();
  }

  SLT_DrawImageWhenSliceChange();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by LoadImgFromComboBox()
void MainWindow::SLT_DrawImageWhenSliceChange() {
  if (m_spFixed == nullptr) {
    return;
  }

  int sliderPosIdxZ, sliderPosIdxY, sliderPosIdxX;

  switch (m_enViewArrange) {
  case AXIAL_FRONTAL_SAGITTAL:
    sliderPosIdxZ = this->ui.sliderPosDisp1->value(); // Z corresponds to axial, Y to frontal, X to sagittal
    sliderPosIdxY = this->ui.sliderPosDisp2->value();
    sliderPosIdxX = this->ui.sliderPosDisp3->value();
    break;
  case FRONTAL_SAGITTAL_AXIAL:
    sliderPosIdxY = this->ui.sliderPosDisp1->value();
    sliderPosIdxX = this->ui.sliderPosDisp2->value();
    sliderPosIdxZ = this->ui.sliderPosDisp3->value();

    break;
  case SAGITTAL_AXIAL_FRONTAL:
    sliderPosIdxX = this->ui.sliderPosDisp1->value();
    sliderPosIdxZ = this->ui.sliderPosDisp2->value();
    sliderPosIdxY = this->ui.sliderPosDisp3->value();
    break;
  default:
    sliderPosIdxZ =
        this->ui.sliderPosDisp1
            ->value(); // Z corresponds to axial, Y to frontal, X to sagittal
    sliderPosIdxY = this->ui.sliderPosDisp2->value();
    sliderPosIdxX = this->ui.sliderPosDisp3->value();
    break;
  }

  auto imgSize = m_spFixed->GetBufferedRegion().GetSize(); // 1016x1016 x z
  auto imgOrigin = m_spFixed->GetOrigin();
  auto imgSpacing = m_spFixed->GetSpacing();

  auto curPhysPos = std::array<double, 3>{{
      imgOrigin[2] + sliderPosIdxZ * imgSpacing[2], // Z in default setting
      imgOrigin[1] + sliderPosIdxY * imgSpacing[1], // Y
      imgOrigin[0] + sliderPosIdxX * imgSpacing[0]  // Z
  }};

  const auto refIdx = 3 - m_enViewArrange;

  if (this->ui.checkBoxDrawCrosshair->isChecked()) {
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

  if (m_spMoving != nullptr) {
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixed, m_spMoving, PLANE_AXIAL, curPhysPos[0], m_YKImgFixed[0],
        m_YKImgMoving[0]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixed, m_spMoving, PLANE_FRONTAL, curPhysPos[1], m_YKImgFixed[1],
        m_YKImgMoving[1]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixed, m_spMoving, PLANE_SAGITTAL, curPhysPos[2], m_YKImgFixed[2],
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
        m_spFixed, m_spFixed, PLANE_AXIAL, curPhysPos[0], m_YKImgFixed[0],
        m_YKImgMoving[0]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixed, m_spFixed, PLANE_FRONTAL, curPhysPos[1], m_YKImgFixed[1],
        m_YKImgMoving[1]);
    this->m_cbctrecon->Draw2DFrom3DDouble(
        m_spFixed, m_spFixed, PLANE_SAGITTAL, curPhysPos[2], m_YKImgFixed[2],
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

  this->ui.lineEditCurPosX->setText(strPos3);
  this->ui.lineEditCurPosY->setText(strPos2);
  this->ui.lineEditCurPosZ->setText(strPos1);

  ////Update Origin text box
  auto imgOriginFixed = m_spFixed->GetOrigin();
  QString strOriFixed;
  strOriFixed.sprintf("%3.4f, %3.4f, %3.4f", imgOriginFixed[0],
                      imgOriginFixed[1], imgOriginFixed[2]);
  this->ui.lineEditOriginFixed->setText(strOriFixed);

  if (m_spMoving != nullptr) {
    const auto imgOriginMoving = m_spMoving->GetOrigin();
    QString strOriMoving;
    strOriMoving.sprintf("%3.4f, %3.4f, %3.4f", imgOriginMoving[0],
                         imgOriginMoving[1], imgOriginMoving[2]);
    this->ui.lineEditOriginMoving->setText(strOriMoving);
  }

  auto arr_wnd = std::array<qyklabel *, 3>{{this->ui.labelOverlapWnd1,
                                            this->ui.labelOverlapWnd2,
                                            this->ui.labelOverlapWnd3}};

  if (m_cbctregistration->cur_voi != nullptr) {
    const auto p_cur_voi = m_cbctregistration->cur_voi.get();

    set_points_by_slice<UShortImageType, PLANE_AXIAL, RED>(
        arr_wnd.at(refIdx % 3), p_cur_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);

    set_points_by_slice<UShortImageType, PLANE_FRONTAL, RED>(
        arr_wnd.at((refIdx + 1) % 3), p_cur_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);

    set_points_by_slice<UShortImageType, PLANE_SAGITTAL, RED>(
        arr_wnd.at((refIdx + 2) % 3), p_cur_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);
  }

  if (m_cbctregistration->WEPL_voi != nullptr) {
    const auto p_wepl_voi = m_cbctregistration->WEPL_voi.get();

    set_points_by_slice<UShortImageType, PLANE_AXIAL, GREEN>(
        arr_wnd.at(refIdx % 3), p_wepl_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);

    set_points_by_slice<UShortImageType, PLANE_FRONTAL, GREEN>(
        arr_wnd.at((refIdx + 1) % 3), p_wepl_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);

    set_points_by_slice<UShortImageType, PLANE_SAGITTAL, GREEN>(
        arr_wnd.at((refIdx + 2) % 3), p_wepl_voi, curPhysPos, imgSpacing,
        imgOriginFixed, imgSize);
  }

  SLT_DrawImageInFixedSlice();
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
//Is called by LoadImgFromComboBox()

// Display is not included here
void MainWindow::whenFixedImgLoaded() const {
  if (m_spFixed == nullptr) {
    return;
  }

  auto imgSize = m_spFixed->GetRequestedRegion().GetSize(); // 1016x1016 x z

  // to avoid first unnecessary action.
  disconnect(this->ui.sliderPosDisp1, SIGNAL(valueChanged(int)), this,
             SLOT(SLT_DrawImageWhenSliceChange()));
  disconnect(this->ui.sliderPosDisp2, SIGNAL(valueChanged(int)), this,
             SLOT(SLT_DrawImageWhenSliceChange()));
  disconnect(this->ui.sliderPosDisp3, SIGNAL(valueChanged(int)), this,
             SLOT(SLT_DrawImageWhenSliceChange()));

  disconnect(this->ui.sliderFixedW, SIGNAL(valueChanged(int)), this,
             SLOT(SLT_DrawImageInFixedSlice()));
  disconnect(this->ui.sliderFixedL, SIGNAL(valueChanged(int)), this,
             SLOT(SLT_DrawImageInFixedSlice()));
  disconnect(this->ui.sliderMovingW, SIGNAL(valueChanged(int)), this,
             SLOT(SLT_DrawImageInFixedSlice()));
  disconnect(this->ui.sliderMovingL, SIGNAL(valueChanged(int)), this,
             SLOT(SLT_DrawImageInFixedSlice()));

  initOverlapWndSize();

  this->ui.sliderPosDisp1->setMinimum(0);
  this->ui.sliderPosDisp1->setMaximum(static_cast<int>(imgSize[2] - 1));
  const auto curPosZ = static_cast<int>(imgSize[2] / 2);
  this->ui.sliderPosDisp1->setValue(curPosZ);

  this->ui.sliderPosDisp2->setMinimum(0);
  this->ui.sliderPosDisp2->setMaximum(static_cast<int>(imgSize[1] - 1));
  const auto curPosY = static_cast<int>(imgSize[1] / 2);
  this->ui.sliderPosDisp2->setValue(curPosY);

  this->ui.sliderPosDisp3->setMinimum(0);
  this->ui.sliderPosDisp3->setMaximum(static_cast<int>(imgSize[0] - 1));
  const auto curPosX = static_cast<int>(imgSize[0] / 2);
  this->ui.sliderPosDisp3->setValue(curPosX);

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

  this->ui.sliderFixedW->setValue(iSliderW);
  this->ui.sliderMovingW->setValue(iSliderW);

  this->ui.sliderFixedL->setValue(iSliderL);
  this->ui.sliderMovingL->setValue(iSliderL);

  connect(this->ui.sliderPosDisp1, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageWhenSliceChange()));
  connect(this->ui.sliderPosDisp2, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageWhenSliceChange()));
  connect(this->ui.sliderPosDisp3, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageWhenSliceChange()));

  connect(this->ui.sliderFixedW, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageInFixedSlice()));
  connect(this->ui.sliderFixedL, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageInFixedSlice()));
  connect(this->ui.sliderMovingW, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageInFixedSlice()));
  connect(this->ui.sliderMovingL, SIGNAL(valueChanged(int)), this,
          SLOT(SLT_DrawImageInFixedSlice()));
}





