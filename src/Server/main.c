#include <SDL2/SDL.h>
#include <enet/enet.h>

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occorred while initializing ENet!\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    ENetHost *server;
    ENetEvent event;

    address.host = ENET_HOST_ANY;
    address.port = 7777;

    server = enet_host_create(&address, 32, 1, 0, 0);
    if (server == NULL) {
        fprintf(stderr, "An error occurred while trying to create an Enet server host!\n");
        return EXIT_FAILURE;
    }

    // GAME LOOP START
    
    for ( ; ; ) {
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    fprintf(stdout, "A new client connected from %x:%u\n",
                            event.peer -> address.host,
                            event.peer -> address.port);
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    fprintf(stdout, "A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                            event.packet->dataLength,
                            event.packet->data,
                            event.peer->address.host,
                            event.peer->address.port,
                            event.channelID);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    fprintf(stdout, "%x:%u disconnected!\n",
                            event.peer -> address.host,
                            event.peer -> address.port);
                    break;
            }
        }
    }

    // GAME LOOP END

    enet_host_destroy(server);

    return EXIT_SUCCESS;
}
