#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

namespace lichess
{
    class Client
    {
    public:
        Client(QString accessToken);

    private:
        QString accessToken;
    };
}

#endif // CLIENT_H
