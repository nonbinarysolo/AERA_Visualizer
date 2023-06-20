//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/ AERA Visualizer
//_/_/ 
//_/_/ Copyright (c) 2018-2023 Jeff Thompson
//_/_/ Copyright (c) 2018-2023 Kristinn R. Thorisson
//_/_/ Copyright (c) 2023 Chloe Schaff
//_/_/ Copyright (c) 2018-2023 Icelandic Institute for Intelligent Machines
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

#include <ctime>
#include "player.hpp"

#include <QtWidgets>

using namespace std;
using namespace std::chrono;
using namespace core;
using namespace r_code;

namespace aera_visualizer {

PlayerView::PlayerView(AeraVisualizerWindow* mainWindow)
: QDockWidget("Timeline", mainWindow),
  mainWindow_(mainWindow),
  isPlaying_(false),
  showRelativeTime_(true),
  playTime_(seconds(0)),
  playTimerId_(0)
{
  // No title bar
  setTitleBarWidget(new QWidget());

  playIcon_ = QIcon(":/images/play.png");
  pauseIcon_ = QIcon(":/images/pause.png");
  playPauseButton_ = new QToolButton(this);
  connect(playPauseButton_, SIGNAL(clicked()), this, SLOT(playPauseButtonClicked()));
  playPauseButton_->setIcon(playIcon_);

  stepBackButton_ = new QToolButton(this);
  stepBackButton_->setIcon(QIcon(":/images/play-step-back.png"));
  connect(stepBackButton_, SIGNAL(clicked()), this, SLOT(stepBackButtonClicked()));
  
  playSlider_ = new QSlider(Qt::Horizontal, this);
  playSlider_->setMaximum(2000);
#if 1
  // Until we implement playSliderValueChanged, disable the slider so the user can't drag it.
  // See https://github.com/IIIM-IS/AERA_Visualizer/issues/3 .
  playSlider_->setEnabled(false);
#else
  connect(playSlider_, SIGNAL(valueChanged(int)), this, SLOT(playSliderValueChanged(int)));
#endif
  
  stepButton_ = new QToolButton(this);
  stepButton_->setIcon(QIcon(":/images/play-step.png"));
  connect(stepButton_, SIGNAL(clicked()), this, SLOT(stepButtonClicked()));

  playTimeLabel_ = new ClickableLabel("000s:000ms:000us", this);
  playTimeLabel_->setFont(QFont("Courier", 10));
  connect(playTimeLabel_, SIGNAL(clicked()), this, SLOT(playTimeLabelClicked()));
  
  // Put together the player layout
  QHBoxLayout* playerLayout = new QHBoxLayout();
  playerLayout->addWidget(stepBackButton_);
  playerLayout->addWidget(playPauseButton_);
  playerLayout->addWidget(stepButton_);
  playerLayout->addWidget(playSlider_);
  playerLayout->addWidget(playTimeLabel_);

  // Put everything in a container
  QWidget* container = new QWidget();
  container->setObjectName("player_container");
  container->setLayout(playerLayout);
  setWidget(container);
}

void PlayerView::startPlay()
{
  // Update the state
  playPauseButton_->setIcon(pauseIcon_);
  isPlaying_ = true;

  // Start the timer
  if (playTimerId_ == 0)
    playTimerId_ = startTimer(AeraVisualizer_playTimerTick.count());
}

void PlayerView::stopPlay()
{
  // Update the state
  playPauseButton_->setIcon(playIcon_);
  isPlaying_ = false;

  // Stop the timer
  if (playTimerId_ != 0) {
    killTimer(playTimerId_);
    playTimerId_ = 0;
  }
}

void PlayerView::playPauseButtonClicked()
{
  if (isPlaying_)
    stopPlay();
  else
    startPlay();
}

void PlayerView::stepButtonClicked()
{
  mainWindow_->stepButtonClickedImpl();
}

void PlayerView::stepBackButtonClicked()
{
  mainWindow_->stepBackButtonClickedImpl();
}

void PlayerView::playSliderValueChanged(int value)
{
  // TODO: Implement to check if the user moved the slider,
  // stopPlay, update the play time.
}

void PlayerView::playTimeLabelClicked()
{
  showRelativeTime_ = !showRelativeTime_;
  setPlayTime(playTime_);
}

void PlayerView::setPlayTime(Timestamp time)
{
  playTime_ = time;

  uint64 total_us;
  if (showRelativeTime_)
    total_us = duration_cast<microseconds>(time - timeReference_).count();
  else
    total_us = duration_cast<microseconds>(time.time_since_epoch()).count();
  uint64 us = total_us % 1000;
  uint64 ms = total_us / 1000;
  uint64 s = ms / 1000;
  ms = ms % 1000;

  char buffer[100];
  if (showRelativeTime_)
    sprintf(buffer, "%03ds:%03dms:%03dus", (int)s, (int)ms, (int)us);
  else {
    // Get the UTC time.
    time_t gmtTime = s;
    struct tm* t = gmtime(&gmtTime);
    sprintf(buffer, "%04d-%02d-%02d   UTC\n%02d:%02d:%02d:%03d:%03d",
      t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
      t->tm_hour, t->tm_min, t->tm_sec, (int)ms, (int)us);
  }
  playTimeLabel_->setText(buffer);
}

void PlayerView::setSliderToPlayTime()
{
  // If there are no events, fix the slider to the start
  if (mainWindow_->getNumberOfEvents() == 0) {
    playSlider_->setValue(0);
    return;
  }

  // Figure out how far along the we are
  auto maximumEventTime = mainWindow_->getTimeOfLastEvent();
  int value = playSlider_->maximum() *
    ((double)duration_cast<microseconds>(playTime_ - timeReference_).count() /
      duration_cast<microseconds>(maximumEventTime - timeReference_).count());
  playSlider_->setValue(value);
}

void PlayerView::setUIEnabled(bool enabled) {
  playPauseButton_->setEnabled(enabled);
  stepBackButton_->setEnabled(enabled);
  stepButton_->setEnabled(enabled);
  playTimeLabel_->setEnabled(enabled);
}

void PlayerView::timerEvent(QTimerEvent* event)
{
  // TODO: Make sure we don't re-enter.

  if (event->timerId() != playTimerId_)
    // This timer event is not for us.
    return;

  // Anyone who should respond to timer events goes here
  mainWindow_->timerTick();
}

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent, Qt::WindowFlags f)
  : QLabel(text, parent) {}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) { emit clicked(); }

}
