# Erlang Pulley Plugin

> This plugin is known to the Steamworks Pulley as `erlang_node`,
> and it uses Erlang inter-process communication to send data
> to an Erlang node.

## Usage

In a pulleyscript, send tuples to this backend with

    -> erlang_node(tgt=<nodename>, msg=<service>)

both `nodename` and `service` are strings, which are used to
construct a message like the following:

    { <service>, <nodename> } ! { <add|remove>, { <tuple> } }

where <add|remove> is the operation being performed on the
tuple, and <tuple> is the data from LDAP -- all of which
is represented as an Erlang string.


