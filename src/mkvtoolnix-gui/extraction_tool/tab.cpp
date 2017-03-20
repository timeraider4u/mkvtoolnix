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
//   Util::Settings::get().handleSplitterSizes(ui->chapterEditorSplitter);
// 
//   ui->elements->setModel(m_chapterModel);
//   ui->tvChNames->setModel(m_nameModel);
// 
//   ui->cbChNameCountry ->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
// 
//   ui->cbChNameLanguage->setup();
//   ui->cbChNameCountry->setup(true);
// 
//   m_nameWidgets << ui->pbChRemoveName
//                 << ui->lChName         << ui->leChName
//                 << ui->lChNameLanguage << ui->cbChNameLanguage
//                 << ui->lChNameCountry  << ui->cbChNameCountry;
// 
//   Util::fixScrollAreaBackground(ui->scrollArea);
//   Util::fixComboBoxViewWidth(*ui->cbChNameLanguage);
//   Util::fixComboBoxViewWidth(*ui->cbChNameCountry);
//   Util::HeaderViewManager::create(*ui->elements,  "ChapterEditor::Elements");
//   Util::HeaderViewManager::create(*ui->tvChNames, "ChapterEditor::ChapterNames");
// 
//   m_addEditionBeforeAction->setIcon(QIcon{Q(":/icons/16x16/edit-table-insert-row-above.png")});
//   m_addEditionAfterAction->setIcon(QIcon{Q(":/icons/16x16/edit-table-insert-row-below.png")});
//   m_addChapterBeforeAction->setIcon(QIcon{Q(":/icons/16x16/edit-table-insert-row-above.png")});
//   m_addChapterAfterAction->setIcon(QIcon{Q(":/icons/16x16/edit-table-insert-row-below.png")});
//   m_addSubChapterAction->setIcon(QIcon{Q(":/icons/16x16/edit-table-insert-row-under.png")});
//   m_generateSubChaptersAction->setIcon(QIcon{Q(":/icons/16x16/.png")});
//   m_duplicateAction->setIcon(QIcon{Q(":/icons/16x16/tab-duplicate.png")});
//   m_removeElementAction->setIcon(QIcon{Q(":/icons/16x16/list-remove.png")});
//   m_renumberSubChaptersAction->setIcon(QIcon{Q(":/icons/16x16/format-list-ordered.png")});
//   m_massModificationAction->setIcon(QIcon{Q(":/icons/16x16/tools-wizard.png")});

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

//   m_expandAllAction->setText(QY("&Expand all"));
//   m_collapseAllAction->setText(QY("&Collapse all"));

  setupToolTips();

//   m_chapterModel->retranslateUi();
//   m_nameModel->retranslateUi();
// 
//   resizeChapterColumnsToContents();

  emit titleChanged();
}

void
Tab::setupToolTips() {
//   Util::setToolTip(ui->elements, QY("Right-click for actions for editions and chapters"));
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
//   m_analyzer.reset();
//   m_nameModel->reset();
//   m_chapterModel->reset();
}

// Tab::LoadResult
// Tab::loadFromMatroskaFile() {
//   m_analyzer = std::make_unique<QtKaxAnalyzer>(this, m_fileName);
// 
//   if (!m_analyzer->set_parse_mode(kax_analyzer_c::parse_mode_fast).set_open_mode(MODE_READ).process()) {
//     auto text = Q("%1 %2")
//       .arg(QY("The file you tried to open (%1) could not be read successfully.").arg(m_fileName))
//       .arg(QY("Possible reasons are: the file is not a Matroska file; the file is write-protected; the file is locked by another process; you do not have permission to access the file."));
//     Util::MessageBox::critical(this)->title(QY("File parsing failed")).text(text).exec();
//     emit removeThisTab();
//     return {};
//   }
// 
//   auto idx = m_analyzer->find(KaxChapters::ClassInfos.GlobalId);
//   if (-1 == idx) {
//     Util::MessageBox::critical(this)->title(QY("File parsing failed")).text(QY("The file you tried to open (%1) does not contain any chapters.").arg(m_fileName)).exec();
//     emit removeThisTab();
//     return {};
//   }
// 
//   auto chapters = m_analyzer->read_element(idx);
//   if (!chapters) {
//     Util::MessageBox::critical(this)->title(QY("File parsing failed")).text(QY("The file you tried to open (%1) could not be read successfully.").arg(m_fileName)).exec();
//     emit removeThisTab();
//   }
// 
//   m_analyzer->close_file();
// 
//   return { std::static_pointer_cast<KaxChapters>(chapters), true };
// }
// 
// Tab::LoadResult
// Tab::checkSimpleFormatForBomAndNonAscii(ChaptersPtr const &chapters) {
//   auto result = Util::checkForBomAndNonAscii(m_fileName);
// 
//   if (   (BO_NONE != result.byteOrder)
//       || !result.containsNonAscii
//       || !Util::Settings::get().m_ceTextFileCharacterSet.isEmpty())
//     return { chapters, false };
// 
//   Util::enableChildren(this, false);
// 
//   m_originalFileName = m_fileName;
//   auto dlg           = new SelectCharacterSetDialog{this, m_originalFileName};
// 
//   connect(dlg, &SelectCharacterSetDialog::characterSetSelected, this, &Tab::reloadSimpleChaptersWithCharacterSet);
//   connect(dlg, &SelectCharacterSetDialog::rejected,             this, &Tab::closeTab);
// 
//   dlg->show();
// 
//   return {};
// }
// 
// Tab::LoadResult
// Tab::loadFromChapterFile() {
//   auto isSimpleFormat = false;
//   auto chapters       = ChaptersPtr{};
//   auto error          = QString{};
// 
//   try {
//     chapters = parse_chapters(to_utf8(m_fileName), 0, -1, 0, "", to_utf8(Util::Settings::get().m_ceTextFileCharacterSet), true, &isSimpleFormat);
// 
//   } catch (mtx::mm_io::exception &ex) {
//     error = Q(ex.what());
// 
//   } catch (mtx::chapter_parser_x &ex) {
//     error = Q(ex.what());
//   }
// 
//   if (!chapters) {
//     auto message = QY("The file you tried to open (%1) is recognized as neither a valid Matroska nor a valid chapter file.").arg(m_fileName);
//     if (!error.isEmpty())
//       message = Q("%1 %2").arg(message).arg(QY("Error message from the parser: %1").arg(error));
// 
//     Util::MessageBox::critical(this)->title(QY("File parsing failed")).text(message).exec();
//     emit removeThisTab();
// 
//   } else if (isSimpleFormat) {
//     auto result = checkSimpleFormatForBomAndNonAscii(chapters);
// 
//     m_fileName.clear();
//     emit titleChanged();
// 
//     return result;
//   }
// 
//   return { chapters, !isSimpleFormat };
// }

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


// bool
// Tab::isSourceMatroska()
//   const {
//   return !!m_analyzer;
// }

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
