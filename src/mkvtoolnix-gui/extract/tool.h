#ifndef MTX_MKVTOOLNIX_GUI_EXTRACT_TOOL_H
#define MTX_MKVTOOLNIX_GUI_EXTRACT_TOOL_H

#include "common/common_pch.h"

#include "mkvtoolnix-gui/main_window/tool_base.h"
#include "mkvtoolnix-gui/util/files_drag_drop_handler.h"

class QDragEnterEvent;
class QDropEvent;
class QMenu;

namespace mtx { namespace gui { namespace Extract {

namespace Ui {
class Tool;
}

class Tab;

class Tool : public ToolBase {
  Q_OBJECT;

protected:
  // UI stuff:
  std::unique_ptr<Ui::Tool> ui;
  QMenu *m_extractMenu;
  mtx::gui::Util::FilesDragDropHandler m_filesDDHandler;

public:
  explicit Tool(QWidget *parent, QMenu *extractMenu);
  ~Tool();

  virtual void setupUi() override;
  virtual void setupActions() override;

public slots:
  virtual void retranslateUi();
  virtual void toolShown() override;
  virtual void tabTitleChanged();
  virtual void enableMenuActions();
  virtual void selectFileToOpen();
  virtual bool closeTab(int index);
  virtual void closeCurrentTab();
  virtual void closeSendingTab();
  virtual bool closeAllTabs();
  virtual void reload();
  virtual void openFiles(QStringList const &fileNames);
  virtual void openFilesFromCommandLine(QStringList const &fileNames);
  virtual void setupTabPositions();

protected:
  virtual Tab *appendTab(Tab *tab);
  virtual Tab *currentTab();
  virtual void forEachTab(std::function<void(Tab &)> const &worker);

  virtual void openFile(QString const &fileName);
  virtual void showExtractWidget();

  virtual void dragEnterEvent(QDragEnterEvent *event) override;
  virtual void dropEvent(QDropEvent *event) override;

};

}}}

#endif // MTX_MKVTOOLNIX_GUI_EXTRACT_TOOL_H
