#include <regex>
#include <algorithm>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QtWidgets>
#include <QRegularExpression>
#include "explanation-log-window.hpp"
#include "aera-visualizer-scene.hpp"
#include "model-item.hpp"
#include "composite-state-item.hpp"
#include "instantiated-composite-state-item.hpp"

using namespace std;
using namespace core;
using namespace r_code;
using namespace r_exec;

namespace aera_visualizer {

InstantiatedCompositeStateItem::InstantiatedCompositeStateItem(
  QMenu* contextMenu, NewInstantiatedCompositeStateEvent* newInstantiatedCompositeStateEvent,
  ReplicodeObjects& replicodeObjects, AeraVisualizerScene* parent)
  : AeraGraphicsItem(contextMenu, newInstantiatedCompositeStateEvent, replicodeObjects, parent,
      "Instantiated Composite State"),
  newInstantiatedCompositeStateEvent_(newInstantiatedCompositeStateEvent), showState_(HIDE_ICST)
{
  setFactIcstHtml();
  setBoundCstAndMembersHtml();
  setTextItemAndPolygon(makeHtml());
}

void InstantiatedCompositeStateItem::getIcstOrImdlValues(
  string source, std::vector<string>& templateValues, std::vector<string>& exposedValues)
{
  templateValues.clear();
  exposedValues.clear();

  // Debug: Generalize from these formats.
  smatch matches;
  if (regex_search(source, matches, regex("^\\(i\\w+ \\w+ \\|\\[\\] \\[([:\\.\\w]+) ([:\\.\\w]+)\\] \\w+ \\w+\\)$"))) {
    exposedValues.push_back(matches[1].str());
    exposedValues.push_back(matches[2].str());
  }
  else if (regex_search(source, matches, regex("^\\(i\\w+ \\w+ \\[([:\\.\\w]+) ([:\\.\\w]+) ([:\\.\\w]+)\\] \\[([:\\.\\w]+) ([:\\.\\w]+) ([:\\.\\w]+) ([:\\.\\w]+)\\] \\w+ \\w+\\)$"))) {
    templateValues.push_back(matches[1].str());
    templateValues.push_back(matches[2].str());
    templateValues.push_back(matches[3].str());
    exposedValues.push_back(matches[4].str());
    exposedValues.push_back(matches[5].str());
    exposedValues.push_back(matches[6].str());
    exposedValues.push_back(matches[7].str());
  }
}

void InstantiatedCompositeStateItem::setFactIcstHtml()
{
  auto icst = newInstantiatedCompositeStateEvent_->object_->get_reference(0);

  // Strip the ending confidence value and propagation of saliency threshold.
  regex saliencyRegex("\\s+[\\w\\:]+\\)$");
  regex confidenceAndSaliencyRegex("\\s+\\w+\\s+[\\w\\:]+\\)$");
  string factIcstSource = regex_replace(
    replicodeObjects_.getSourceCode(newInstantiatedCompositeStateEvent_->object_), confidenceAndSaliencyRegex, ")");
  string icstSource = regex_replace(replicodeObjects_.getSourceCode(icst), saliencyRegex, ")");

  QString icstLabel(replicodeObjects_.getLabel(icst).c_str());

  // Temporarily use "!down" which doesn't have spaces.
  factIcstHtml_ = QString(factIcstSource.c_str()).replace(icstLabel, "!down");
  factIcstHtml_ += QString("\n      ") + icstSource.c_str();

  factIcstHtml_ = htmlify(factIcstHtml_);
  factIcstHtml_.replace("!down", DownArrowHtml);
  addSourceCodeHtmlLinks(icst, factIcstHtml_);
}

void InstantiatedCompositeStateItem::setBoundCstAndMembersHtml()
{
  auto factIcst = (_Fact*)newInstantiatedCompositeStateEvent_->object_;
  auto icst = factIcst->get_reference(0);
  auto cst = icst->get_reference(0);

  string icstSource = replicodeObjects_.getSourceCode(icst);
  std::vector<string> templateValues;
  std::vector<string> exposedValues;
  getIcstOrImdlValues(icstSource, templateValues, exposedValues);

  string cstSource = CompositeStateItem::simplifyCstSource(replicodeObjects_.getSourceCode(cst));
  // Get just the set of members, which start on the third line and are indented by three spaces.
  string cstMembersSource;
  // TODO: Handle any number of members.
  auto match = QRegularExpression("^.+\\n.+\\n((   .+\\n)+)").match(cstSource.c_str());
  if (match.hasMatch())
    // Strip the ending \n .
    cstMembersSource = match.captured(1).mid(0, match.captured(1).size() - 1).toStdString();

  // Substitute variables.
  int iVariable = -1;
  size_t iTemplateValues = 0;
  size_t iExposedValues = 0;
  while (iTemplateValues < templateValues.size() || iExposedValues < exposedValues.size()) {
    ++iVariable;
    // v0, v1, v2, etc. are split between templateValues and exposedValues.
    string boundValue = (iTemplateValues < templateValues.size() ? 
      templateValues[iTemplateValues] : exposedValues[iExposedValues]);

    string variable = "v" + to_string(iVariable) + ":";
    cstSource = regex_replace(cstSource, regex(variable), variable + boundValue);
    // For boundCstMemberHtml_, don't include the variable.
    cstMembersSource = regex_replace(cstMembersSource, regex(variable), boundValue);

    if (iTemplateValues < templateValues.size())
      // Still looking at templateValues.
      ++iTemplateValues;
    else
      ++iExposedValues;
  }

  // Debug: How to correctly get the timestamp variables.
  auto afterVariable = "v" + to_string(++iVariable) + ":";
  auto beforeVariable = "v" + to_string(++iVariable) + ":";
  cstSource = regex_replace(cstSource, regex(afterVariable), replicodeObjects_.relativeTime(factIcst->get_after()));
  cstSource = regex_replace(cstSource, regex(beforeVariable), replicodeObjects_.relativeTime(factIcst->get_before()));
  cstMembersSource = regex_replace(cstMembersSource, regex(afterVariable), replicodeObjects_.relativeTime(factIcst->get_after()));
  cstMembersSource = regex_replace(cstMembersSource, regex(beforeVariable), replicodeObjects_.relativeTime(factIcst->get_before()));

  boundCstHtml_ = htmlify(cstSource);
  addSourceCodeHtmlLinks(cst, boundCstHtml_);
  ModelItem::highlightVariables(boundCstHtml_);

  boundCstMembersHtml_ = htmlify(cstMembersSource);
  addSourceCodeHtmlLinks(cst, boundCstMembersHtml_);
}

QString InstantiatedCompositeStateItem::makeHtml()
{
  QString html = headerHtml_;
  if (showState_ == HIDE_ICST) {
    html += SelectedRadioButtonHtml + " Hide iCst" +
      " <a href=\"#show-icst\">" + UnselectedRadioButtonHtml + " Show iCst</a><br>";
    html += boundCstMembersHtml_;
  }
  else {
    html += "<a href=\"#hide-icst\">" + UnselectedRadioButtonHtml + " Hide iCst</a>" +
      " " + SelectedRadioButtonHtml + " Show iCst<br>";
    html += factIcstHtml_ + "<br><br>" + boundCstHtml_;
  }

  return html;
}

void InstantiatedCompositeStateItem::textItemLinkActivated(const QString& link)
{
  if (link == "#hide-icst") {
    showState_ = HIDE_ICST;
    setTextItemAndPolygon(makeHtml());
  }
  else if (link == "#show-icst") {
    showState_ = SHOW_ICST;
    setTextItemAndPolygon(makeHtml());
  }
  else
    // For #debug_oid- and others, defer to the base class.
    AeraGraphicsItem::textItemLinkActivated(link);
}

}
