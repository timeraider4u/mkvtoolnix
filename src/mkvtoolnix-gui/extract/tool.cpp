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
#include "mkvtoolnix-gui/forms/extract/tool.h"
#include "mkvtoolnix-gui/forms/main_window/main_window.h"
#include "mkvtoolnix-gui/extract/tab.h"
#include "mkvtoolnix-gui/extract/tool.h"
#include "mkvtoolnix-gui/main_window/main_window.h"
#include "mkvtoolnix-gui/util/file_dialog.h"
#include "mkvtoolnix-gui/util/message_box.h"
#include "mkvtoolnix-gui/util/settings.h"
#include "mkvtoolnix-gui/util/string.h"
#include "mkvtoolnix-gui/util/widget.h"

namespace mtx { namespace gui { namespace Extract {

using namespace mtx::gui;

Tool::Tool(QWidget *parent,
           QMenu *extractMenu)
  : ToolBase{parent}
  , ui{new Ui::Tool}
  , m_extractMenu{extractMenu}
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

  showExtractWidget();

  retranslateUi();

  connect(ui->editors, &QTabWidget::tabCloseRequested, this, &Tool::closeTab);
}

void
Tool::setupActions() {
  auto mw   = MainWindow::get();
  auto mwUi = MainWindow::getUi();

  connect(mwUi->actionExtractOpen,               &QAction::triggered,             this, &Tool::selectFileToOpen);
  connect(mwUi->actionExtractReload,             &QAction::triggered,             this, &Tool::reload);
  connect(mwUi->actionExtractClose,              &QAction::triggered,             this, &Tool::closeCurrentTab);
  connect(mwUi->actionExtractCloseAll,           &QAction::triggered,             this, &Tool::closeAllTabs);

  connect(ui->openFileButton,                          &QPushButton::clicked,           this, &Tool::selectFileToOpen);

  connect(m_extractMenu,                         &QMenu::aboutToShow,             this, &Tool::enableMenuActions);
  connect(mw,                                          &MainWindow::preferencesChanged, this, &Tool::setupTabPositions);
  connect(mw,                                          &MainWindow::preferencesChanged, this, &Tool::retranslateUi);

  connect(App::instance(),                             &App::editingChaptersRequested,  this, &Tool::openFilesFromCommandLine);
}

void
Tool::showExtractWidget() {
  ui->stack->setCurrentWidget(ui->editors->count() ? ui->editorsPage : ui->noFilesPage);
  enableMenuActions();
}

void
Tool::enableMenuActions() {
  auto mwUi        = MainWindow::getUi();
  auto tab         = currentTab();
  auto hasFileName = tab && !tab->fileName().isEmpty();
  auto tabEnabled  = tab && tab->areWidgetsEnabled();

  mwUi->actionExtractReload->setEnabled(        tabEnabled                                 && hasFileName);
  mwUi->actionExtractClose->setEnabled(         !!tab);
  mwUi->menuExtractAll->setEnabled(             !!tab);
  mwUi->actionExtractCloseAll->setEnabled(      !!tab);
}

void
Tool::toolShown() {
  MainWindow::get()->showTheseMenusOnly({ m_extractMenu });
  showExtractWidget();
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

  ui->editors->addTab(tab, Util::escape(tab->title(), Util::EscapeKeyboardShortcuts));

  showExtractWidget();

  ui->editors->setCurrentIndex(ui->editors->count() - 1);

  return tab;
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
  auto fileNames = Util::getOpenFileNames(this, QY("Open files in extractor"), Util::Settings::get().lastOpenDirPath(),
                                          QY("Supported file types")           + Q(" (*.mkv *.mka *.mks *.mk3d);;") +
                                          QY("Matroska files")                 + Q(" (*.mkv *.mka *.mks *.mk3d);;") +
                                          QY("All files")                      + Q(" (*)"));
  if (fileNames.isEmpty())
    return;

  MainWindow::get()->setStatusBarMessage(QNY("Opening %1 file in the extractor…", "Opening %1 files in the extractor…", fileNames.count()).arg(fileNames.count()));

  for (auto const &fileName : fileNames)
    openFile(fileName);
}

void
Tool::reload() {
  auto tab = currentTab();
  if (!tab || tab->fileName().isEmpty())
    return;

  if (Util::Settings::get().m_warnBeforeClosingModifiedTabs && tab->hasBeenModified()) {
    auto answer = Util::MessageBox::question(this)
      ->title(QY("Reload modified file"))
      .text(QY("The file \"%1\" has been modified. Do you really want to reload it? All changes will be lost.").arg(tab->title()))
      .buttonLabel(QMessageBox::Yes, QY("&Reload file"))
      .buttonLabel(QMessageBox::No,  QY("Cancel"))
      .exec();
    if (answer != QMessageBox::Yes)
      return;
  }

  tab->load();
}

bool
Tool::closeTab(int index) {
  if ((0  > index) || (ui->editors->count() <= index))
    return false;

  auto tab = static_cast<Tab *>(ui->editors->widget(index));

  if (Util::Settings::get().m_warnBeforeClosingModifiedTabs && tab->hasBeenModified()) {
    MainWindow::get()->switchToTool(this);
    ui->editors->setCurrentIndex(index);
    auto answer = Util::MessageBox::question(this)
      ->title(QY("Close modified file"))
      .text(QY("The file \"%1\" has been modified. Do you really want to close? All changes will be lost.").arg(tab->title()))
      .buttonLabel(QMessageBox::Yes, QY("&Close file"))
      .buttonLabel(QMessageBox::No,  QY("Cancel"))
      .exec();
    if (answer != QMessageBox::Yes)
      return false;
  }

  ui->editors->removeTab(index);
  delete tab;

  showExtractWidget();

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
