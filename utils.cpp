#include "utils.h"

QRectF halfRect(const QRectF &rect)
{
    auto w = rect.width() / 4;
    auto h = rect.height() / 4;
    return QRectF(rect.x() + w, rect.y() + h, 2 * w, 2 * h);
}
