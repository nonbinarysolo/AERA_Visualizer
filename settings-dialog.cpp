//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//_/_/
//_/_/ AERA Visualizer
//_/_/ 
//_/_/ Copyright (c) 2018-2023 Jeff Thompson
//_/_/ Copyright (c) 2018-2023 Kristinn R. Thorisson
//_/_/ Copyright (c) 2018-2023 Icelandic Institute for Intelligent Machines
//_/_/ Copyright (c) 2023 Chloe Schaff
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

#include "settings-dialog.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>


//using namespace std;

/* 
* TO DO
* - Add labels and dividers between sections
* - Add responders
* - Add XML export to settings.h or xml_parser in CoreLibrary
* - See bottom of settings.xml for valid ranges and units
* - Left justify prompts, right justify inputs
* - Set Delete on Close flag
*/

namespace aera_visualizer {

  SettingsDialog::SettingsDialog(AeraVisualizerWindow* parent) : QDialog(parent) {
    // Store this for reference
    parentWindow_ = parent;

    // Set up the window
    setWindowTitle("Configure AERA instance");
    setWindowIcon(QIcon(":/images/app.ico"));
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Set up the major layouts
    QVBoxLayout* dialogLayaout = new QVBoxLayout(this);     // All the settings + Apply/Cancel/Reset buttons
    QHBoxLayout* settingsLayout = new QHBoxLayout(this);    // Both halves of the settings menu
    QVBoxLayout* leftSideLayout = new QVBoxLayout(this);    // Left side of the settings menu
    QVBoxLayout* rightSideLayout = new QVBoxLayout(this);   // Right side of the settings menu
    settingsLayout->addLayout(leftSideLayout);
    settingsLayout->addSpacing(50);
    settingsLayout->addLayout(rightSideLayout);
    dialogLayaout->addLayout(settingsLayout);

    // Create the controls
    QPushButton* applySettingsButton = new QPushButton("Apply", this);
    QPushButton* cancelButton = new QPushButton("Cancel", this);
    QPushButton* resetButton = new QPushButton("Reset to Defaults", this);

    QHBoxLayout* buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->addWidget(resetButton);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(applySettingsButton);

    dialogLayaout->addLayout(buttonsLayout);



    // I/O Devices and Load segments //
    QLabel* ioHeader = new QLabel("I/O & Load", this);
    ioHeader->setStyleSheet("background-color: rgba(0,0,0, 24);");
    QGridLayout* ioLoadLayout = new QGridLayout(this);
    leftSideLayout->addWidget(ioHeader);
    leftSideLayout->addLayout(ioLoadLayout);

    // Put together the IO Device prompt
    QLabel* ioDevicePrompt = new QLabel("I/O Device:", this);
    QComboBox* ioDeviceSelection = new QComboBox(this);
    ioDeviceSelection->addItems({ "Test Mem", "TCP I/O Device", "Video Screen" });

    // Add it to the layout
    ioLoadLayout->addWidget(ioDevicePrompt, 0, 0);
    ioLoadLayout->addWidget(ioDeviceSelection, 0, 1);

    // Put together the user operators prompt
    QLabel* userOperatorsPrompt = new QLabel("User operators file:", this);
    QLabel* userOperatorsEntry = new QLabel("FILE SELECT", this);

    // Add it to the layout
    ioLoadLayout->addWidget(userOperatorsPrompt, 1, 0);
    ioLoadLayout->addWidget(userOperatorsEntry, 1, 1);

    // Put together the user classes prompt
    QLabel* userClassesPrompt = new QLabel("User classes file:", this);
    QLabel* userClassesEntry = new QLabel("FILE SELECT", this);

    // Add it to the layout
    ioLoadLayout->addWidget(userClassesPrompt, 2, 0);
    ioLoadLayout->addWidget(userClassesEntry, 2, 1);


    // Init Segment //
    QLabel* initHeader = new QLabel("Init", this);
    initHeader->setStyleSheet("background-color: rgba(0,0,0, 24);");
    QGridLayout* initLayout = new QGridLayout(this);
    leftSideLayout->addWidget(initHeader);
    leftSideLayout->addLayout(initLayout);

    // Put together the base period prompt
    QLabel* basePeriodPrompt = new QLabel("Base period:", this);
    QLabel* basePeriodEntry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    initLayout->addWidget(basePeriodPrompt, 0, 0);
    initLayout->addWidget(basePeriodEntry, 0, 1);

    // Put together the reduction cores prompt
    QLabel* rdxCoresPrompt = new QLabel("Reduction cores:", this);
    QLabel* rdxCoresEntry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    initLayout->addWidget(rdxCoresPrompt, 1, 0);
    initLayout->addWidget(rdxCoresEntry, 1, 1);

    // Put together the time cores prompt
    QLabel* timeCoresPrompt = new QLabel("Time cores:", this);
    QLabel* timeCoresEntry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    initLayout->addWidget(timeCoresPrompt, 2, 0);
    initLayout->addWidget(timeCoresEntry, 2, 1);


    // System segment //
    QLabel* systemHeader = new QLabel("System", this);
    systemHeader->setStyleSheet("background-color: rgba(0,0,0, 24);");
    QGridLayout* systemLayout = new QGridLayout(this);
    leftSideLayout->addWidget(systemHeader);
    leftSideLayout->addLayout(systemLayout);
    
    // Put together the mdl_inertia_sr_thr prompt
    QLabel* mdl_inertia_sr_thr_Prompt = new QLabel("mdl_inertia_sr_thr:", this);
    QLabel* mdl_inertia_sr_thr_Entry = new QLabel("FLOAT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(mdl_inertia_sr_thr_Prompt, 0, 0);
    systemLayout->addWidget(mdl_inertia_sr_thr_Entry, 0, 1);

    // Put together the mdl_inertia_cnt_thr prompt
    QLabel* mdl_inertia_cnt_thr_Prompt = new QLabel("mdl_inertia_cnt_thr:", this);
    QLabel* mdl_inertia_cnt_thr_Entry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(mdl_inertia_cnt_thr_Prompt, 1, 0);
    systemLayout->addWidget(mdl_inertia_cnt_thr_Entry, 1, 1);

    // Put together the tpx_dsr_thr prompt
    QLabel* tpx_dsr_thr_Prompt = new QLabel("tpx_dsr_thr:", this);
    QLabel* tpx_dsr_thr_Entry = new QLabel("FLOAT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(tpx_dsr_thr_Prompt, 2, 0);
    systemLayout->addWidget(tpx_dsr_thr_Entry, 2, 1);

    // Put together the min_sim_time_horizon prompt
    QLabel* min_sim_time_horizon_Prompt = new QLabel("min_sim_time_horizon:", this);
    QLabel* min_sim_time_horizon_Entry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(min_sim_time_horizon_Prompt, 3, 0);
    systemLayout->addWidget(min_sim_time_horizon_Entry, 3, 1);

    // Put together the max_sim_time_horizon prompt
    QLabel* max_sim_time_horizon_Prompt = new QLabel("max_sim_time_horizon:", this);
    QLabel* max_sim_time_horizon_Entry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(max_sim_time_horizon_Prompt, 4, 0);
    systemLayout->addWidget(max_sim_time_horizon_Entry, 4, 1);

    // Put together the sim_time_horizon_factor prompt
    QLabel* sim_time_horizon_factor_Prompt = new QLabel("sim_time_horizon_factor:", this);
    QLabel* sim_time_horizon_factor_Entry = new QLabel("FLOAT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(sim_time_horizon_factor_Prompt, 5, 0);
    systemLayout->addWidget(sim_time_horizon_factor_Entry, 5, 1);

    // Put together the tpx_time_horizon prompt
    QLabel* tpx_time_horizon_Prompt = new QLabel("tpx_time_horizon:", this);
    QLabel* tpx_time_horizon_Entry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(tpx_time_horizon_Prompt, 6, 0);
    systemLayout->addWidget(tpx_time_horizon_Entry, 6, 1);

    // Put together the perf_sampling_period prompt
    QLabel* perf_sampling_period_Prompt = new QLabel("perf_sampling_period:", this);
    QLabel* perf_sampling_period_Entry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(perf_sampling_period_Prompt, 7, 0);
    systemLayout->addWidget(perf_sampling_period_Entry, 7, 1);

    // Put together the float_tolerance prompt
    QLabel* float_tolerance_Prompt = new QLabel("float_tolerance:", this);
    QLabel* float_tolerance_Entry = new QLabel("FLOAT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(float_tolerance_Prompt, 8, 0);
    systemLayout->addWidget(float_tolerance_Entry, 8, 1);

    // Put together the time_tolerance prompt
    QLabel* time_tolerance_Prompt = new QLabel("time_tolerance:", this);
    QLabel* time_tolerance_Entry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(time_tolerance_Prompt, 9, 0);
    systemLayout->addWidget(time_tolerance_Entry, 9, 1);

    // Put together the primary_thz prompt
    QLabel* primary_thz_Prompt = new QLabel("primary_thz:", this);
    QLabel* primary_thz_Entry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(primary_thz_Prompt, 10, 0);
    systemLayout->addWidget(primary_thz_Entry, 10, 1);

    // Put together the secondary_thz prompt
    QLabel* secondary_thz_Prompt = new QLabel("secondary_thz:", this);
    QLabel* secondary_thz_Entry = new QLabel("INT SPINBOX", this);

    // Add it to the layout
    systemLayout->addWidget(secondary_thz_Prompt, 11, 0);
    systemLayout->addWidget(secondary_thz_Entry, 11, 1);

    // Run //
    QLabel* runHeader = new QLabel("Run", this);
    runHeader->setStyleSheet("background-color: rgba(0,0,0, 24);");
    QGridLayout* runLayout = new QGridLayout(this);
    leftSideLayout->addWidget(runHeader);
    leftSideLayout->addLayout(runLayout);

    // Put together the VARIABLE prompt
    QLabel* run_time_Prompt = new QLabel("run_time:", this);
    QLabel* run_time_Entry = new QLabel("INT SPINBOX", this);
    runLayout->addWidget(run_time_Prompt, 0, 0);
    runLayout->addWidget(run_time_Entry, 0, 1);

    // Put together the probe_level prompt
    QLabel* probe_level_Prompt = new QLabel("probe_level:", this);
    QLabel* probe_level_Entry = new QLabel("INT SPINBOX", this);
    runLayout->addWidget(probe_level_Prompt, 2, 0);
    runLayout->addWidget(probe_level_Entry, 2, 1);

    // Add in a spacer
    QLabel* spacer = new QLabel("", this);
    leftSideLayout->addWidget(spacer);
    
    
    // Debug segment //
    QLabel* debugHeader = new QLabel("Debug", this);
    debugHeader->setStyleSheet("background-color: rgba(0,0,0, 24);");
    QGridLayout* debugLayout = new QGridLayout(this);
    rightSideLayout->addWidget(debugHeader);
    rightSideLayout->addLayout(debugLayout);

    // Put together the debug prompt
    QLabel* debug_Prompt = new QLabel("debug:", this);
    QLabel* debug_Entry = new QLabel("CHECKBOX", this);
    debugLayout->addWidget(debug_Prompt, 0, 0);
    debugLayout->addWidget(debug_Entry, 0, 1);

    // Put together the debug_windows prompt
    QLabel* debug_windows_Prompt = new QLabel("debug_windows:", this);
    QLabel* debug_windows_Entry = new QLabel("INT SPINBOX", this);
    debugLayout->addWidget(debug_windows_Prompt, 1, 0);
    debugLayout->addWidget(debug_windows_Entry, 1, 1);

    // Put together the runtime_output_file_path prompt
    QLabel* runtime_output_file_path_Prompt = new QLabel("runtime_output_file_path:", this);
    QLabel* runtime_output_file_path_Entry = new QLabel("FILE SELECT", this);
    debugLayout->addWidget(runtime_output_file_path_Prompt, 2, 0);
    debugLayout->addWidget(runtime_output_file_path_Entry, 2, 1);

    // Put together the trace_levels prompt
    QLabel* trace_levels_Prompt = new QLabel("trace_levels:", this);
    QLabel* trace_levels_Entry = new QLabel("INT SPINBOX", this);
    debugLayout->addWidget(trace_levels_Prompt, 3, 0);
    debugLayout->addWidget(trace_levels_Entry, 3, 1);


    // Resilience //
    QLabel* resilienceHeader = new QLabel("Resilience", this);
    resilienceHeader->setStyleSheet("background-color: rgba(0,0,0, 24);");
    QGridLayout* resilienceLayout = new QGridLayout(this);
    rightSideLayout->addWidget(resilienceHeader);
    rightSideLayout->addLayout(resilienceLayout);

    // Put together the ntf_mk_resilience prompt
    QLabel* ntf_mk_resilience_Prompt = new QLabel("ntf_mk_resilience:", this);
    QLabel* ntf_mk_resilience_Entry = new QLabel("INT SPINBOX", this);
    resilienceLayout->addWidget(ntf_mk_resilience_Prompt, 0, 0);
    resilienceLayout->addWidget(ntf_mk_resilience_Entry, 0, 1);

    // Put together the goal_pred_success_resilience prompt
    QLabel* goal_pred_success_resilience_Prompt = new QLabel("goal_pred_success_resilience:", this);
    QLabel* goal_pred_success_resilience_Entry = new QLabel("INT SPINBOX", this);
    resilienceLayout->addWidget(goal_pred_success_resilience_Prompt, 1, 0);
    resilienceLayout->addWidget(goal_pred_success_resilience_Entry, 1, 1);
    

    // Objects //
    QLabel* objectsHeader = new QLabel("Objects", this);
    objectsHeader->setStyleSheet("background-color: rgba(0,0,0, 24);");
    QGridLayout* objectLayout = new QGridLayout(this);
    rightSideLayout->addWidget(objectsHeader);
    rightSideLayout->addLayout(objectLayout);

    // Put together the get_objects prompt
    QLabel* get_objects_Prompt = new QLabel("get_objects:", this);
    QLabel* get_objects_Entry = new QLabel("CHECKBOX", this);
    get_objects_Entry->setEnabled(false); // The visualizer needs this
    objectLayout->addWidget(get_objects_Prompt, 0, 0);
    objectLayout->addWidget(get_objects_Entry, 0, 1);

    // Put together the keep_invalidated_objects prompt
    QLabel* keep_invalidated_objects_Prompt = new QLabel("keep_invalidated_objects:", this);
    QLabel* keep_invalidated_objects_Entry = new QLabel("CHECKBOX", this);
    keep_invalidated_objects_Entry->setEnabled(false); // The visualizer needs this
    objectLayout->addWidget(keep_invalidated_objects_Prompt, 1, 0);
    objectLayout->addWidget(keep_invalidated_objects_Entry, 1, 1);

    // Put together the decompile_objects prompt
    QLabel* decompile_objects_Prompt = new QLabel("decompile_objects:", this);
    QLabel* decompile_objects_Entry = new QLabel("CHECKBOX", this);
    keep_invalidated_objects_Entry->setEnabled(false); // The visualizer needs this
    objectLayout->addWidget(decompile_objects_Prompt, 2, 0);
    objectLayout->addWidget(decompile_objects_Entry, 2, 1);

    // Put together the decompile_to_file prompt
    QLabel* decompile_to_file_Prompt = new QLabel("decompile_to_file:", this);
    QLabel* decompile_to_file_Entry = new QLabel("CHECKBOX", this);
    objectLayout->addWidget(decompile_to_file_Prompt, 3, 0);
    objectLayout->addWidget(decompile_to_file_Entry, 3, 1);

    // Put together the decompilation_file_path prompt
    QLabel* decompilation_file_path_Prompt = new QLabel("decompilation_file_path:", this);
    QLabel* decompilation_file_path_Entry = new QLabel("FILE SELECT", this);
    objectLayout->addWidget(decompilation_file_path_Prompt, 4, 0);
    objectLayout->addWidget(decompilation_file_path_Entry, 4, 1);

    // Put together the ignore_named_objects prompt
    QLabel* ignore_named_objects_Prompt = new QLabel("ignore_named_objects:", this);
    QLabel* ignore_named_objects_Entry = new QLabel("CHECKB", this);
    objectLayout->addWidget(ignore_named_objects_Prompt, 5, 0);
    objectLayout->addWidget(ignore_named_objects_Entry, 5, 1);

    // Put together the write_objects prompt
    QLabel* write_objects_Prompt = new QLabel("write_objects:", this);
    QLabel* write_objects_Entry = new QLabel("CHECKBOX", this);
    objectLayout->addWidget(write_objects_Prompt, 6, 0);
    objectLayout->addWidget(write_objects_Entry, 6, 1);

    // Put together the objects_path prompt
    QLabel* objects_path_Prompt = new QLabel("objects_path:", this);
    QLabel* objects_path_Entry = new QLabel("FILE SELECT", this);
    objectLayout->addWidget(objects_path_Prompt, 7, 0);
    objectLayout->addWidget(objects_path_Entry, 7, 1);

    // Put together the test_objects prompt
    QLabel* test_objects_Prompt = new QLabel("test_objects:", this);
    QLabel* test_objects_Entry = new QLabel("CHECKBOX", this);
    objectLayout->addWidget(test_objects_Prompt, 8, 0);
    objectLayout->addWidget(test_objects_Entry, 8, 1);

    
    // Models //
    QLabel* modelsHeader = new QLabel("Models", this);
    modelsHeader->setStyleSheet("background-color: rgba(0,0,0, 24);");
    QGridLayout* modelsLayout = new QGridLayout(this);
    rightSideLayout->addWidget(modelsHeader);
    rightSideLayout->addLayout(modelsLayout);

    // Put together the get_models prompt
    QLabel* get_models_Prompt = new QLabel("get_models:", this);
    QLabel* get_models_Entry = new QLabel("CHECKBOX", this);
    modelsLayout->addWidget(get_models_Prompt, 0, 0);
    modelsLayout->addWidget(get_models_Entry, 0, 1);

    // Put together the decompile_models prompt
    QLabel* decompile_models_Prompt = new QLabel("decompile_models:", this);
    QLabel* decompile_models_Entry = new QLabel("CHECKBOX", this);
    modelsLayout->addWidget(decompile_models_Prompt, 1, 0);
    modelsLayout->addWidget(decompile_models_Entry, 1, 1);

    // Put together the ignore_named_models prompt
    QLabel* ignore_named_models_Prompt = new QLabel("ignore_named_models:", this);
    QLabel* ignore_named_models_Entry = new QLabel("CHECKBOX", this);
    modelsLayout->addWidget(ignore_named_models_Prompt, 2, 0);
    modelsLayout->addWidget(ignore_named_models_Entry, 2, 1);

    // Put together the write_models prompt
    QLabel* _Prompt = new QLabel("write_models:", this);
    QLabel* _Entry = new QLabel("CHECKBOX", this);
    modelsLayout->addWidget(_Prompt, 3, 0);
    modelsLayout->addWidget(_Entry, 3, 1);

    // Put together the models_path prompt
    QLabel* models_path_Prompt = new QLabel("models_path:", this);
    QLabel* models_path_Entry = new QLabel("FILE SELECT", this);
    modelsLayout->addWidget(models_path_Prompt, 4, 0);
    modelsLayout->addWidget(models_path_Entry, 4, 1);

    // Put together the test_models prompt
    QLabel* test_models_Prompt = new QLabel("test_models:", this);
    QLabel* test_models_Entry = new QLabel("CHECKBOX", this);
    modelsLayout->addWidget(test_models_Prompt, 5, 0);
    modelsLayout->addWidget(test_models_Entry, 5, 1);
  }
  
  void SettingsDialog::applyChanges() {
    // Save the current settings to disk
  }

  void SettingsDialog::cancelChanges() {
    // Save the current settings to disk
    // Can probably just redirect to close()
  }

  void SettingsDialog::resetToDefaults() {
    // Save the current settings to disk
  }
}
