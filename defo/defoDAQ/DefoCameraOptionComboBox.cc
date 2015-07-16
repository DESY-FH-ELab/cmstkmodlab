#include "DefoCameraOptionComboBox.h"

DefoCameraOptionComboBox::DefoCameraOptionComboBox(
    DefoCameraModel* model
  , const DefoCameraModel::Option& option
  , QWidget* parent
) :
    QComboBox(parent)
  , model_(model)
  , option_(option)
{

  connect(
          model_
        , SIGNAL(deviceOptionChanged(DefoCameraModel::Option,int))
        , this
        , SLOT(deviceOptionChanged(DefoCameraModel::Option,int))
  );

  connect(
        model_
        , SIGNAL(deviceStateChanged(State))
        , this
        , SLOT(deviceStateChanged(State))
  );

  connect(model_, SIGNAL(controlStateChanged(bool)),
	  this, SLOT(controlStateChanged(bool)));

  connect(
          this
        , SIGNAL(currentIndexChanged(int))
        , this
        , SLOT(selectionChanged(int))
  );

  deviceStateChanged(model_->getDeviceState());

}

/// Propagates device state and options to GUI
void DefoCameraOptionComboBox::deviceStateChanged(State newState)
{

  bool enabled = (newState == READY);
  setEnabled(enabled);

  if ( !enabled ) {
    // not enabled, makes no sense to have options
    clear();
  }
  else {
    // get list of options
    DefoCameraModel::OptionList list = model_->getOptions(option_);
    // add them to the combo box
    for (
          DefoCameraModel::OptionList::const_iterator it = list.begin()
        ; it < list.end()
        ; ++it
    ) {
//      std::cout << *it << std::endl;
      addItem(QString(it->c_str()));
    }
  }

}

void DefoCameraOptionComboBox::controlStateChanged(bool enabled)
{
  if (enabled && model_->getDeviceState()==READY) {
    setEnabled(true);
  } else {
    setEnabled(false);
  }
}

/// Propagates GUI setting to model
void DefoCameraOptionComboBox::selectionChanged(int index) {
//  std::cout << "option changed (gui) " << index << std::endl;
  if (index > -1)
    model_->setOptionSelection(option_, index);
}

/// Propagates model setting to GUI
void DefoCameraOptionComboBox::deviceOptionChanged(
    DefoCameraModel::Option option
  , int value
) {
  if (option == option_) {
//    std::cout << "option changed (model) " << value << std::endl;
    setCurrentIndex(value);
  }
}
