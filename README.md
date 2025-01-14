This is a work in progress server for a game I'm working on, built in Unity.

The server runs on PORT 4444 currently.

It simply just returns data to the client, doesn't accept any input from the clients.

Handles 1 client at a time.

However there are 2 threads:
    1 - Main Thread.
    2 - Sub thread used for local input to close the server properly.

The server can be closed by typing exit on the terminal. It can't be closed by a client!