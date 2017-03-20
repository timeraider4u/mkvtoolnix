#include "common/common_pch.h"

#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
//#include <QTimer>

#include <matroska/KaxSemantic.h>

#include "common/qt.h"
#include "mkvtoolnix-gui/app.h"
#include "mkvtoolnix-gui/forms/extraction_tool/tab.h"
//#include "mkvtoolnix-gui/extraction_tool/attachement_model.h"
#include "mkvtoolnix-gui/extraction_tool/tab.h"
#include "mkvtoolnix-gui/extraction_tool/tool.h"
#include "mkvtoolnix-gui/main_window/main_window.h"
//#include "mkvtoolnix-gui/util/file.h"
#include "mkvtoolnix-gui/util/file_dialog.h"
#include "mkvtoolnix-gui/util/message_box.h"
//#include "mkvtoolnix-gui/util/model.h"
//#include "mkvtoolnix-gui/util/string.h"
#include "mkvtoolnix-gui/util/settings.h"
#include "mkvtoolnix-gui/util/widget.h"

namespace mtx { namespace gui { namespace ExtractionTool {

using namespace mtx::gui;

Tab::Tab(QWidget *parent,
         QString const &fileName)
  : QWidget{parent}
  , ui{new Ui::Tab}
  , m_fileName{fileName}
{
  // Setup UI controls.
  ui->setupUi(this);

  setupUi();

  retranslateUi();
}

Tab::~Tab() {
}

void
Tab::setupUi() {
  auto mw = MainWindow::get();
}

void
Tab::updateFileNameDisplay() {
  auto info = QFileInfo{m_fileName};
  ui->fileName->setText(info.fileName());
  ui->directory->setText(QDir::toNativeSeparators(info.path()));
}

void
Tab::retranslateUi() {
  ui->retranslateUi(this);

  updateFileNameDisplay();

  setupToolTips();

  emit titleChanged();
}

void
Tab::setupToolTips() {
}

QString
Tab::title()
  const {
  return QFileInfo{m_fileName}.fileName();
}

QString const &
Tab::fileName()
  const {
  return m_fileName;
}

void
Tab::resetData() {
}

bool
Tab::areWidgetsEnabled()
  const {
  return ui->elements->isEnabled();
}

void
Tab::load() {
  resetData();

  m_savedState = currentState();
  auto result  = kax_analyzer_c::probe(to_utf8(m_fileName)) ? loadFromMatroskaFile()
               : m_fileName.toLower().endsWith(Q(".mpls"))  ? loadFromMplsFile()
               :                                              loadFromChapterFile();

  if (result.first)
    chaptersLoaded(result.first, result.second);
}

void
Tab::enableNameWidgets(bool enable) {
  for (auto const &widget : m_nameWidgets)
    widget->setEnabled(enable);
}


bool
Tab::isSourceMatroska()
  const {
  return !!m_analyzer;
}

void
Tab::resetData() {
  m_analyzer.reset();
  //m_nameModel->reset();
  //m_attachmentModel->reset();
}

bool
Tab::hasBeenModified()
  const {
  return false;
}

void
Tab::closeTab() {
  emit removeThisTab();
}

QString
Tab::currentState()
  const {
  return QString{};
}

}}}
