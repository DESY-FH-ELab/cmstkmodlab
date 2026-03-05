/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2025 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "DefoLensComboBox.h"

DefoLensComboBox::DefoLensComboBox(DefoLensModel* model,
				   QWidget *parent)
  : QComboBox(parent),
    model_(model)
{
  // No user editable text
  setEditable(false);

  const QList<QString>& items = model->getNames();

  for (QList<QString>::const_iterator it = items.begin();
       it != items.end();
       ++it) {
    addItem(*it);
  }

  connect(this, SIGNAL(currentIndexChanged(const QString&)),
	  model, SLOT(setCurrentName(const QString&)));

  connect(model, SIGNAL(lensChanged()),
	  this, SLOT(lensChanged()));
}

void DefoLensComboBox::lensChanged()
{
  int idx = findText(model_->getCurrentName());
  if (idx>=0) setCurrentIndex(idx);
}
