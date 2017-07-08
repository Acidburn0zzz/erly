# Tutorials

> This directory contains tutorial code from erlang.org, along with
> a Makefile for practical demonstrations.


## Server

Run one server:

```
make server
```

The server comes up with an Erlang prompt. In principle you don't need to
do anything with the server, it responds to messages from clients. To stop
it, halt the Erlang executable:

```
halt().
```

## Client

Run one or more clients (in separate terminals):

```
make client
```

Clients have Erlang node names ping, pong, pang, peng and pyng. You can't
run more than five at once like this. Each client starts by pinging the
server with 3 messages. After that, use the functions defined by the
messenger example in the Erlang tutorial:

```
messenger:logon(bill).
messenger:message(fred, hey_there).
messenger:logoff().
halt().
```

