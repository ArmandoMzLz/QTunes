#include "sqlconnection.h"

sqlConnection::sqlConnection() {
    class sqlConnection {
    private:
        std::string hostName;
        std::string port;
        std::string dbName;
        std::string username;
        std::string password;

    public:
        sqlConnection(std::string sHostName, std::string sPort, std::string sDbName, std::string sUsername, std::string sPassword) {
            hostName = sHostName;
            port = sPort;
            dbName = sDbName;
            username = sUsername;
            password = sPassword;
        }

        std::string setHostName() const { return hostName; }
        std::string setPort() const { return port; }
        std::string setDbName() const { return dbName; }
        std::string setUsername() const { return username; }
        std::string setPassword() const { return password; }
    };
}

