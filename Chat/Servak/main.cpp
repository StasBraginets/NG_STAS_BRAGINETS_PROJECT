#include <QCoreApplication>
#include "servak.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    servak server ("0.0.0.0", 5981); // Create server
    return a.exec();
}
