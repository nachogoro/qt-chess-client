#include "mainwindow.h"

#include <QCoreApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString lichessAccessToken = "lip_Ci1XUDGIzyPgtP1fXpG3";
    QString gameId = "VU9vgFYZ";

    // Set up the network access manager
    QNetworkAccessManager manager;

    // Set up the request
    QNetworkRequest request;
    request.setUrl(QUrl("https://lichess.org/game/export/" + gameId + "?evals=false&clocks=false"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Make the request and get the reply
    QNetworkReply *reply = manager.get(request);

    // Connect to the finished signal
    QObject::connect(reply, &QNetworkReply::finished, [&] {
        // Check the status code
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            // Success
            QByteArray response = reply->readAll();
            qDebug() << qPrintable(response);
            // Process the response data
        } else {
            // Failure
            // Handle the error
            qDebug() << "Failure :(";
        }

        // Clean up
        reply->deleteLater();
    });

    MainWindow w;
    w.show();
    return a.exec();
}
