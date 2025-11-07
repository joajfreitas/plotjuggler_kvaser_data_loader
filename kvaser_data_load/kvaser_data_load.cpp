#include "kvaser_data_load.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include <QTextStream>

#include "fcp-cpp/src/fcp.hpp"

#include "select_can_database.h"

KvaserDataLoad::KvaserDataLoad() : extensions{"kvaser_csv"} {}

const QRegExp csv_separator("(\\,)");

const std::vector<const char *> &
KvaserDataLoad::compatibleFileExtensions() const {
  return extensions;
}

void KvaserDataLoad::addToPlot(PlotDataMapRef &plot_data, std::string name,
                               double time, double value) {
  auto p = plots_map.find(name);
  if (p != plots_map.end()) {
    PlotData::Point point(time, value);
    p->second->pushBack(point);
  } else {
    auto it = plot_data.addNumeric(name);
    plots_map[name] = &(it->second);
    PlotData::Point point(time, value);
    it->second.pushBack(point);
  }
}

bool KvaserDataLoad::readDataFromFile(FileLoadInfo *info,
                                      PlotDataMapRef &plot_data) {

  QFile file(info->filename);
  if (!file.open(QFile::ReadOnly)) {
    return false;
  }
  QTextStream text_stream(&file);


  DialogSelectCanDatabase* dialog = new DialogSelectCanDatabase(last_used_database_locations_);
  if (dialog->exec() != static_cast<int>(QDialog::Accepted)) {
    delete dialog;
    return false;
  }

  last_used_database_locations_ = dialog->GetDatabaseLocations();
  bool use_enhanced_metadata = dialog->UseEnhancedMetadata();

  // The first line ud contain the name of the columns
  QString first_line = text_stream.readLine();
  QStringList column_names = first_line.split(csv_separator);

  Fcp fcp{};
  fcp.decompile_file(last_used_database_locations_[0].toStdString());

  QProgressDialog progress_dialog;
  progress_dialog.setLabelText("Loading... please wait");
  progress_dialog.setWindowModality(Qt::ApplicationModal);
  progress_dialog.setRange(0, file.size());
  progress_dialog.setAutoClose(true);
  progress_dialog.setAutoReset(true);
  progress_dialog.show();

  //-----------------
  // read the file line by line
  int linecount = 1;
  uint64_t bytes_read{0};
  while (!text_stream.atEnd()) {
    QString line = text_stream.readLine();

    bytes_read += line.size() + 1;
    progress_dialog.setValue(bytes_read);

    linecount++;

    // Split using the comma separator.
    QStringList string_items = line.split(csv_separator);
    if (string_items.size() != column_names.size()) {
      auto err_msg = QString("The number of values at line %1 is %2,\n"
                             "but the expected number of columns is %3.\n"
                             "Aborting...")
                         .arg(linecount)
                         .arg(string_items.size())
                         .arg(column_names.size());

      QMessageBox::warning(nullptr, "Error reading file", err_msg);
      return false;
    }

    bool is_number1;
    bool is_number2;
    double timestamp = string_items[0].toDouble(&is_number1);
    if (!is_number1) {
      continue;
    }

    CANdata msg{0};
    msg.sid = string_items[2].toDouble(&is_number1);
    if (!is_number1) {
      continue;
    }
    msg.dlc = string_items[4].toDouble(&is_number1);
    if (!is_number1) {
      continue;
    }

    msg.data[0] = (string_items[5+1].toUInt(&is_number1) << 8) + string_items[5+0].toUInt(&is_number2);
    if (!is_number1 || !is_number2) {
      continue;
    }
    msg.data[1] = (string_items[5+3].toUInt(&is_number1) << 8) + string_items[5+2].toUInt(&is_number2);
    if (!is_number1 || !is_number2) {
      continue;
    }
    msg.data[2] = (string_items[5+5].toUInt(&is_number1) << 8) + string_items[5+4].toUInt(&is_number2);
    if (!is_number1 || !is_number2) {
      continue;
    }
    msg.data[3] = (string_items[5+7].toUInt(&is_number1) << 8) + string_items[5+6].toUInt(&is_number2);
    if (!is_number1 || !is_number2) {
      continue;
    }

    try {
      const auto decoded = fcp.decode_msg(msg);
      for (const auto &[name, value] : decoded.second) {
        addToPlot(plot_data, name, timestamp, value);
      }
    } catch (const std::exception &e) {
      continue;
    }

  }

  file.close();

  return true;
}
