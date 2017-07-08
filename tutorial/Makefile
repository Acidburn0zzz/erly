help:
	@echo "Usage: make <server|client>"
	@false

messenger.beam: messenger.erl
	erlc messenger.erl


RUN_SERVER=	messenger:start_server(), messenger:start_pong().
RUN_CLIENT=	messenger:ping(3).

server: messenger.beam
	erl -sname messenger -eval '$(RUN_SERVER)'

client: messenger.beam
	erl -sname ping -eval '$(RUN_CLIENT)' || \
	erl -sname pong -eval '$(RUN_CLIENT)' || \
	erl -sname pang -eval '$(RUN_CLIENT)' || \
	erl -sname peng -eval '$(RUN_CLIENT)' || \
	erl -sname pyng -eval '$(RUN_CLIENT)'

