#pragma once

// ClassRight.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Mongoose.h"
#include <string>
#include <iostream>

static struct mg_serve_http_opts s_http_server_opts;

static void ev_handler(struct mg_connection* nc, int ev, void* p) {
    if (ev == MG_EV_HTTP_REQUEST)
    {
        mg_serve_http(nc, (struct http_message*)p, s_http_server_opts);
    }
}//event handler function

int initServer(int port)
{
    //moongoose event manager
    struct mg_mgr mgr;
    //moongoose connection
    struct mg_connection* nc;

    std::string portToChar = std::to_string(port); // use std::to_string to cast to a string
    static char const* sPort = portToChar.c_str();
    //init moongoose
    mg_mgr_init(&mgr, NULL);

    std::cout << "Starting web server on port " << sPort << std::endl;

    nc = mg_bind(&mgr, sPort, ev_handler);

    //if connection fails
    if (nc == NULL) {
        std::cout << "Failed to create listener" << std::endl;
        return 1;
    }

    //set up http server options

    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.document_root = ".";
    s_http_server_opts.enable_directory_listing = "yes";



    //creates infinite loop - same thing as while (true)

    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);

    return 0;

}

int start()
{
    int port = 8080;
    //if port is not int
    if (std::cin.fail()) {
        port = 8080;
    }

    initServer(port);

    return 0;


}

