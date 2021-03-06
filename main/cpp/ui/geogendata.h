#ifndef GEOGENDATA_H
#define GEOGENDATA_H

#include <QStringList>

class GeoGenData
{
public:
    GeoGenData();

    bool setFilenames(QString path, QStringList &missingFiles);

    QString communitiesFile;
    QString geogridFile;
    QString schoolsFile;
    QString universitiesFile;
    QString workplacesFile;
};

#endif // GEOGENDATA_H
