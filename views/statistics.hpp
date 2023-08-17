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

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <QDockWidget>
#include "../aera-visualizer-window.hpp"
#include "../replicode-objects.hpp"

/* This file can be used as a template for creating new views for
* the visualizer. Each view extends a QDockWidget so it can be dragged
* around and reconfigured by the user. Most of its communication should
* be with the mainWindow_, but it's also okay for views to talk directly
* to eachother where relevant. See createDockWidgets() in
* aera-visualizer-window.cpp as an example of how to add this to the window.
* Remember to add a class VIEWNAME statement to main.cpp since views will
* give weird errors if not forwardly-declared.
*/

namespace aera_visualizer {

	/**
 * StatsView extends QDockWidget to allow the user to
 * rearrange it as needed
 */
	class StatisticsView : public QDockWidget
	{
		Q_OBJECT

	public:
		/**
		 * Create a StatsView. This will display various things  like how many
		 * models AERA's learned and so on
		 * \param mainWindow The main parent window for this window.
		 */
		StatisticsView(AeraVisualizerWindow* mainWindow);

		// Used to update the replicodeObjects (if you need access to those)
		void setReplicodeObjects(ReplicodeObjects* replicodeObjects) {
			replicodeObjects_ = replicodeObjects;
		}

	private:
		ReplicodeObjects* replicodeObjects_;

		int seedModels_ = 0;
		int modelsLearned_ = 0;
		int modelsDeleted_ = 0;
		float learningFraction_ = 0.0;

		QLabel* seedModelsLabel_;
		QLabel* modelsLearnedLabel_;
		QLabel* modelsDeletedLabel_;
		QLabel* learningFractionLabel_;
	};
}

#endif
