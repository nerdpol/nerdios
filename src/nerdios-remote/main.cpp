#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("nerdpol Inc.");
    QCoreApplication::setOrganizationDomain("nerdpol.io");
    QCoreApplication::setApplicationName("nerdios");

    return app.exec();
}
