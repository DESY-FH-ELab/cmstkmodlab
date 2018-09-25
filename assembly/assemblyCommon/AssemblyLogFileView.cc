/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <nqlogger.h>
#include <ApplicationConfig.h>

#include <AssemblyLogFileView.h>

#include <QVBoxLayout>
#include <QToolBox>
#include <QFont>

AssemblyLogFileView::AssemblyLogFileView(const QStringList& skimkeys_list, QWidget* parent) : QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout;
  this->setLayout(layout);

  QToolBox* toolbox = new QToolBox;
  layout->addWidget(toolbox);

  // Full Output
  viewers_.insert("*", new AssemblyLogTextViewer);

  toolbox->addItem(viewers_.value("*"), tr("Full Output"));

  // Skimmed Outputs
  for(const auto& key : skimkeys_list)
  {
    viewers_.insert(key, new AssemblyLogTextViewer);

    toolbox->addItem(viewers_.value(key), key);
  }
}

AssemblyLogFileView::~AssemblyLogFileView()
{
}

void AssemblyLogFileView::append_text(const QStringList& qstr_list)
{
  for(const auto& i_qstr : qstr_list)
  {
    for(const auto& j_key : viewers_.keys())
    {
      const bool append = ((j_key == "*") || (i_qstr.contains(j_key)));

      if(append){ viewers_.value(j_key)->appendPlainText(i_qstr); }
    }
  }
}
// ===============================================================================

AssemblyLogTextViewer::AssemblyLogTextViewer()
{
  this->setReadOnly(true);

  // font
  QFont font("Monospace");
  font.setStyleHint(QFont::Monospace);
  this->setFont(font);

  // color
  this->setStyleSheet("color: white; background-color: black");
};

AssemblyLogTextViewer::~AssemblyLogTextViewer()
{
}
// ===============================================================================
