#ifndef MTX_MKVTOOLNIX_GUI_EXTRACTION_TOOL_TAB_H
#define MTX_MKVTOOLNIX_GUI_EXTRACTION_TOOL_TAB_H

#include "common/common_pch.h"

#include "mkvtoolnix-gui/extraction_tool/attachment_model.h"
#include "mkvtoolnix-gui/types.h"

class QAction;
class QItemSelection;

namespace libebml {
class EbmlBinary;
};

namespace mtx { namespace gui { namespace ExtractionTool {

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
  //std::unique_ptr<QtKaxAnalyzer> m_analyzer;
  //QDateTime m_fileModificationTime;

  //NameModel *m_nameModel;
  AttachmentModel *m_attachmentModel;

 // QAction *m_expandAllAction, *m_collapseAllAction, *m_addEditionBeforeAction, *m_addEditionAfterAction, *m_addChapterBeforeAction,
  // *m_addChapterAfterAction, *m_addSubChapterAction, *m_removeElementAction;
  //QAction *m_duplicateAction, *m_massModificationAction, *m_generateSubChaptersAction, *m_renumberSubChaptersAction;
  QList<QWidget *> m_nameWidgets;

  QString m_savedState;

public:
  explicit Tab(QWidget *parent, QString const &fileName = QString{});
  ~Tab();

  virtual void retranslateUi();
  virtual QString const &fileName() const;
  virtual QString title() const;
  virtual bool hasChapters() const;
  virtual bool hasBeenModified() const;
  virtual bool areWidgetsEnabled() const;
  virtual bool isSourceMatroska() const;

signals:
  void removeThisTab();
  void titleChanged();

public slots:
  virtual void load();
  virtual void save();

  virtual void closeTab();

protected:
  void setupUi();
  void resetData();
  //void expandCollapseAll(bool expand, QModelIndex const &parentIdx = {});

  LoadResult loadFromMatroskaFile();
  
  void updateFileNameDisplay();

protected:
  void setupToolTips();

  QString currentState() const;
  //static QString formatEbmlBinary(EbmlBinary *binary);
};

}}}

#endif // MTX_MKVTOOLNIX_GUI_EXTRACTION_TOOL_TAB_H
