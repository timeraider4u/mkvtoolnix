#include "common/common_pch.h"

#include <QDebug>

#include "common/ebml.h"
#include "common/logger.h"
#include "common/qt.h"
#include "common/strings/formatting.h"
#include "mkvtoolnix-gui/extraction_tool/attachment_model.h"
#include "mkvtoolnix-gui/util/model.h"

namespace mtx { namespace gui { namespace ExtractionTool {

using namespace mtx::gui;

static auto const s_numColumns = 4;

AttachmentModel::AttachmentModel(QObject *parent)
  : QStandardItemModel{parent}
{
}

AttachmentModel::~AttachmentModel() {
}

}}}
