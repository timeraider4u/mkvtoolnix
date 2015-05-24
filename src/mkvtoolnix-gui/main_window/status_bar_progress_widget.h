#ifndef MTX_MKVTOOLNIX_GUI_MAIN_WINDOW_STATUS_BAR_PROGRESS_WIDGET_H
#define MTX_MKVTOOLNIX_GUI_MAIN_WINDOW_STATUS_BAR_PROGRESS_WIDGET_H

#include "common/common_pch.h"

#include <QWidget>

class QTreeView;

namespace mtx { namespace gui {

namespace Ui {
class StatusBarProgressWidget;
}

class StatusBarProgressWidget : public QWidget {
  Q_OBJECT;

protected:
  std::unique_ptr<Ui::StatusBarProgressWidget> ui;
  int m_numPendingAuto{}, m_numPendingManual{};

public:
  explicit StatusBarProgressWidget(QWidget *parent = nullptr);
  ~StatusBarProgressWidget();

  void retranslateUi();

public slots:
  void setProgress(int progress, int totalProgress);
  void setJobStats(int numPendingAutomatic, int numPendingManual, int numOther);
};

}}

#endif // MTX_MKVTOOLNIX_GUI_MAIN_WINDOW_STATUS_BAR_PROGRESS_WIDGET_H
