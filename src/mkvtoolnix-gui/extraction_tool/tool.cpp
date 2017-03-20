#include "common/common_pch.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>

#include "common/qt.h"
#include "mkvtoolnix-gui/app.h"
#include "mkvtoolnix-gui/forms/extraction_tool/tool.h"
#include "mkvtoolnix-gui/forms/main_window/main_window.h"
#include "mkvtoolnix-gui/extraction_tool/tab.h"
#include "mkvtoolnix-gui/extraction_tool/tool.h"
#include "mkvtoolnix-gui/main_window/main_window.h"
#include "mkvtoolnix-gui/util/file_dialog.h"
#include "mkvtoolnix-gui/util/message_box.h"
#include "mkvtoolnix-gui/util/settings.h"
#include "mkvtoolnix-gui/util/string.h"
#include "mkvtoolnix-gui/util/widget.h"

namespace mtx { namespace gui { namespace ExtractionTool {

using namespace mtx::gui;

Tool::Tool(QWidget *parent,
           QMenu *extractionToolMenu)
  : ToolBase{parent}
  , ui{new Ui::Tool}
  , m_ExtractionToolMenu{extractionToolMenu}
  , m_filesDDHandler{Util::FilesDragDropHandler::Mode::Remember}
{
  // Setup UI controls.
  ui->setupUi(this);
}

Tool::~Tool() {
}

void
Tool::setupUi() {
  setupTabPositions();

  showExtractionToolWidget();

  retranslateUi();

  connect(ui->editors, &QTabWidget::tabCloseRequested, this, &Tool::closeTab);
}

void
Tool::setupActions() {
  auto mw   = MainWindow::get();
  auto mwUi = MainWindow::getUi();

  connect(mwUi->actionChapterEditorOpen,               &QAction::triggered,             this, &Tool::selectFileToOpen);
  connect(mwUi->actionChapterEditorReload,             &QAction::triggered,             this, &Tool::reload);
  connect(mwUi->actionChapterEditorClose,              &QAction::triggered,             this, &Tool::closeCurrentTab);
  connect(mwUi->actionChapterEditorCloseAll,           &QAction::triggered,             this, &Tool::closeAllTabs);

  connect(ui->openFileButton,                          &QPushButton::clicked,           this, &Tool::selectFileToOpen);

  connect(m_extractionToolMenu,                         &QMenu::aboutToShow,             this, &Tool::enableMenuActions);
  connect(mw,                                          &MainWindow::preferencesChanged, this, &Tool::setupTabPositions);
  connect(mw,                                          &MainWindow::preferencesChanged, this, &Tool::retranslateUi);

  connect(App::instance(),                             &App::editingChaptersRequested,  this, &Tool::openFilesFromCommandLine);
}

void
Tool::showExtractionToolWidget() {
  ui->stack->setCurrentWidget(ui->editors->count() ? ui->editorsPage : ui->noFilesPage);
  enableMenuActions();
}

void
Tool::enableMenuActions() {
  auto mwUi        = MainWindow::getUi();
  auto tab         = currentTab();
  auto hasFileName = tab && !tab->fileName().isEmpty();
  auto hasElements = tab && tab->hasChapters();
  auto tabEnabled  = tab && tab->areWidgetsEnabled();
  auto isSourceKax = tab && tab->isSourceMatroska();

  mwUi->actionExtractionToolReload->setEnabled(        tabEnabled                                 && hasFileName);
  mwUi->actionExtractionToolClose->setEnabled(         !!tab);
  mwUi->actionExtractionToolCloseAll->setEnabled(      !!tab);
}

void
Tool::toolShown() {
  MainWindow::get()->showTheseMenusOnly({ m_extractionToolMenu });
  showExtractionToolWidget();
}

void
Tool::retranslateUi() {
  ui->retranslateUi(this);
  for (auto idx = 0, numTabs = ui->editors->count(); idx < numTabs; ++idx) {
    static_cast<Tab *>(ui->editors->widget(idx))->retranslateUi();
    auto button = Util::tabWidgetCloseTabButton(*ui->editors, idx);
    if (button)
      button->setToolTip(App::translate("CloseButton", "Close Tab"));
  }
}

Tab *
Tool::appendTab(Tab *tab) {
  connect(tab, &Tab::removeThisTab,          this, &Tool::closeSendingTab);
  connect(tab, &Tab::titleChanged,           this, &Tool::tabTitleChanged);
  connect(tab, &Tab::numberOfEntriesChanged, this, &Tool::enableMenuActions);

  ui->editors->addTab(tab, Util::escape(tab->title(), Util::EscapeKeyboardShortcuts));

  showExtractionToolWidget();

  ui->editors->setCurrentIndex(ui->editors->count() - 1);

  return tab;
}

void
Tool::newFile() {
  appendTab(new Tab{this})
    ->newFile();
}

void
Tool::dragEnterEvent(QDragEnterEvent *event) {
  m_filesDDHandler.handle(event, false);
}

void
Tool::dropEvent(QDropEvent *event) {
  if (m_filesDDHandler.handle(event, true))
    openFiles(m_filesDDHandler.fileNames());
}

void
Tool::openFile(QString const &fileName) {
  auto &settings = Util::Settings::get();
  settings.m_lastOpenDir = QFileInfo{fileName}.path();
  settings.save();

  appendTab(new Tab{this, fileName})
   ->load();
}

void
Tool::openFiles(QStringList const &fileNames) {
  for (auto const &fileName : fileNames)
    openFile(fileName);
}

void
Tool::openFilesFromCommandLine(QStringList const &fileNames) {
  MainWindow::get()->switchToTool(this);
  openFiles(fileNames);
}

void
Tool::selectFileToOpen() {
  auto fileNames = Util::getOpenFileNames(this, QY("Open files in extraction tool"), Util::Settings::get().lastOpenDirPath(),
                                          QY("Supported file types")           + Q(" (*.mkv *.mka *.mks *.mk3d );;") +
                                          QY("Matroska files")                 + Q(" (*.mkv *.mka *.mks *.mk3d);;") +
                                          QY("All files")                      + Q(" (*)"));
  if (fileNames.isEmpty())
    return;

  MainWindow::get()->setStatusBarMessage(QNY("Opening %1 file in the extraction tool…", "Opening %1 files in the extraction tool…", fileNames.count()).arg(fileNames.count()));

  for (auto const &fileName : fileNames)
    openFile(fileName);
}

void
Tool::reload() {
  auto tab = currentTab();
  if (!tab || tab->fileName().isEmpty())
    return;

  tab->load();
}

bool
Tool::closeTab(int index) {
  if ((0  > index) || (ui->editors->count() <= index))
    return false;

  auto tab = static_cast<Tab *>(ui->editors->widget(index));

  ui->editors->removeTab(index);
  delete tab;

  showChapterEditorsWidget();

  return true;
}

void
Tool::closeCurrentTab() {
  closeTab(ui->editors->currentIndex());
}

void
Tool::closeSendingTab() {
  auto idx = ui->editors->indexOf(dynamic_cast<Tab *>(sender()));
  if (-1 != idx)
    closeTab(idx);
}

bool
Tool::closeAllTabs() {
  for (auto index = ui->editors->count(); index > 0; --index)
    if (!closeTab(index - 1))
      return false;

  return true;
}

Tab *
Tool::currentTab() {
  return static_cast<Tab *>(ui->editors->widget(ui->editors->currentIndex()));
}

void
Tool::tabTitleChanged() {
  auto tab = dynamic_cast<Tab *>(sender());
  auto idx = ui->editors->indexOf(tab);
  if (tab && (-1 != idx))
    ui->editors->setTabText(idx, Util::escape(tab->title(), Util::EscapeKeyboardShortcuts));
}

void
Tool::setupTabPositions() {
  ui->editors->setTabPosition(Util::Settings::get().m_tabPosition);
}

void
Tool::forEachTab(std::function<void(Tab &)> const &worker) {
  auto currentIndex = ui->editors->currentIndex();

  for (auto index = 0, numTabs = ui->editors->count(); index < numTabs; ++index) {
    ui->editors->setCurrentIndex(index);
    worker(dynamic_cast<Tab &>(*ui->editors->widget(index)));
  }

  ui->editors->setCurrentIndex(currentIndex);
}

}}}
