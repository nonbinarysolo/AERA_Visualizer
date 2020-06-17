//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/ AERA VISUALIZER
//_/_/
//_/_/ Copyright(c)2020 Icelandic Institute for Intelligent Machines ses
//_/_/ Vitvelastofnun Islands ses, kt. 571209-0390
//_/_/ Author: Jeffrey Thompson <jeff@iiim.is>
//_/_/
//_/_/ -----------------------------------------------------------------------
//_/_/ Released under Open-Source BSD License with CADIA Clause v 1.0
//_/_/
//_/_/ Redistribution and use in source and binary forms, with or without 
//_/_/ modification, is permitted provided that the following conditions 
//_/_/ are met:
//_/_/
//_/_/ - Redistributions of source code must retain the above copyright 
//_/_/   and collaboration notice, this list of conditions and the 
//_/_/   following disclaimer.
//_/_/
//_/_/ - Redistributions in binary form must reproduce the above copyright 
//_/_/   notice, this list of conditions and the following
//_/_/   disclaimer in the documentation and/or other materials provided 
//_/_/   with the distribution.
//_/_/
//_/_/ - Neither the name of its copyright holders nor the names of its 
//_/_/   contributors may be used to endorse or promote products 
//_/_/   derived from this software without specific prior written permission.
//_/_/
//_/_/ - CADIA Clause v 1.0: The license granted in and to the software under 
//_/_/   this agreement is a limited-use license. The software may not be used
//_/_/   in furtherance of: 
//_/_/   (i) intentionally causing bodily injury or severe emotional distress 
//_/_/   to any person; 
//_/_/   (ii) invading the personal privacy or violating the human rights of 
//_/_/   any person; or 
//_/_/   (iii) committing or preparing for any act of war.
//_/_/
//_/_/ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//_/_/ "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//_/_/ LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
//_/_/ A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//_/_/ OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//_/_/ SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//_/_/ LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
//_/_/ DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
//_/_/ THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//_/_/ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//_/_/ OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//_/_/
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

#ifndef EXPLANATION_LOG_WINDOW_HPP
#define EXPLANATION_LOG_WINDOW_HPP

#include <QTextBrowser>
#include "aera-visualizer-window.hpp"

namespace aera_visualizer {
/**
 * ExplanationLogWindow extends AeraVisulizerWindowBase to present the player
 * control panel and the explanation log.
 */
class ExplanationLogWindow : public AeraVisulizerWindowBase
{
  Q_OBJECT

public:
  /**
   * Create an ExplanationLogWindow.
   * \param parent The main parent window for this window.
   * \param replicodeObjects The ReplicodeObjects used to find objects.
   */
  ExplanationLogWindow(AeraVisulizerWindow* mainWindow, ReplicodeObjects& replicodeObjects);

  void appendHtml(const QString& html)
  {
    // TODO: Does QTextBrowser have an actual append operation?
    html_ += html;
    textBrowser_->setText(html_);
  }

  void appendHtml(const std::string& html) { appendHtml(QString(html.c_str())); }

protected:
  // TODO: Implement.
  bool haveMoreEvents() override { return false; }
  // TODO: Implement.
  core::Timestamp stepEvent(core::Timestamp maximumTime) override { return r_code::Utils_MaxTime; }
  // TODO: Implement.
  core::Timestamp unstepEvent(core::Timestamp minimumTime) override { return r_code::Utils_MaxTime; }

private slots:
  void textBrowserAnchorClicked(const QUrl& url);

private:
  /**
   * ExplanationLogWindow::TextBrowser extends QTextBrowser so that we can override its
   * mouseMoveEvent.
   */
  class TextBrowser : public QTextBrowser {
  public:
    TextBrowser(ExplanationLogWindow* parent)
      : QTextBrowser(parent), parent_(parent)
    {}

    ExplanationLogWindow* parent_;

  protected:
    void mouseMoveEvent(QMouseEvent* event) override;
  };
  friend TextBrowser;

  AeraVisulizerWindow* parent_;
  ReplicodeObjects& replicodeObjects_;
  // TODO: We should be able to use textBrowser_ to append HTML.
  QString html_;
  TextBrowser* textBrowser_;
};

}

#endif
