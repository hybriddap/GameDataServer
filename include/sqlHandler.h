#ifndef SQLHANDLER_H
#define SQLHANDLER_H

//includes


//variables

//methods
int connectToDataBase();
int executeQuery(char *query);
char * formatResult();
void closeConnection();

#endif