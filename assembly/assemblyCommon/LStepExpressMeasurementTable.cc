#include <nqlogger.h>
#include <iostream>
#include "LStepExpressMeasurementTable.h"

LStepExpressMeasurementTable::LStepExpressMeasurementTable() : QAbstractTableModel()
{
  std::vector<float> column1;
  std::vector<float> column2;
  std::vector<float> column3;
  std::vector<float> column4;
  std::vector<float> column5;
  
  Columns.push_back(column1);
  Columns.push_back(column2);
  Columns.push_back(column3);
  Columns.push_back(column4);
  Columns.push_back(column5);
}
 
int LStepExpressMeasurementTable::rowCount(const QModelIndex& parent) const
{
  return Columns[0].size();
}
 
int LStepExpressMeasurementTable::columnCount(const QModelIndex& parent) const
{
  return Columns.size();
}
 
QVariant LStepExpressMeasurementTable::data(const QModelIndex& index, int role) const
{
  if(role == Qt::DisplayRole)
    {
      return Columns[index.column()][index.row()];
    }
  return QVariant::Invalid;
}
 
void LStepExpressMeasurementTable::update()
{
  reset();
}

void LStepExpressMeasurementTable::insertData(int column_index, std::vector<float> values)
{
    //  NQLog("LStepExpressMeasurementTable ", NQLog::Debug) << "insert data ";

  //overwrite column
  Columns[column_index] = values;


  //  NQLog("LStepExpressMeasurementTable ", NQLog::Debug) << "no. columns = "<< Columns.size() <<" no. rows = "<<Columns[column_index].size();

  QModelIndex in;
  //set the data
  for(unsigned int i = 0; i < values.size(); i++){
    in = createIndex(column_index, i);
    emit dataChanged(in,in);
  }
  //  NQLog("LStepExpressMeasurementTable ", NQLog::Debug) << "ending insert data ";
}

QVariant LStepExpressMeasurementTable::headerData(int section, Qt::Orientation orientation, int role) const
{
 
  if(role == Qt::DisplayRole)
    {
      if(orientation == Qt::Horizontal){
	switch (section){
	case 0: return QString("#");
	case 1: return QString("x-pos");
	case 2: return QString("y-pos");
	case 3: return QString("z-pos");
	case 4: return QString("meas");
	}
      }else{
	std::stringstream ss;
	ss << "V_" << section;
	return QString(ss.str().c_str());
      }
    }
 
  return QVariant::Invalid;
}
