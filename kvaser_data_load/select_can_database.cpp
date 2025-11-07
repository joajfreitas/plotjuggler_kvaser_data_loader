#include <QFileDialog>
#include <QSettings>

#include "select_can_database.h"
#include "ui_select_can_database.h"

// Public
DialogSelectCanDatabase::DialogSelectCanDatabase(const QStringList& existing_files,
                                                 QWidget* parent)
  : QDialog(parent)
  , ui_(new Ui::DialogSelectCanDatabase)
  , database_locations_(existing_files)
  , use_enhanced_metadata_(true) {
  ui_->setupUi(this);

  // Populate list with existing files
  for (const QString& file : existing_files) {
    ui_->jsonFilesList->addItem(file);
  }

  // Enable OK button if we have files
  ui_->okButton->setEnabled(!existing_files.isEmpty());

  connect(ui_->okButton, &QPushButton::clicked, this, &DialogSelectCanDatabase::Ok);
  connect(ui_->cancelButton, &QPushButton::clicked, this, &DialogSelectCanDatabase::Cancel);
  connect(ui_->addDatabaseButton, &QPushButton::clicked, this, &DialogSelectCanDatabase::AddDatabaseFile);
}

QStringList DialogSelectCanDatabase::GetDatabaseLocations() const {
  return database_locations_;
}

//CanFrameProcessor::CanProtocol DialogSelectCanDatabase::GetCanProtocol() const {
//  return protocol_;
//}

bool DialogSelectCanDatabase::UseEnhancedMetadata() const {
  return use_enhanced_metadata_;
}

DialogSelectCanDatabase::~DialogSelectCanDatabase() {
  delete ui_;
}

// Private slots
void DialogSelectCanDatabase::Ok() {
  accept();
}

void DialogSelectCanDatabase::Cancel() {
  reject();
}

void DialogSelectCanDatabase::AddDatabaseFile() {
  QStringList fileLocations =
      QFileDialog::getOpenFileNames(this, tr("Select CAN database"), QString(), tr("CAN database (*.json)"));

  if (!fileLocations.isEmpty()) {
    for (const QString& location : fileLocations) {
      if (!database_locations_.contains(location)) {
        database_locations_.append(location);
        ui_->jsonFilesList->addItem(location);
      }
    }

    // Enable OK button as soon as we have at least one DBC file
    ui_->okButton->setEnabled(!database_locations_.isEmpty());
  }
}
