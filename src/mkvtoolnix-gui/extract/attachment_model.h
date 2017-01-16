#ifndef MTX_MKVTOOLNIX_GUI_EXTRACT_ATTACHMENTS_MODEL_H
#define MTX_MKVTOOLNIX_GUI_EXTRACT_ATTACHMENTS_MODEL_H

#include "common/common_pch.h"

#include <QIcon>
#include <QList>
#include <QStandardItemModel>

#include <matroska/KaxAttachments.h>

using namespace libmatroska;

using AttachmentPtr = std::shared_ptr<KaxAttachments>;

Q_DECLARE_METATYPE(AttachmentPtr);

namespace mtx { namespace gui { namespace Extract {

class AttachmentModel: public QStandardItemModel {
  Q_OBJECT;

protected:
  QHash<qulonglong, std::shared_ptr<EbmlMaster>> m_elementRegistry;
  qulonglong m_nextElementRegistryIdx{};

  QModelIndex m_selectedIdx;

public:
  AttachmentModel(QObject *parent);
  virtual ~AttachmentModel();
};
 
}}}

#endif // MTX_MKVTOOLNIX_GUI_EXTRACT_ATTACHMENTS_MODEL_H
