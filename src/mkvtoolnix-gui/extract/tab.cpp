#include "common/common_pch.h"

#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>

#include <matroska/KaxSemantic.h>

#include "common/bitvalue.h"
#include "common/construct.h"
#include "common/ebml.h"
#include "common/mm_io_x.h"
#include "common/mpls.h"
#include "common/qt.h"
#include "common/segmentinfo.h"
#include "common/segment_tracks.h"
#include "common/strings/formatting.h"
#include "common/strings/parsing.h"
#include "common/translation.h"
#include "common/xml/ebml_chapters_converter.h"
#include "mkvtoolnix-gui/app.h"
#include "mkvtoolnix-gui/extract/tab.h"
#include "mkvtoolnix-gui/extract/tool.h"
#include "mkvtoolnix-gui/main_window/main_window.h"
#include "mkvtoolnix-gui/main_window/select_character_set_dialog.h"
#include "mkvtoolnix-gui/util/file.h"
#include "mkvtoolnix-gui/util/file_dialog.h"
#include "mkvtoolnix-gui/util/header_view_manager.h"
#include "mkvtoolnix-gui/util/message_box.h"
#include "mkvtoolnix-gui/util/model.h"
#include "mkvtoolnix-gui/util/settings.h"
#include "mkvtoolnix-gui/util/widget.h"

namespace mtx { namespace gui { namespace Extract {

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
  Util::Settings::get().handleSplitterSizes(ui->chapterEditorSplitter);

  m_nameWidgets << ui->pbChRemoveName
                << ui->lChName         << ui->leChName
                << ui->lChNameLanguage << ui->cbChNameLanguage
                << ui->lChNameCountry  << ui->cbChNameCountry;

  Util::fixScrollAreaBackground(ui->scrollArea);
  Util::fixComboBoxViewWidth(*ui->cbChNameLanguage);
  Util::fixComboBoxViewWidth(*ui->cbChNameCountry);
  Util::HeaderViewManager::create(*ui->elements,  "ChapterEditor::Elements");
  Util::HeaderViewManager::create(*ui->tvChNames, "ChapterEditor::ChapterNames");

  auto mw = MainWindow::get();
  
  connect(m_expandAllAction,               &QAction::triggered,                                                    this,                 &Tab::expandAll);
  connect(m_collapseAllAction,             &QAction::triggered,                                                    this,                 &Tab::collapseAll);

  connect(mw,                              &MainWindow::preferencesChanged,                                        ui->cbChNameLanguage, &Util::ComboBoxBase::reInitialize);
  connect(mw,                              &MainWindow::preferencesChanged,                                        ui->cbChNameCountry,  &Util::ComboBoxBase::reInitialize);
}

void
Tab::updateFileNameDisplay() {
  if (!m_fileName.isEmpty()) {
    auto info = QFileInfo{m_fileName};
    ui->fileName->setText(info.fileName());
    ui->directory->setText(QDir::toNativeSeparators(info.path()));

  } else {
    ui->fileName->setText(QY("<Unsaved file>"));
    ui->directory->setText(Q(""));

  }
}

void
Tab::retranslateUi() {
  ui->retranslateUi(this);

  updateFileNameDisplay();

  m_expandAllAction->setText(QY("&Expand all"));
  m_collapseAllAction->setText(QY("&Collapse all"));
  m_addEditionBeforeAction->setText(QY("Add new e&dition before"));
  m_addEditionAfterAction->setText(QY("Add new ed&ition after"));
  m_addChapterBeforeAction->setText(QY("Add new c&hapter before"));
  m_addChapterAfterAction->setText(QY("Add new ch&apter after"));
  m_addSubChapterAction->setText(QY("Add new &sub-chapter inside"));
  m_removeElementAction->setText(QY("&Remove selected edition or chapter"));
  m_duplicateAction->setText(QY("D&uplicate selected edition or chapter"));
  m_massModificationAction->setText(QY("Additional &modifications"));
  m_generateSubChaptersAction->setText(QY("&Generate sub-chapters"));
  m_renumberSubChaptersAction->setText(QY("Re&number sub-chapters"));

  setupToolTips();

  emit titleChanged();
}

void
Tab::setupToolTips() {
  Util::setToolTip(ui->elements, QY("Right-click for actions for editions and chapters"));
  Util::setToolTip(ui->pbBrowseSegmentUID, QY("Select an existing Matroska or WebM file and the GUI will add its segment UID to the input field on the left."));
}

QString
Tab::title()
  const {
  if (m_fileName.isEmpty())
    return QY("<Unsaved file>");
  return QFileInfo{m_fileName}.fileName();
}

QString const &
Tab::fileName()
  const {
  return m_fileName;
}

void
Tab::resetData() {
  m_analyzer.reset();
  m_nameModel->reset();
  m_chapterModel->reset();
}

Tab::LoadResult
Tab::loadFromMatroskaFile() {
  m_analyzer = std::make_unique<QtKaxAnalyzer>(this, m_fileName);

  if (!m_analyzer->set_parse_mode(kax_analyzer_c::parse_mode_fast).set_open_mode(MODE_READ).process()) {
    auto text = Q("%1 %2")
      .arg(QY("The file you tried to open (%1) could not be read successfully.").arg(m_fileName))
      .arg(QY("Possible reasons are: the file is not a Matroska file; the file is write-protected; the file is locked by another process; you do not have permission to access the file."));
    Util::MessageBox::critical(this)->title(QY("File parsing failed")).text(text).exec();
    emit removeThisTab();
    return {};
  }

  auto idx = m_analyzer->find(KaxChapters::ClassInfos.GlobalId);
  if (-1 == idx) {
    Util::MessageBox::critical(this)->title(QY("File parsing failed")).text(QY("The file you tried to open (%1) does not contain any chapters.").arg(m_fileName)).exec();
    emit removeThisTab();
    return {};
  }

  auto chapters = m_analyzer->read_element(idx);
  if (!chapters) {
    Util::MessageBox::critical(this)->title(QY("File parsing failed")).text(QY("The file you tried to open (%1) could not be read successfully.").arg(m_fileName)).exec();
    emit removeThisTab();
  }

  m_analyzer->close_file();

  return { std::static_pointer_cast<KaxChapters>(chapters), true };
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
  loadFromMatroskaFile();
}

void
Tab::expandAll() {
  expandCollapseAll(true);
}

void
Tab::collapseAll() {
  expandCollapseAll(false);
}

void
Tab::expandCollapseAll(bool expand,
                       QModelIndex const &parentIdx) {
  if (parentIdx.isValid())
    ui->elements->setExpanded(parentIdx, expand);

  for (auto row = 0, numRows = m_chapterModel->rowCount(parentIdx); row < numRows; ++row)
    expandCollapseAll(expand, m_chapterModel->index(row, 0, parentIdx));
}

void
Tab::expandInsertedElements(QModelIndex const &parentIdx,
                            int,
                            int) {
  expandCollapseAll(true, parentIdx);
}

QString
Tab::formatEbmlBinary(EbmlBinary *binary) {
  auto value = std::string{};
  auto data  = static_cast<unsigned char const *>(binary ? binary->GetBuffer() : nullptr);

  if (data)
    for (auto end = data + binary->GetSize(); data < end; ++data)
      value += (boost::format("%|1$02x|") % static_cast<unsigned int>(*data)).str();

  return Q(value);
}

bool
Tab::isSourceMatroska()
  const {
  return !!m_analyzer;
}

bool
Tab::hasBeenModified()
  const {
  return currentState() != m_savedState;
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
