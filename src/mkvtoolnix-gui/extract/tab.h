#ifndef MTX_MKVTOOLNIX_GUI_EXTRACT_TAB_H
#define MTX_MKVTOOLNIX_GUI_EXTRACT_TAB_H

#include "common/common_pch.h"

#include <QDateTime>
#include <QModelIndex>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "common/qt_kax_analyzer.h"
#include "common/timestamp.h"
#include "mkvtoolnix-gui/extract/attachment_model.h"
#include "mkvtoolnix-gui/types.h"

class QAction;
class QItemSelection;

namespace libebml {
class EbmlBinary;
};

namespace mtx { namespace gui { namespace Extract {

namespace Ui {
class Tab;
}

class AttachmentModel;
//class NameModel;

class Tab : public QWidget {
  Q_OBJECT;

protected:
    using ValidationResult = std::pair<bool, QString>;
    using LoadResult       = std::pair<AttachmentPtr, bool>;

protected:
  // UI stuff:
  std::unique_ptr<Ui::Tab> ui;

  QString m_fileName, m_originalFileName;
  std::unique_ptr<QtKaxAnalyzer> m_analyzer;
  QDateTime m_fileModificationTime;

  QList<QWidget *> m_nameWidgets;

public:
  explicit Tab(QWidget *parent, QString const &fileName = QString{});
  ~Tab();

  virtual void retranslateUi();
  virtual QString const &fileName() const;
  virtual QString title() const;
  virtual bool hasBeenModified() const;
  virtual bool areWidgetsEnabled() const;
  virtual bool isSourceMatroska() const;

signals:
  void removeThisTab();
  void titleChanged();

public slots:
  virtual void load();
  virtual void closeTab();

protected:
  void setupUi();
  void resetData();
  void expandCollapseAll(bool expand, QModelIndex const &parentIdx = {});

  LoadResult loadFromMatroskaFile();

  void updateFileNameDisplay();

protected:
  void setupToolTips();

  QString currentState() const;

  static QString formatEbmlBinary(EbmlBinary *binary);
};

}}}

#endif // MTX_MKVTOOLNIX_GUI_EXTRACT_TAB_H
