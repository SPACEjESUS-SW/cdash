# cdash

## Build
```bash
# Builds server and client programs
make server client
```

## Run
### Normal
```bash
# Server
./bin/server_app <port_no>

# Client
./bin/client_app <ip> <port_no> 0
```

### Bombard
```bash
./bin/client_app <ip> <port_no> 1
```