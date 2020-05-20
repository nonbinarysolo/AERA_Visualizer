#include "aera-visualizer-window.hpp"
#include "arrow.hpp"
#include "model-item.hpp"
#include "aera-visualizer-scene.hpp"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QApplication>

using namespace core;
using namespace r_code;

namespace aera_visualizer {

AeraVisualizerScene::AeraVisualizerScene(
    QMenu* itemMenu, ReplicodeObjects& replicodeObjects, AeraVisulizerWindow* parent)
  : QGraphicsScene(parent),
  parent_(parent),
  replicodeObjects_(replicodeObjects),
  didInitialFit_(false),
  borderFlashPen_(Qt::blue, 3),
  noFlashColor_("black"),
  valueUpFlashColor_("green"),
  valueDownFlashColor_("red")
{
  itemMenu_ = itemMenu;
  itemColor_ = Qt::white;
  lineColor_ = Qt::black;
  setBackgroundBrush(QColor(230, 230, 230));
  flashTimerId_ = 0;
}

void AeraVisualizerScene::addAeraGraphicsItem(AeraGraphicsItem* item)
{
  if (!didInitialFit_) {
    didInitialFit_ = true;
    views().at(0)->fitInView(QRectF(0, 0, 1180, 690));
  }

  item->setBrush(itemColor_);

  auto newObjectEvent = item->getAeraEvent();
  if (qIsNaN(newObjectEvent->itemPosition_.x())) {
    // Assign an initial position.
    // TODO: Do this with a grid layout.
    if (newObjectEvent->object_->get_oid() == 60)
      newObjectEvent->itemPosition_ = QPointF(620, 20 + item->boundingRect().height() / 2);
    else if (newObjectEvent->object_->get_debug_oid() == 2061)
      newObjectEvent->itemPosition_ = QPointF(1050, 20 + item->boundingRect().height() / 2);
    else if (newObjectEvent->eventType_ == ProgramReductionNewObjectEvent::EVENT_TYPE)
      newObjectEvent->itemPosition_ = QPointF(
        140 + (newObjectEvent->object_->get_oid() - 49) * 12, 20 + item->boundingRect().height() / 2);
    else if (newObjectEvent->eventType_ == NewInstantiatedCompositeStateEvent::EVENT_TYPE)
      newObjectEvent->itemPosition_ = QPointF(
        190 + (newObjectEvent->object_->get_oid() - 59) * 21, 270);
    else
      newObjectEvent->itemPosition_ = QPointF(
        150 + (newObjectEvent->object_->get_oid() - 52) * 320, 
        640 + item->boundingRect().height() / 2);
  }

  addItem(item);
  item->setPos(newObjectEvent->itemPosition_);
}

void AeraVisualizerScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  mouseScreenPosition_ = event->screenPos();
  QGraphicsScene::mouseMoveEvent(event);
}

void AeraVisualizerScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  if (mouseEvent->button() != Qt::LeftButton)
    return;

  views().at(0)->setDragMode(QGraphicsView::ScrollHandDrag);
  QGraphicsScene::mousePressEvent(mouseEvent);
}

void AeraVisualizerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
  // Reset the drag mode.
  views().at(0)->setDragMode(QGraphicsView::NoDrag);
  QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void AeraVisualizerScene::addArrow(AeraGraphicsItem* startItem, AeraGraphicsItem* endItem)
{
  if (startItem == endItem)
    return;

  Arrow* arrow = new Arrow(startItem, endItem);
  arrow->setColor(lineColor_);
  startItem->addArrow(arrow);
  endItem->addArrow(arrow);
  arrow->setZValue(-1000.0);
  addItem(arrow);
  arrow->updatePosition();
}

AeraGraphicsItem* AeraVisualizerScene::getAeraGraphicsItem(Code* object)
{
  foreach(QGraphicsItem* item, items()) {
    auto graphicsItem = dynamic_cast<AeraGraphicsItem*>(item);
    if (graphicsItem) {
      if (graphicsItem->getAeraEvent()->object_ == object)
        return graphicsItem;
    }
  }

  return 0;
}

void AeraVisualizerScene::scaleViewBy(double factor)
{
  double currentScale = views().at(0)->transform().m11();

  QGraphicsView* view = views().at(0);
  QMatrix oldMatrix = view->matrix();
  view->resetMatrix();
  view->translate(oldMatrix.dx(), oldMatrix.dy());
  view->scale(currentScale *= factor, currentScale *= factor);
}

void AeraVisualizerScene::zoomViewHome()
{
  views().at(0)->fitInView(itemsBoundingRect(), Qt::KeepAspectRatio);
}

void AeraVisualizerScene::zoomToItem(QGraphicsItem* item)
{
  views().at(0)->fitInView(item, Qt::KeepAspectRatio);
}

#if QT_CONFIG(wheelevent)
void AeraVisualizerScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
  // Accept the event to override other behavior.
  event->accept();
  scaleViewBy(pow((double)2, event->delta() / 1000.0));
}
#endif

void AeraVisualizerScene::timerEvent(QTimerEvent* event)
{
  // TODO: Make sure we don't re-enter.

  if (event->timerId() != flashTimerId_)
    // This timer event is not for us.
    return;

  bool isFlashing = false;
  foreach(QGraphicsItem* item, items()) {
    auto aeraGraphicsItem = dynamic_cast<AeraGraphicsItem*>(item);
    if (!aeraGraphicsItem)
      continue;

    if (aeraGraphicsItem->borderFlashCountdown_ > 0) {
      isFlashing = true;

      --aeraGraphicsItem->borderFlashCountdown_;
      if (aeraGraphicsItem->borderFlashCountdown_ % 2 == 1)
        aeraGraphicsItem->setPen(borderFlashPen_);
      else
        aeraGraphicsItem->setPen(lineColor_);
    }

    auto modelItem = dynamic_cast<ModelItem*>(item);
    if (modelItem) {
      if (modelItem->evidenceCountFlashCountdown_ > 0) {
        isFlashing = true;

        --modelItem->evidenceCountFlashCountdown_;
        if (modelItem->evidenceCountFlashCountdown_ % 2 == 1)
          modelItem->setEvidenceCountColor
          (modelItem->evidenceCountIncreased_ ? valueUpFlashColor_ : valueDownFlashColor_);
        else
          modelItem->setEvidenceCountColor(noFlashColor_);
      }

      if (modelItem->successRateFlashCountdown_ > 0) {
        isFlashing = true;

        --modelItem->successRateFlashCountdown_;
        if (modelItem->successRateFlashCountdown_ % 2 == 1)
          modelItem->setSuccessRateColor
          (modelItem->successRateIncreased_ ? valueUpFlashColor_ : valueDownFlashColor_);
        else
          modelItem->setSuccessRateColor(noFlashColor_);
      }
    }
  }

  if (!isFlashing) {
    killTimer(flashTimerId_);
    flashTimerId_ = 0;
  }
}

ExplanationLogWindow* AeraVisualizerScene::getExplanationLogWindow()
{
  return parent_->getExplanationLogWindow();
}

}
