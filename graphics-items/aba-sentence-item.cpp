//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/ AERA Visualizer
//_/_/ 
//_/_/ Copyright (c) 2022 Jeff Thompson
//_/_/ Copyright (c) 2022 Kristinn R. Thorisson
//_/_/ Copyright (c) 2022 Icelandic Institute for Intelligent Machines
//_/_/ http://www.iiim.is
//_/_/
//_/_/ --- Open-Source BSD License, with CADIA Clause v 1.0 ---
//_/_/
//_/_/ Redistribution and use in source and binary forms, with or without
//_/_/ modification, is permitted provided that the following conditions
//_/_/ are met:
//_/_/ - Redistributions of source code must retain the above copyright
//_/_/   and collaboration notice, this list of conditions and the
//_/_/   following disclaimer.
//_/_/ - Redistributions in binary form must reproduce the above copyright
//_/_/   notice, this list of conditions and the following disclaimer 
//_/_/   in the documentation and/or other materials provided with 
//_/_/   the distribution.
//_/_/
//_/_/ - Neither the name of its copyright holders nor the names of its
//_/_/   contributors may be used to endorse or promote products
//_/_/   derived from this software without specific prior 
//_/_/   written permission.
//_/_/   
//_/_/ - CADIA Clause: The license granted in and to the software 
//_/_/   under this agreement is a limited-use license. 
//_/_/   The software may not be used in furtherance of:
//_/_/    (i)   intentionally causing bodily injury or severe emotional 
//_/_/          distress to any person;
//_/_/    (ii)  invading the personal privacy or violating the human 
//_/_/          rights of any person; or
//_/_/    (iii) committing or preparing for any act of war.
//_/_/
//_/_/ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
//_/_/ CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
//_/_/ INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
//_/_/ MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
//_/_/ DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
//_/_/ CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//_/_/ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
//_/_/ BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
//_/_/ SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
//_/_/ INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
//_/_/ WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
//_/_/ NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//_/_/ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
//_/_/ OF SUCH DAMAGE.
//_/_/ 
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#include <regex>
#include <QMenu>
#include "../explanation-log-window.hpp"
#include "../aera-visualizer-window.hpp"
#include "../submodules/AERA/r_exec/factory.h"
#include "aera-visualizer-scene.hpp"
#include "instantiated-composite-state-item.hpp"
#include "aba-sentence-item.hpp"

using namespace std;
using namespace core;
using namespace r_code;
using namespace r_exec;

namespace aera_visualizer {

AbaSentenceItem::AbaSentenceItem(
  AbaAddSentence* step, ReplicodeObjects& replicodeObjects,
  AeraVisualizerScene* parent)
: ExpandableGoalOrPredItem(step, replicodeObjects, "Step " + RightDoubleArrowHtml, parent, Qt::white),
  step_(step)
{
  if (step->object_->code(0).asOpcode() == Opcodes::Fact &&
      step->object_->get_reference(0)->code(0).asOpcode() == Opcodes::Cmd)
    // Highlight causal events.
    setBrush(QColor(0x00, 0x99, 0x99));
  else {
    if (step->graphId_ > 0) {
      // Get colors for the opponent graph.
      if (step->isAssumption_)
        setBrush(Color_opponent_ms_asm_culprit);
      else
        setBrush(Color_opponent_ms_nonAsm);
    }
    else {
      if (step->isAssumption_)
        setBrush(step->isClaim_ ? Color_proponent_asm_toBeProved : Color_proponent_asm);
      else
        setBrush(step->isClaim_ ? Color_proponent_nonAsm_toBeProved : Color_proponent_nonAsm);
    }
  }

  statusTextItem_ = new QGraphicsTextItem(this);
  statusTextItem_->setPos(boundingRect().left() - 2, boundingRect().top() - 12);
  setStatus(STATUS_UNMARKED);
}

void AbaSentenceItem::textItemLinkActivated(const QString& link)
{
  if (link == "#this") {
    auto menu = new QMenu();
    menu->addAction("Zoom to This", [=]() { parent_->zoomToItem(this); });
    menu->addAction("Focus on This", [=]() { parent_->focusOnItem(this); });
    menu->addAction("Center on This", [=]() { parent_->centerOnItem(this); });

    menu->exec(QCursor::pos() - QPoint(10, 10));
    delete menu;
  }
  if (link == "#expand" || link == "#unexpand") {
    ExpandableGoalOrPredItem::textItemLinkActivated(link);
    statusTextItem_->setPos(boundingRect().left() - 2, boundingRect().top() - 12);
  }
  else
    // For #detail_oid- and others, defer to the base class.
    ExpandableGoalOrPredItem::textItemLinkActivated(link);
}

}