// utils/MaPSAConverter.cpp

#include "MaPSAConverter.h"

#include <nqlogger.h>
#include <QString>
#include <QRegularExpression>
#include <QMessageBox>
namespace utils {

    QString convert_MaPSA_ID(QString MaPSA_rawID)
    {
        //Check if the raw_ID is empty
        if (MaPSA_rawID.isEmpty()) {
            NQLog("MaPSAIDConverter", NQLog::Fatal) << "MaPSA_ID string is empty";
        }
        //Create a regex for the raw ID read from the barcode  YYYYY_ZZZ_PSP_MAIN(L|R)
        QRegularExpression raw_regex(
            "^(?<batch>\\d{5})(_|-)(?<wafer>\\d{3})(_|-)(PSP)(_|-)(MAIN)(?<side>L|R)$"
        );
        //Create a regex to check if the raw_ID has a correct format of XXX_YYYYY_ZZZ(L|R)
        QRegularExpression corr_regex(
            "^(?<vendor>QPT|HPK)(_|-)(?<batch>\\d{5})(_|-)(?<wafer>\\d{3})(?<side>L|R)$"
        );

        QRegularExpressionMatch raw_result = raw_regex.match(MaPSA_rawID);
        QRegularExpressionMatch corr_result = corr_regex.match(MaPSA_rawID);

        if (raw_result.hasMatch()){
            //If the ID is in the barcode format, 
            // then extract the from it usefull numbers 
            // and create a proxy for the full name (w/o vendor)
            QString batch = raw_result.captured("batch");
            QString wafer = raw_result.captured("wafer");
            QString side = raw_result.captured("side");
            QString MaPSA_convID = QString("%1_%2%3")
                .arg(batch)
                .arg(wafer)
                .arg(side);
            return MaPSA_convID;

        }else if(corr_result.hasMatch()){
            //If the ID has already correct format, return it
            return MaPSA_rawID;

        }else{
            QMessageBox msgBox;
            msgBox.setWindowTitle("Warning - MaPSA ID");
            msgBox.setText(QString("MapPSA ID: %1 does not  follow the format conventions!\n"
                "Use XXX_YYYYY_ZZZ(L/R) or YYYYY_ZZZ_PSP_MAIN(L/R), where\n"
                "XXX - QPT or HPK,\n"
                "YYYYY and ZZZ are Si ingot ID and wafer ID written on the MaPSA box/envelope,\n"
                "L/R - side of the wafer.").arg(MaPSA_rawID));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }

            return QString();
    }
}
