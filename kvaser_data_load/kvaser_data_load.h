#pragma once

#include <QObject>
#include <QtPlugin>
#include "PlotJuggler/dataloader_base.h"

#include <unordered_map>

using namespace PJ;

class KvaserDataLoad : public DataLoader
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "facontidavide.PlotJuggler3.DataLoader")
  Q_INTERFACES(PJ::DataLoader)

public:
  KvaserDataLoad();
  virtual const std::vector<const char*>& compatibleFileExtensions() const override;

  bool readDataFromFile(PJ::FileLoadInfo* fileload_info,
                        PlotDataMapRef& destination) override;

  ~KvaserDataLoad() override = default;

  virtual const char* name() const override
  {
    return "Kvaser csv data loader";
  }

protected:
  QSize parseHeader(QFile* file, std::vector<std::string>& ordered_names);
  void addToPlot(PlotDataMapRef& plot_data, std::string name, double time, double value);

private:
  std::vector<const char*> extensions;
  std::string _default_time_axis;
  std::unordered_map<std::string, PlotData*> plots_map{};
  QStringList last_used_database_locations_;
};


