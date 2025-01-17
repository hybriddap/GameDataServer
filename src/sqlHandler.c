#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>

#include "credentials.h"
#include "inputHandler.h"

PGconn *conn;
PGresult *res;

void returnError(PGconn *conn) {
    fprintf(stderr, "Error connecting to DataBase: %s\n", PQerrorMessage(conn));
    PQfinish(conn);
}

int connectToDataBase()
{
    conn = PQconnectdb(conninfo);
    //check connection status
    if (PQstatus(conn) != CONNECTION_OK) {
        returnError(conn);
        return 1;
    }
    printer("Successfully connected to database!",0);
    return 0;
}

int executeQuery(char *query)
{
    res = PQexec(conn, query);
    //check query status
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        returnError(conn);
        return 1;
    }
    return 0;
}

void printResult()
{
    //get number of rows and columns
    int rows = PQntuples(res);
    int cols = PQnfields(res);

    //print column names
    printf("Query Results:\n");
    for (int i = 0; i < cols; i++) {
        printf("%s\t", PQfname(res, i));
    }
    printf("\n");

    //print each row of the result
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%s\t", PQgetvalue(res, i, j));
        }
        printf("\n");
    }
}

char * formatResult()
{
    int rows = PQntuples(res);
    int cols = PQnfields(res);

    size_t json_size = rows * cols * 100 + 2;  //rough estimation of size
    char *json = malloc(json_size);
    if (!json) {
        PQclear(res);
        return NULL;  //return when memory allocation fails
    }
    
    strcpy(json, "[");  //start JSON array
    for (int i = 0; i < rows; i++) {
        strcat(json, "{");
        for (int j = 0; j < cols; j++) {
            char temp[256];
            snprintf(temp, sizeof(temp), "\"%s\":\"%s\"%s",
                     PQfname(res, j), PQgetvalue(res, i, j), j < cols - 1 ? "," : "");
            strcat(json, temp);
        }
        strcat(json, "}");
        if (i < rows - 1) strcat(json, ",");
    }
    strcat(json, "]");  //end JSON array
    printer("Message successfully sent from database!",0);
    return json;
}

void closeConnection()
{
    //free result and close connection
    PQclear(res);
    PQfinish(conn);
    printer("Disconnected from database.",0);
}

int mainLoop() {

    //connect to db
    connectToDataBase();

    //select query
    executeQuery("SELECT id, name, age FROM test2");

    //print result
    printResult();

    //close connection
    closeConnection();

    return 0;
}
