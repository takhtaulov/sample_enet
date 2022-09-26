#include <SDL2/SDL.h>
#include <enet/enet.h>

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occorred while initializing ENet!\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetHost *client;
    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;

    client = enet_host_create(NULL, 1, 1, 0, 0);
    if (client == NULL) {
        fprintf(stderr, "An error occorred while trying to create an Enet client host!\n");
        return EXIT_FAILURE;
    }

    // TODO: Create a constant variable for 'magic' numbers
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 7777;

    peer = enet_host_connect(client, &address, 1, 0);
    if (peer == NULL) {
        fprintf(stderr, "No available peers for initiating an ENet connection!\n");
        return EXIT_FAILURE;
    }

    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
            fprintf(stdout, "Connection to 127.0.0.1:7777 succeeded!\n");
    }
    else {
        enet_peer_reset(peer);
        fprintf(stdout, "Connection to 127.0.0.1:7777 failed!\n");
        return EXIT_SUCCESS;
    }

    // GAME LOOP START

    while (enet_host_service(client, &event, 1000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                fprintf(stdout, "A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                        event.packet -> dataLength,
                        event.packet -> data,
                        event.peer -> address.host,
                        event.peer -> address.port,
                        event.channelID); 
                break;
        }
    }

    // GAME LOOP END

    enet_peer_disconnect(peer, 0);

    while (enet_host_service(client, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                fprintf(stdout, "Disconnection succeeded!\n");
                break;
        }
    }    

    return EXIT_SUCCESS;
}
